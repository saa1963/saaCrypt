/*
 * Copyright(C) 2000-2010 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 *
 * ����������� ���, ������������ � ���� �����, ������������
 * ������������� ��� ����� �������� � �� ����� ���� �����������
 * ��� ������ ����������.
 *
 * �������� ������-��� �� ����� �������
 * ��������������� �� ���������������� ����� ����.
 */

//--------------------------------------------------------------------
// � ������ ������� ��������������� ���������� ������� � web ��������
// �� ����������� ������.
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
#   include <windows.h>
#   include <winsock.h>
#   include <wincrypt.h>
#   include <wintrust.h>
#   include <schannel.h>
#   include <time.h>
#   define SECURITY_WIN32
#   include <security.h>
#   include <sspi.h>
#   define IS_SOCKET_ERROR(a) (a==SOCKET_ERROR)
#   include <WinCryptEx.h>
#else
#   include "CSP_WinDef.h"
#   include "CSP_WinCrypt.h"
#   include "CSP_Sspi.h"
#   include "CSP_SChannel.h"
#   include "CpSSP.h"
#   include <sys/types.h>
#   ifdef _AIX
#	include <fcntl.h>
#   else
#	include <sys/fcntl.h>
#   endif
#   include <sys/stat.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <errno.h>
#   include <unistd.h>

#   define INVALID_SOCKET (-1)
#   define IS_SOCKET_ERROR(a) (a<0)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr *LPSOCKADDR;
    static int WSAGetLastError()
    {
      return errno;
    }
#   define LocalAlloc(a, b) malloc(b)
#   define LocalFree free
#   define MoveMemory memmove
#   define closesocket close
#endif

#define IO_BUFFER_SIZE  0x10000

#define NT4_DLL_NAME TEXT("Security.dll")

static LPSTR   pszProxyServer  = "proxy";
static INT     iProxyPort      = 80;

// ����� ������������.
static LPSTR   pszServerName   = NULL;
static INT     iPortNumber     = 443;
static LPSTR   pszFileName     = "Default.Htm";
static DWORD   dwProtocol      = 0;

static HCERTSTORE      hMyCertStore = NULL;
static SCHANNEL_CRED   SchannelCred;

#ifdef _WIN32
static HMODULE g_hSecurity = NULL;
#endif

static PSecurityFunctionTable g_pSSPI;

static
SECURITY_STATUS
CreateCredentials(
    PCredHandle phCreds);

static INT
ConnectToServer(
    LPSTR pszServerName,
    INT   iPortNumber,
    SOCKET *pSocket);

static
SECURITY_STATUS
PerformClientHandshake(
    SOCKET          Socket,
    PCredHandle     phCreds,
    LPSTR           pszServerName,
    CtxtHandle *    phContext,
    SecBuffer *     pExtraData);

static
SECURITY_STATUS
ClientHandshakeLoop(
    SOCKET          Socket,
    PCredHandle     phCreds,
    CtxtHandle *    phContext,
    BOOL            fDoInitialRead,
    SecBuffer *     pExtraData);

static
SECURITY_STATUS
HttpsGetFile(
    SOCKET          Socket,
    PCredHandle     phCreds,
    CtxtHandle *    phContext,
    LPSTR           pszFileName);

static 
void
DisplayCertChain(
    PCCERT_CONTEXT  pServerCert,
    BOOL            fLocal);

static 
DWORD
VerifyServerCertificate(
    PCCERT_CONTEXT  pServerCert,
    PSTR            pszServerName,
    DWORD           dwCertFlags);

static
LONG
DisconnectFromServer(
    SOCKET          Socket, 
    PCredHandle     phCreds,
    CtxtHandle *    phContext);

static void
DisplayConnectionInfo(
    CtxtHandle *phContext);

static void
GetNewClientCredentials(
    CredHandle *phCreds,
    CtxtHandle *phContext);

//-------------------------------------------------------------
// ������� ������ ���������� �� ������������� ������� �������.
static void Usage(void)
{
    printf("\n");
    printf("USAGE: webclient -s<server> \n");
    printf("\n");
    printf("    -s<server>      DNS name of server.\n");
}//Usage


//-------------------------------------------------------------
// ������� �������� ����������.
static BOOL
LoadSecurityLibrary(void)
{
    INIT_SECURITY_INTERFACE         pInitSecurityInterface;
#ifdef _WIN32
    g_hSecurity = LoadLibrary(NT4_DLL_NAME);
    if(g_hSecurity == NULL)
    {
        printf("Error 0x%x loading %s.\n", GetLastError(), NT4_DLL_NAME);
        return FALSE;
    }

    pInitSecurityInterface = (INIT_SECURITY_INTERFACE)GetProcAddress(
                                    g_hSecurity,
                                    "InitSecurityInterfaceA");
    
#else
    pInitSecurityInterface=InitSecurityInterfaceA;
#endif

    if(pInitSecurityInterface == NULL)
    {
        printf("Error 0x%x reading InitSecurityInterface entry point.\n", 
               GetLastError());
        return FALSE;
    }

    g_pSSPI = pInitSecurityInterface();

    if(g_pSSPI == NULL)
    {
        printf("Error 0x%x reading security interface.\n",
               GetLastError());
        return FALSE;
    }

    return TRUE;
}//LoadSecurityLibrary

//-------------------------------------------------------------
// ������� ��������� ����������.

static void
UnloadSecurityLibrary(void)
{
#ifdef _WIN32
    FreeLibrary(g_hSecurity);
    g_hSecurity = NULL;
#endif
} //UnloadSecurityLibrary


//-------------------------------------------------------------
// ������� main.

#ifdef _WIN32
int _cdecl
#else
int
#endif
main(int argc, char *argv[])
{
#ifdef _WIN32
    WSADATA WsaData;
#endif
    SOCKET  Socket = INVALID_SOCKET;

    CredHandle hClientCreds;
    CtxtHandle hContext;
    BOOL fCredsInitialized = FALSE;
    BOOL fContextInitialized = FALSE;

    SecBuffer  ExtraData;
    SECURITY_STATUS Status;

    PCCERT_CONTEXT pRemoteCertContext = NULL;

    int ret = 1;
    INT i;
    INT iOption;
    PCHAR pszOption;

    //
    // ������ ��������� ������.
    //

    if(argc <= 1)
    {
        Usage();
        return 1;
    }

    for(i = 1; i < argc; i++) 
    {
        if(argv[i][0] == '/') argv[i][0] = '-';

        if(argv[i][0] != '-') 
        {
            printf("**** Invalid argument \"%s\"\n", argv[i]);
            Usage();
            return 1;
        }

        iOption = argv[i][1];
        pszOption = &argv[i][2];

        switch(iOption) 
        {
        case 's':
            pszServerName = pszOption;
            break;

        
        default:
            printf("**** Invalid option \"%s\"\n", argv[i]);
            Usage();
            return 1;
        }
    }


    if(!LoadSecurityLibrary())
    {
        printf("Error initializing the security library\n");
        goto cleanup;
    }

#ifdef _WIN32
    //
    // ������������� ���������� WinSock.
    //

    if(WSAStartup(0x0101, &WsaData) == SOCKET_ERROR)
    {
        printf("Error %d returned by WSAStartup\n", GetLastError());
        goto cleanup;
    }
#endif
    //
    // �������� ��������.
    //

    if(CreateCredentials(&hClientCreds))
    {
        printf("Error creating credentials\n");
        goto cleanup;
    }
    fCredsInitialized = TRUE;


    //
    // ���������� � ��������.
    //

    if(ConnectToServer(pszServerName, iPortNumber, &Socket))
    {
        printf("Error connecting to server\n");
        goto cleanup;
    }


	// 
        // ������������ �����
        //
    if(PerformClientHandshake(Socket,
                              &hClientCreds,
                              pszServerName,
                              &hContext,
                              &ExtraData))
    {
        printf("Error performing handshake\n");
        goto cleanup;
    }
    fContextInitialized = TRUE;


    //
    // �������������� �������� �������.
    //

    // ��������� ����������� �������.
    Status = g_pSSPI->QueryContextAttributes(&hContext,
                                             SECPKG_ATTR_REMOTE_CERT_CONTEXT,
                                             (PVOID)&pRemoteCertContext);
    if(Status != SEC_E_OK)
    {
        printf("Error 0x%x querying remote certificate\n", Status);
        goto cleanup;
    }

    // ����� ������� ������������ �������.
    DisplayCertChain(pRemoteCertContext, FALSE);

    // �������� ���������������� ����������� �������.
    Status = VerifyServerCertificate(pRemoteCertContext,
                                     pszServerName,
                                     0);
    if(Status)
    {
        // ���������� ������� �� ������������. �������� ���� ������������
	// ���������� � ������������ ��������, ���� ������������� �����
	// "��������� ����������".

        // ����� ����� �������� ����������.

        printf("**** Error 0x%x authenticating server credentials!\n", Status);
//        goto cleanup;
    }

    // ������������ ��������� ����������� �������.
    CertFreeCertificateContext(pRemoteCertContext);
    pRemoteCertContext = NULL;


    //
    // ����� ���������� � ����������. 
    //

    DisplayConnectionInfo(&hContext);


    //
    // ������ ����� � �������.
    //

    if(HttpsGetFile(Socket, 
                    &hClientCreds,
                    &hContext, 
                    pszFileName))
    {
        printf("Error fetching file from server\n");
        goto cleanup;
    }

    //
    // �������� ����������� ������� � �������� ����������.
    //

    if(DisconnectFromServer(Socket, &hClientCreds, &hContext))
    {
        printf("Error disconnecting from server\n");
        goto cleanup;
    }
    fContextInitialized = FALSE;
    Socket = INVALID_SOCKET;


    ret = 0;
cleanup:

    // ������������ ��������� ����������� �������.
    if(pRemoteCertContext)
    {
        CertFreeCertificateContext(pRemoteCertContext);
        pRemoteCertContext = NULL;
    }

    // ������������ ����������� SSPI ���������.
    if(fContextInitialized)
    {
        g_pSSPI->DeleteSecurityContext(&hContext);
    }

    // ������������ ����������� SSPI ��������.
    if(fCredsInitialized)
    {
        g_pSSPI->FreeCredentialsHandle(&hClientCreds);
    }

    // �������� ������.
    if(Socket != INVALID_SOCKET)
    {
        closesocket(Socket);
    }

#ifdef _WIN32
    // ���������� ������ ���������� WinSock.
    WSACleanup();
#endif

    // �������� ��������� ������������ "MY".
    if(hMyCertStore)
    {
        CertCloseStore(hMyCertStore, 0);
    }

    UnloadSecurityLibrary();

    printf("Done\n");
    return ret;
} // main

//-------------------------------------------------------------
// ������� �������� ��������.
static
SECURITY_STATUS
CreateCredentials(
    PCredHandle phCreds)            // out
{
    TimeStamp       tsExpiry;
    SECURITY_STATUS Status;

    DWORD           cSupportedAlgs = 0;
    ALG_ID          rgbSupportedAlgs[16];

    PCCERT_CONTEXT  pCertContext = NULL;

    // �������� ��������� ������������ "MY" , � ������� Internet Explorer
    // ������ ����������� �������.
    hMyCertStore = CertOpenSystemStore(0, "MY");

        if(!hMyCertStore)
        {
            printf("**** Error 0x%x returned by CertOpenSystemStore\n", 
                GetLastError());
            return SEC_E_NO_CREDENTIALS;
        }
   
   
    // ���������� ��������� Schannel ��������. 
    // � ������ ������� ������������ ������������ �������� � ����������.

    ZeroMemory(&SchannelCred, sizeof(SchannelCred));

    SchannelCred.dwVersion  = SCHANNEL_CRED_VERSION;
    if(pCertContext)
    {
        SchannelCred.cCreds     = 1;
        SchannelCred.paCred     = &pCertContext;
    }

    SchannelCred.grbitEnabledProtocols = dwProtocol;

    if(cSupportedAlgs)
    {
        SchannelCred.cSupportedAlgs    = cSupportedAlgs;
        SchannelCred.palgSupportedAlgs = rgbSupportedAlgs;
    }

    SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;

    // ���� SCH_CRED_MANUAL_CRED_VALIDATION ����������, ���������
    // ��������� � ������ ������� ���������� ������� ����������� "�������". 
    SchannelCred.dwFlags |= SCH_CRED_MANUAL_CRED_VALIDATION;

    //
    // �������� SSPI �������.
    //

    Status = g_pSSPI->AcquireCredentialsHandleA(
                        NULL,                   // Name of principal    
                        UNISP_NAME_A,           // Name of package
                        SECPKG_CRED_OUTBOUND,   // Flags indicating use
                        NULL,                   // Pointer to logon ID
                        &SchannelCred,          // Package specific data
                        NULL,                   // Pointer to GetKey() func
                        NULL,                   // Value to pass to GetKey()
                        phCreds,                // (out) Cred Handle
                        &tsExpiry);             // (out) Lifetime (optional)
    if(Status != SEC_E_OK)
    {
        printf("**** Error 0x%x returned by AcquireCredentialsHandle\n", Status);
        goto cleanup;
    }

cleanup:

    //
    // ������������ ��������� �����������. � Schannel ��� ������� ��� �����.
    //

    if(pCertContext)
    {
        CertFreeCertificateContext(pCertContext);
    }


    return Status;
} //CreateCredentials

//-------------------------------------------------------------
// �������, ��������������� ���������� � ��������.
static INT
ConnectToServer(
    LPSTR    pszServerName, // in
    INT      iPortNumber,   // in
    SOCKET * pSocket)       // out
{
  SOCKET Socket;
  struct sockaddr_in sin;
  struct hostent *hp;

  Socket = socket(PF_INET, SOCK_STREAM, 0);
  if(Socket == INVALID_SOCKET)
    {
      printf("**** Error %d creating socket\n", WSAGetLastError());
      return WSAGetLastError();
    }

  sin.sin_family = AF_INET;
  sin.sin_port = htons((u_short)iPortNumber);

  if((hp = gethostbyname(pszServerName)) == NULL)
    {
      printf("**** Error %d returned by gethostbyname\n", WSAGetLastError());
      return WSAGetLastError();
    }
  else
    {
      memcpy(&sin.sin_addr, hp->h_addr, 4);
    }
    

  if(IS_SOCKET_ERROR(connect(Socket, (struct sockaddr *)&sin, sizeof(sin))))
    {
      printf("**** Error %d connecting to \"%s\" (%s)\n", 
	     WSAGetLastError(),
	     pszServerName, 
	     inet_ntoa(sin.sin_addr));
      closesocket(Socket);
      return WSAGetLastError();
    }

  *pSocket = Socket;

  return SEC_E_OK;
} //ConnectToServer

//-------------------------------------------------------------
// �������, ����������� ������ ���������� � ��������.

static
LONG
DisconnectFromServer(
    SOCKET          Socket, 
    PCredHandle     phCreds,
    CtxtHandle *    phContext)
{
    DWORD           dwType;
    char*           pbMessage;
    DWORD           cbMessage;
    DWORD           cbData;

    SecBufferDesc   OutBuffer;
    SecBuffer       OutBuffers[1];
    DWORD           dwSSPIFlags;
    unsigned long   dwSSPIOutFlags;
    TimeStamp       tsExpiry;
    DWORD           Status;

    //
    // ����������� schannel � �������� ����������.
    //

    dwType = SCHANNEL_SHUTDOWN;

    OutBuffers[0].pvBuffer   = &dwType;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer   = sizeof(dwType);

    OutBuffer.cBuffers  = 1;
    OutBuffer.pBuffers  = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    Status = g_pSSPI->ApplyControlToken(phContext, &OutBuffer);

    if(FAILED(Status)) 
    {
        printf("**** Error 0x%x returned by ApplyControlToken\n", Status);
        goto cleanup;
    }

    //
    // ���������� SSL ���������, ����������� ������������ � ��������.
    //

    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
                  ISC_REQ_REPLAY_DETECT     |
                  ISC_REQ_CONFIDENTIALITY   |
                  ISC_RET_EXTENDED_ERROR    |
                  ISC_REQ_ALLOCATE_MEMORY   |
                  ISC_REQ_STREAM;

    OutBuffers[0].pvBuffer   = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer   = 0;

    OutBuffer.cBuffers  = 1;
    OutBuffer.pBuffers  = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    Status = g_pSSPI->InitializeSecurityContextA(
                    phCreds,
                    phContext,
                    NULL,
                    dwSSPIFlags,
                    0,
                    SECURITY_NATIVE_DREP,
                    NULL,
                    0,
                    phContext,
                    &OutBuffer,
                    &dwSSPIOutFlags,
                    &tsExpiry);

    if(FAILED(Status)) 
    {
        printf("**** Error 0x%x returned by InitializeSecurityContext\n", Status);
        goto cleanup;
    }

    pbMessage = OutBuffers[0].pvBuffer;
    cbMessage = OutBuffers[0].cbBuffer;


    //
    // ������� ����� ��������� �������.
    //

    if(pbMessage != NULL && cbMessage != 0)
    {
        cbData = send(Socket, pbMessage, cbMessage, 0);
        if(IS_SOCKET_ERROR(cbData) || cbData == 0)
        {
            Status = WSAGetLastError();
            printf("**** Error %d sending close notify\n", Status);
            goto cleanup;
        }

        printf("Sending Close Notify\n");
        printf("%d bytes of handshake data sent\n", cbData);

        
        // ������������ ��������� ������.
        g_pSSPI->FreeContextBuffer(pbMessage);
    }
    

cleanup:

    // ������������ ��������� ���������.
    g_pSSPI->DeleteSecurityContext(phContext);

    // �������� ������.
    closesocket(Socket);

    return Status;
} // DisconnectFromServer

//-------------------------------------------------------------
// ������� ������������ ����� ������� � ��������.
static
SECURITY_STATUS
PerformClientHandshake(
    SOCKET          Socket,         // in
    PCredHandle     phCreds,        // in
    LPSTR           pszServerName,  // in
    CtxtHandle *    phContext,      // out
    SecBuffer *     pExtraData)     // out
{
    SecBufferDesc   OutBuffer;
    SecBuffer       OutBuffers[1];
    DWORD           dwSSPIFlags;
    unsigned long   dwSSPIOutFlags;
    TimeStamp       tsExpiry;
    SECURITY_STATUS scRet;
    DWORD           cbData;

    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
                  ISC_REQ_REPLAY_DETECT     |
                  ISC_REQ_CONFIDENTIALITY   |
                  ISC_RET_EXTENDED_ERROR    |
                  ISC_REQ_ALLOCATE_MEMORY   |
                  ISC_REQ_STREAM;

    //
    //  ������������� ��������� ClientHello � ��������� ������.
    //

    OutBuffers[0].pvBuffer   = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer   = 0;

    OutBuffer.cBuffers = 1;
    OutBuffer.pBuffers = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    scRet = g_pSSPI->InitializeSecurityContextA(
                    phCreds,
                    NULL,
                    pszServerName,
                    dwSSPIFlags,
                    0,
                    SECURITY_NATIVE_DREP,
                    NULL,
                    0,
                    phContext,
                    &OutBuffer,
                    &dwSSPIOutFlags,
                    &tsExpiry);

    if(scRet != SEC_I_CONTINUE_NEEDED)
    {
        printf("**** Error %d returned by InitializeSecurityContext (1)\n", scRet);
        return scRet;
    }

    // ������� ������ �������.
    if(OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
    {
        cbData = send(Socket,
                      OutBuffers[0].pvBuffer,
                      OutBuffers[0].cbBuffer,
                      0);
        if(IS_SOCKET_ERROR(cbData) || cbData == 0)
        {
            printf("**** Error %d sending data to server (1)\n", WSAGetLastError());
            g_pSSPI->FreeContextBuffer(OutBuffers[0].pvBuffer);
            g_pSSPI->DeleteSecurityContext(phContext);
            return SEC_E_INTERNAL_ERROR;
        }

        printf("%d bytes of handshake data sent\n", cbData);

        
        // ������������ ��������� ������.
        g_pSSPI->FreeContextBuffer(OutBuffers[0].pvBuffer);
        OutBuffers[0].pvBuffer = NULL;
    }


    return ClientHandshakeLoop(Socket, phCreds, phContext, TRUE, pExtraData);
} // PerformClientHandshake

//-------------------------------------------------------------
// ������� ������ ����������� ����� �������� � ��������.
static
SECURITY_STATUS
ClientHandshakeLoop(
    SOCKET          Socket,         // in
    PCredHandle     phCreds,        // in
    CtxtHandle *    phContext,      // in, out
    BOOL            fDoInitialRead, // in
    SecBuffer *     pExtraData)     // out
{
    SecBufferDesc   InBuffer;
    SecBuffer       InBuffers[2];
    SecBufferDesc   OutBuffer;
    SecBuffer       OutBuffers[1];
    DWORD           dwSSPIFlags;
    unsigned long   dwSSPIOutFlags;
    TimeStamp       tsExpiry;
    SECURITY_STATUS scRet;
    DWORD           cbData;

    char*          IoBuffer;
    DWORD           cbIoBuffer;
    BOOL            fDoRead;


    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
                  ISC_REQ_REPLAY_DETECT     |
                  ISC_REQ_CONFIDENTIALITY   |
                  ISC_RET_EXTENDED_ERROR    |
                  ISC_REQ_ALLOCATE_MEMORY   |
                  ISC_REQ_STREAM;

    //
    // ���������� ������ ������.
    //

    IoBuffer = LocalAlloc(LMEM_FIXED, IO_BUFFER_SIZE);
    if(IoBuffer == NULL)
    {
        printf("**** Out of memory (1)\n");
        return SEC_E_INTERNAL_ERROR;
    }
    cbIoBuffer = 0;

    fDoRead = fDoInitialRead;


    // 
    // ���� �� ��� ����, ���� �� ���������� ����� �����������, 
    // ���� �� ���������� ������.
    //

    scRet = SEC_I_CONTINUE_NEEDED;

    while(scRet == SEC_I_CONTINUE_NEEDED        ||
          scRet == SEC_E_INCOMPLETE_MESSAGE     ||
          scRet == SEC_I_INCOMPLETE_CREDENTIALS) 
   {

        //
        // ������ ������ �� �������.
        //

        if(0 == cbIoBuffer || scRet == SEC_E_INCOMPLETE_MESSAGE)
        {
            if(fDoRead)
            {
                cbData = recv(Socket, 
                              IoBuffer + cbIoBuffer, 
                              IO_BUFFER_SIZE - cbIoBuffer, 
                              0);
                if(IS_SOCKET_ERROR(cbData))
                {
                    printf("**** Error %d reading data from server\n", WSAGetLastError());
                    scRet = SEC_E_INTERNAL_ERROR;
                    break;
                }
                else if(cbData == 0)
                {
                    printf("**** Server unexpectedly disconnected\n");
                    scRet = SEC_E_INTERNAL_ERROR;
                    break;
                }

                printf("%d bytes of handshake data received\n", cbData);

                
                cbIoBuffer += cbData;
            }
            else
            {
                fDoRead = TRUE;
            }
        }


        //
        // ��������� ������� �������. Buffer 0 �������� ������, ����������
        // �� �������. Schannel ��������� ��������� ��� ��� �� ���.
        // ���������� ������ (� ����� ������) ������������� �  buffer 1 �
        // �������� ��� ������ SECBUFFER_EXTRA.
        //

        InBuffers[0].pvBuffer   = IoBuffer;
        InBuffers[0].cbBuffer   = cbIoBuffer;
        InBuffers[0].BufferType = SECBUFFER_TOKEN;

        InBuffers[1].pvBuffer   = NULL;
        InBuffers[1].cbBuffer   = 0;
        InBuffers[1].BufferType = SECBUFFER_EMPTY;

        InBuffer.cBuffers       = 2;
        InBuffer.pBuffers       = InBuffers;
        InBuffer.ulVersion      = SECBUFFER_VERSION;

        //
        // ��������� �������� �������. ������������� ������������ ����� �������, 
	// ����� pvBuffer �������� NULL. 
	// ��� ������� ��� ����, ����� � ������ ������� �� ���� ������������� ���������
	// ������������ ������.
        //

        OutBuffers[0].pvBuffer  = NULL;
        OutBuffers[0].BufferType= SECBUFFER_TOKEN;
        OutBuffers[0].cbBuffer  = 0;

        OutBuffer.cBuffers      = 1;
        OutBuffer.pBuffers      = OutBuffers;
        OutBuffer.ulVersion     = SECBUFFER_VERSION;

        //
        // ����� InitializeSecurityContext.
        //

        scRet = g_pSSPI->InitializeSecurityContextA(phCreds,
                                          phContext,
                                          NULL,
                                          dwSSPIFlags,
                                          0,
                                          SECURITY_NATIVE_DREP,
                                          &InBuffer,
                                          0,
                                          NULL,
                                          &OutBuffer,
                                          &dwSSPIOutFlags,
                                          &tsExpiry);

        //
        // ���� InitializeSecurityContext ������� ��������� (��� ���� ��������� 
        // ���� �� ���������������� ������), �� ����������� ������� ���������
        // ������ �������.
        //

        if(scRet == SEC_E_OK                ||
           scRet == SEC_I_CONTINUE_NEEDED   ||
           FAILED(scRet) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))
        {
            if(OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
            {
                cbData = send(Socket,
                              OutBuffers[0].pvBuffer,
                              OutBuffers[0].cbBuffer,
                              0);
                if(IS_SOCKET_ERROR(cbData) || cbData == 0)
                {
                    printf("**** Error %d sending data to server (2)\n", 
                        WSAGetLastError());
                    g_pSSPI->FreeContextBuffer(OutBuffers[0].pvBuffer);
                    g_pSSPI->DeleteSecurityContext(phContext);
                    return SEC_E_INTERNAL_ERROR;
                }

                printf("%d bytes of handshake data sent\n", cbData);

               
                // ������������ ��������� ������.
                g_pSSPI->FreeContextBuffer(OutBuffers[0].pvBuffer);
                OutBuffers[0].pvBuffer = NULL;
            }
        }


        //
        // ���� InitializeSecurityContext ������� ������ SEC_E_INCOMPLETE_MESSAGE,
        // ����� ���������� ��������� ������� ���������� ������ �� ������� � ���������
	// ������� �����.
        //

        if(scRet == SEC_E_INCOMPLETE_MESSAGE)
        {
            continue;
        }


        //
        // ���� InitializeSecurityContext ���������� SEC_E_OK, ����� 
        // ����� ������� ���������� �������.
        //

        if(scRet == SEC_E_OK)
        {
            //
            // ���� "extra" ����� �������� ������, �� ��� �������� �������
            // ��������� ������������. �� ���������� ���������. 
	    // ���������� ����� ���������� �� ��� ������ DecryptMessage.
            //

            printf("Handshake was successful\n");

            if(InBuffers[1].BufferType == SECBUFFER_EXTRA)
            {
                pExtraData->pvBuffer = LocalAlloc(LMEM_FIXED, 
                                                  InBuffers[1].cbBuffer);
                if(pExtraData->pvBuffer == NULL)
                {
                    printf("**** Out of memory (2)\n");
                    return SEC_E_INTERNAL_ERROR;
                }

                MoveMemory(pExtraData->pvBuffer,
                           IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer),
                           InBuffers[1].cbBuffer);

                pExtraData->cbBuffer   = InBuffers[1].cbBuffer;
                pExtraData->BufferType = SECBUFFER_TOKEN;

                printf("%d bytes of app data was bundled with handshake data\n",
                    pExtraData->cbBuffer);
            }
            else
            {
                pExtraData->pvBuffer   = NULL;
                pExtraData->cbBuffer   = 0;
                pExtraData->BufferType = SECBUFFER_EMPTY;
            }

            //
            // �����
            //
	    {
		// Sample for usage QueryContextAttributes() & native TimeStamp
		SecPkgContext_Lifespan ls;
		double hi, lo;
		time_t clock_1970;

		scRet = g_pSSPI->QueryContextAttributes(phContext,
					SECPKG_ATTR_LIFESPAN, &ls);
		hi = ls.tsExpiry.HighPart;
		lo = ls.tsExpiry.LowPart;
		    // Convert 100-ns interval since January 1, 1601 (UTC) 
		    // to 1-sec interval science January 1, 1970, UTC
		clock_1970 = (time_t)(
			((ldexp(hi, 32) + lo)*100.e-9)
			- 11644473600. //SystemTimeToFileTime({.wYear = 1970, .wMonth = 1, .wDay = 1}... 
		    );
		    // Convert UTC time_t to local time string
		printf("Security Context of this session valid till %s (local time)\n", ctime(&clock_1970));
	    }
	    {
		// Sample for usage QueryContextAttributes() & FileTimeToSystemTime()
		FILETIME   ft;
		SYSTEMTIME st;
		unsigned hi;
		unsigned lo;
		SecPkgContext_Lifespan ls;

		scRet = g_pSSPI->QueryContextAttributes(phContext,
					SECPKG_ATTR_LIFESPAN, &ls);
		hi = ls.tsStart.HighPart;
		lo = ls.tsStart.LowPart;
		memcpy(&ft, &ls.tsStart, sizeof(ft));
		FileTimeToSystemTime(&ft,&st);
		printf("Connection start {%x, %x}: %d/%d/%d %d:%d:%d.%03d UTC\n", 
			    hi, lo,
			    st.wYear, st.wMonth, st.wDay,
			    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		hi = ls.tsExpiry.HighPart;
		lo = ls.tsExpiry.LowPart;
		memcpy(&ft, &ls.tsExpiry, sizeof(ft));
		FileTimeToSystemTime(&ft,&st);
		printf("Connection expiry {%x, %x}: %d/%d/%d %d:%d:%d.%03d UTC\n", 
			    hi, lo,
			    st.wYear, st.wMonth, st.wDay,
			    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	    }
            break;
        }


        //
        // �������� �� ������.
        //

        if(FAILED(scRet))
        {
            printf("**** Error 0x%x returned by InitializeSecurityContext (2)\n", scRet);
            break;
        }


        //
        // ���� InitializeSecurityContext ���������� SEC_I_INCOMPLETE_CREDENTIALS,
        // �� ������ ������ ��� �������� �������������� �������. 
        //

        if(scRet == SEC_I_INCOMPLETE_CREDENTIALS)
        {
            //
            // ������. ������ �������� �������������� �������, �� ����������
            // ������ �� �������� ����������� �������.
            //

            // 
            // ��� ������� ������ ������ ���������� ������������ �������
            // ("issuers"), ���������� �� ������� � �������� �����
            // ���������� ���������� �������. ���� ��� ������� ��������� �������,
            // �� ���������� ���������� ��� ������ ������ �����������.
            // � ��������� ������ �������������� ������� ����������
	    // ���������� �������� (��������� ������� ������).
            //
            
            GetNewClientCredentials(phCreds, phContext);

            // ��������� �������.
            fDoRead = FALSE;
            scRet = SEC_I_CONTINUE_NEEDED;

	    // ���������� ������ Platform SDK!
	    // �������, ��� �� ���� ���������� �� ����� ��������� ������
	    cbIoBuffer = 0;

            continue;
        }


        //
        // ����������� ���� ������ �� "extra" ������ � ��������� �������.
        //

        if ( InBuffers[1].BufferType == SECBUFFER_EXTRA )
        {
            MoveMemory(IoBuffer,
                       IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer),
                       InBuffers[1].cbBuffer);

            cbIoBuffer = InBuffers[1].cbBuffer;
        }
        else
        {
            cbIoBuffer = 0;
        }
    }

    // ����������� ��������� ��������� � ������ ������������ ������.
    if(FAILED(scRet))
    {
        g_pSSPI->DeleteSecurityContext(phContext);
    }

    LocalFree(IoBuffer);

    return scRet;
} // ClientHandshakeLoop


//-------------------------------------------------------------
// ������� ��������� ����� ��� ������ Https.
static
SECURITY_STATUS
HttpsGetFile(
    SOCKET          Socket,         // in
    PCredHandle     phCreds,        // in
    CtxtHandle *    phContext,      // in
    LPSTR           pszFileName)    // in
{
    SecPkgContext_StreamSizes Sizes;
    SECURITY_STATUS scRet;
    SecBufferDesc   Message;
    SecBuffer       Buffers[4];
    SecBuffer *     pDataBuffer;
    SecBuffer *     pExtraBuffer;
    SecBuffer       ExtraBuffer;

    char* pbIoBuffer;
    DWORD cbIoBuffer;
    DWORD cbIoBufferLength;
    char* pbMessage;
    DWORD cbMessage;

    DWORD cbData;
    INT   i;


    //
    // ������ ������� ��������� ������������.
    //

    scRet = g_pSSPI->QueryContextAttributes(phContext,
                                   SECPKG_ATTR_STREAM_SIZES,
                                   &Sizes);
    if(scRet != SEC_E_OK)
    {
        printf("**** Error 0x%x reading SECPKG_ATTR_STREAM_SIZES\n", scRet);
        return scRet;
    }

    printf("\nHeader: %d, Trailer: %d, MaxMessage: %d\n",
        Sizes.cbHeader,
        Sizes.cbTrailer,
        Sizes.cbMaximumMessage);

    //
    // ��������� �������� ������. �������� �����, ������������ EncryptMessage,
    // ������ �� ������������ �� �������� 'Sizes.cbMaximumMessage', ������� 
    // ������ ������ ����� ���� ��������, �������� � ��������� ��������� � 
    // �������������� �����.
    // 

    cbIoBufferLength = Sizes.cbHeader + 
                       Sizes.cbMaximumMessage +
                       Sizes.cbTrailer;// ����� ���� ����� ���������� 
    //������� � ������� ��� ����� ������ ����� cbIoBufferLength
    // ����������� ����� ������� + 2048 ���� ��. RFC 2246

    pbIoBuffer = LocalAlloc(LMEM_FIXED, cbIoBufferLength);
    if(pbIoBuffer == NULL)
    {
        printf("**** Out of memory (2)\n");
        return SEC_E_INTERNAL_ERROR;
    }


    //
    // ���������� HTTP ������� �������.
    //

    // �������� ����������� '/' �� ����� �����, ������ ���� ������ ����.
    if(pszFileName && 
       strlen(pszFileName) > 1 && 
       pszFileName[strlen(pszFileName) - 1] == '/')
    {
        pszFileName[strlen(pszFileName)-1] = 0;
    }

    // ���������� HTTP �������, ���������� �� "header size" ���� � ������.
    // ��� ��������� Schannel ��������� �������� ������������.
    pbMessage = pbIoBuffer + Sizes.cbHeader;

    // ���������� HTTP �������. �� ������, ��� ������������ ������ ���������.
    // ���� ��� �� ���, ��� ���������� break.
    sprintf(pbMessage, 
            "GET /%s HTTP/1.0\r\nUser-Agent: Webclient\r\nAccept:*/*\r\n\r\n", 
            pszFileName);
    printf("\nHTTP request: %s\n", pbMessage);

    cbMessage = (DWORD)strlen(pbMessage);

    printf("Sending plaintext: %d bytes\n", cbMessage);

  
    //
    // ������������ HTTP �������.
    //

    Buffers[0].pvBuffer     = pbIoBuffer;
    Buffers[0].cbBuffer     = Sizes.cbHeader;
    Buffers[0].BufferType   = SECBUFFER_STREAM_HEADER;

    Buffers[1].pvBuffer     = pbMessage;
    Buffers[1].cbBuffer     = cbMessage;
    Buffers[1].BufferType   = SECBUFFER_DATA;

    Buffers[2].pvBuffer     = pbMessage + cbMessage;
    Buffers[2].cbBuffer     = Sizes.cbTrailer;
    Buffers[2].BufferType   = SECBUFFER_STREAM_TRAILER;

    Buffers[3].BufferType   = SECBUFFER_EMPTY;

    Message.ulVersion       = SECBUFFER_VERSION;
    Message.cBuffers        = 4;
    Message.pBuffers        = Buffers;

    scRet = g_pSSPI->EncryptMessage(phContext, 0, &Message, 0);

    if(FAILED(scRet))
    {
        printf("**** Error 0x%x returned by EncryptMessage\n", scRet);
        return scRet;
    }


    // 
    // ������� ������������� ������ �������.
    //

    cbData = send(Socket,
                  pbIoBuffer,
                  Buffers[0].cbBuffer + Buffers[1].cbBuffer + Buffers[2].cbBuffer,
                  0);
    if(IS_SOCKET_ERROR(cbData) || cbData == 0)
    {
        printf("**** Error %d sending data to server (3)\n", 
            WSAGetLastError());
        g_pSSPI->DeleteSecurityContext(phContext);
        return SEC_E_INTERNAL_ERROR;
    }

    printf("%d bytes of application data sent\n", cbData);

    
    //
    // ������ ������ �� ������� �� �� ���������.
    //

    cbIoBuffer = 0;

    for(;;)
    {
        //
        // ������ ������.
        //

        if(0 == cbIoBuffer || scRet == SEC_E_INCOMPLETE_MESSAGE)
        {
            cbData = recv(Socket, 
                          pbIoBuffer + cbIoBuffer, 
                          cbIoBufferLength - cbIoBuffer, 
                          0);
            if(IS_SOCKET_ERROR(cbData))
            {
                printf("**** Error %d reading data from server\n", WSAGetLastError());
                break;
            }
            else if(cbData == 0)
            {
                // ��������� ������ ���������� � ��������.
                if(cbIoBuffer)
                {
                    printf("**** Server unexpectedly disconnected\n");
                    scRet = SEC_E_INTERNAL_ERROR;
                    return scRet;
                }
                else
                {
                    break;
                }
            }
            else
            {
                printf("%d bytes of (encrypted) application data received\n", cbData);

                
                cbIoBuffer += cbData;
            }
        }

        // 
        // ������� ������������� �������� �� ������� ������.
        //

        Buffers[0].pvBuffer     = pbIoBuffer;
        Buffers[0].cbBuffer     = cbIoBuffer;
        Buffers[0].BufferType   = SECBUFFER_DATA;

        Buffers[1].BufferType   = SECBUFFER_EMPTY;
        Buffers[2].BufferType   = SECBUFFER_EMPTY;
        Buffers[3].BufferType   = SECBUFFER_EMPTY;

        Message.ulVersion       = SECBUFFER_VERSION;
        Message.cBuffers        = 4;
        Message.pBuffers        = Buffers;

        scRet = g_pSSPI->DecryptMessage(phContext, &Message, 0, NULL);

        if(scRet == SEC_E_INCOMPLETE_MESSAGE)
        {
            // ������� ����� �������� ������ �������� ������������� ������.
            // ����������� ����� � ������ ������.
            continue;
        }

        // ������ �������� ������
        if(scRet == SEC_I_CONTEXT_EXPIRED)
            break;

        if( scRet != SEC_E_OK && 
            scRet != SEC_I_RENEGOTIATE && 
            scRet != SEC_I_CONTEXT_EXPIRED)
        {
            printf("**** Error 0x%x returned by DecryptMessage\n", scRet);
            return scRet;
        }

        // ������������ ������ � (�������������) ���������� �������.
        pDataBuffer  = NULL;
        pExtraBuffer = NULL;
        for(i = 1; i < 4; i++)
        {

            if(pDataBuffer == NULL && Buffers[i].BufferType == SECBUFFER_DATA)
            {
                pDataBuffer = &Buffers[i];
                printf("Buffers[%d].BufferType = SECBUFFER_DATA\n",i);
            }
            if(pExtraBuffer == NULL && Buffers[i].BufferType == SECBUFFER_EXTRA)
            {
                pExtraBuffer = &Buffers[i];
            }
        }

        // ����� ��� ��������� �������������� ������.
        if(pDataBuffer)
        {
            printf("Decrypted data: %d bytes\n", pDataBuffer->cbBuffer);
        }

        // ������� ���� "extra" ������ �� ������� �����.
        if(pExtraBuffer)
        {
            MoveMemory(pbIoBuffer, pExtraBuffer->pvBuffer, pExtraBuffer->cbBuffer);
            cbIoBuffer = pExtraBuffer->cbBuffer;
        }
        else
        {
            cbIoBuffer = 0;
        }

        if(scRet == SEC_I_RENEGOTIATE)
        {
            // ������ ���������� ������ ����� ����������� � ��������.

            printf("Server requested renegotiate!\n");

            scRet = ClientHandshakeLoop(Socket, 
                                        phCreds, 
                                        phContext, 
                                        FALSE, 
                                        &ExtraBuffer);
            if(scRet != SEC_E_OK)
            {
                return scRet;
            }

            // ������� ���� "extra" ������ �� ������� �����.
            if(ExtraBuffer.pvBuffer)
            {
                MoveMemory(pbIoBuffer, ExtraBuffer.pvBuffer, ExtraBuffer.cbBuffer);
                cbIoBuffer = ExtraBuffer.cbBuffer;
            }
        }
    }

    return SEC_E_OK;
} // HttpsGetFile

//-------------------------------------------------------------
// ������� ������ ������� ������������.
static 
void
DisplayCertChain(
    PCCERT_CONTEXT  pServerCert,
    BOOL            fLocal)
{
    CHAR szName[1000];
    PCCERT_CONTEXT pCurrentCert;
    PCCERT_CONTEXT pIssuerCert;
    DWORD dwVerificationFlags;

    printf("\n");

    // ����� ����� �������
    if(!CertNameToStr(pServerCert->dwCertEncodingType,
                      &pServerCert->pCertInfo->Subject,
                      CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
                      szName, sizeof(szName)))
    {
        printf("**** Error 0x%x building subject name\n", GetLastError());
    }
    if(fLocal)
    {
        printf("Client subject: %s\n", szName);
    }
    else
    {
        printf("Server subject: %s\n", szName);
    }
    if(!CertNameToStr(pServerCert->dwCertEncodingType,
                      &pServerCert->pCertInfo->Issuer,
                      CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
                      szName, sizeof(szName)))
    {
        printf("**** Error 0x%x building issuer name\n", GetLastError());
    }
    if(fLocal)
    {
        printf("Client issuer: %s\n", szName);
    }
    else
    {
        printf("Server issuer: %s\n\n", szName);
    }


    // ����� ������� ������������
    pCurrentCert = pServerCert;
    while(pCurrentCert != NULL)
    {
        dwVerificationFlags = 0;
        pIssuerCert = CertGetIssuerCertificateFromStore(pServerCert->hCertStore,
                                                        pCurrentCert,
                                                        NULL,
                                                        &dwVerificationFlags);
        if(pIssuerCert == NULL)
        {
            if(pCurrentCert != pServerCert)
            {
                CertFreeCertificateContext(pCurrentCert);
            }
            break;
        }

        if(!CertNameToStr(pIssuerCert->dwCertEncodingType,
                          &pIssuerCert->pCertInfo->Subject,
                          CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
                          szName, sizeof(szName)))
        {
            printf("**** Error 0x%x building subject name\n", GetLastError());
        }
        printf("CA subject: %s\n", szName);
        if(!CertNameToStr(pIssuerCert->dwCertEncodingType,
                          &pIssuerCert->pCertInfo->Issuer,
                          CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
                          szName, sizeof(szName)))
        {
            printf("**** Error 0x%x building issuer name\n", GetLastError());
        }
        printf("CA issuer: %s\n\n", szName);

        if(pCurrentCert != pServerCert)
        {
            CertFreeCertificateContext(pCurrentCert);
        }
        pCurrentCert = pIssuerCert;
        pIssuerCert = NULL;
    }
} // DisplayCertChain


//-------------------------------------------------------------
// ������� ������ ������ ���������� ��� �������� ������� ������������.

static
void
DisplayWinVerifyTrustError(DWORD Status)
{
    LPSTR pszName = NULL;

    switch(Status)
    {
    case CERT_E_EXPIRED:                pszName = "CERT_E_EXPIRED";                 break;
    case CERT_E_VALIDITYPERIODNESTING:  pszName = "CERT_E_VALIDITYPERIODNESTING";   break;
    case CERT_E_ROLE:                   pszName = "CERT_E_ROLE";                    break;
    case CERT_E_PATHLENCONST:           pszName = "CERT_E_PATHLENCONST";            break;
    case CERT_E_CRITICAL:               pszName = "CERT_E_CRITICAL";                break;
    case CERT_E_PURPOSE:                pszName = "CERT_E_PURPOSE";                 break;
    case CERT_E_ISSUERCHAINING:         pszName = "CERT_E_ISSUERCHAINING";          break;
    case CERT_E_MALFORMED:              pszName = "CERT_E_MALFORMED";               break;
    case CERT_E_UNTRUSTEDROOT:          pszName = "CERT_E_UNTRUSTEDROOT";           break;
    case CERT_E_CHAINING:               pszName = "CERT_E_CHAINING";                break;
    case TRUST_E_FAIL:                  pszName = "TRUST_E_FAIL";                   break;
    case CERT_E_REVOKED:                pszName = "CERT_E_REVOKED";                 break;
    case CERT_E_UNTRUSTEDTESTROOT:      pszName = "CERT_E_UNTRUSTEDTESTROOT";       break;
    case CERT_E_REVOCATION_FAILURE:     pszName = "CERT_E_REVOCATION_FAILURE";      break;
    case CERT_E_CN_NO_MATCH:            pszName = "CERT_E_CN_NO_MATCH";             break;
    case CERT_E_WRONG_USAGE:            pszName = "CERT_E_WRONG_USAGE";             break;
    default:                            pszName = "(unknown)";                      break;
    }

    printf("Error 0x%x (%s) returned by CertVerifyCertificateChainPolicy!\n", 
        Status, pszName);
} //DisplayWinVerifyTrustError

//-------------------------------------------------------------
// ������� �������� ����������� �������.
static 
DWORD
VerifyServerCertificate(
    PCCERT_CONTEXT  pServerCert,
    PSTR            pszServerName,
    DWORD           dwCertFlags)
{
    HTTPSPolicyCallbackData  polHttps;
    CERT_CHAIN_POLICY_PARA   PolicyPara;
    CERT_CHAIN_POLICY_STATUS PolicyStatus;
    CERT_CHAIN_PARA          ChainPara;
    PCCERT_CHAIN_CONTEXT     pChainContext = NULL;

    LPSTR rgszUsages[] = {  szOID_PKIX_KP_SERVER_AUTH,
                            szOID_SERVER_GATED_CRYPTO,
                            szOID_SGC_NETSCAPE };
    DWORD cUsages = sizeof(rgszUsages) / sizeof(LPSTR);

    PWSTR   pwszServerName = NULL;
    DWORD   cchServerName;
    HRESULT   Status;

    if(pServerCert == NULL)
    {
        Status = SEC_E_WRONG_PRINCIPAL;
        goto cleanup;
    }


    //
    // �������������� ����� ������� � unicode.
    //

    if(pszServerName == NULL || strlen(pszServerName) == 0)
    {
        Status = SEC_E_WRONG_PRINCIPAL;
        goto cleanup;
    }

    cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, NULL, 0);
    pwszServerName = LocalAlloc(LMEM_FIXED, cchServerName * sizeof(WCHAR));
    if(pwszServerName == NULL)
    {
        Status = SEC_E_INSUFFICIENT_MEMORY;
        goto cleanup;
    }
    cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, pwszServerName, cchServerName);
    if(cchServerName == 0)
    {
        Status = SEC_E_WRONG_PRINCIPAL;
        goto cleanup;
    }


    //
    // ���������� ������� ������������.
    //

    ZeroMemory(&ChainPara, sizeof(ChainPara));
    ChainPara.cbSize = sizeof(ChainPara);
    ChainPara.RequestedUsage.dwType = USAGE_MATCH_TYPE_OR;
    ChainPara.RequestedUsage.Usage.cUsageIdentifier     = cUsages;
    ChainPara.RequestedUsage.Usage.rgpszUsageIdentifier = rgszUsages;

    if(!CertGetCertificateChain(
                            NULL,
                            pServerCert,
                            NULL,
                            pServerCert->hCertStore,
                            &ChainPara,
                            0,
                            NULL,
                            &pChainContext))
    {
        Status = GetLastError();
        printf("Error 0x%x returned by CertGetCertificateChain!\n", Status);
        goto cleanup;
    }


    //
    // �������� ������� ������������.
    // 

    ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
    polHttps.cbStruct           = sizeof(HTTPSPolicyCallbackData);
    polHttps.dwAuthType         = AUTHTYPE_SERVER;
    polHttps.fdwChecks          = dwCertFlags;
    polHttps.pwszServerName     = pwszServerName;

    memset(&PolicyPara, 0, sizeof(PolicyPara));
    PolicyPara.cbSize            = sizeof(PolicyPara);
    PolicyPara.pvExtraPolicyPara = &polHttps;

    memset(&PolicyStatus, 0, sizeof(PolicyStatus));
    PolicyStatus.cbSize = sizeof(PolicyStatus);

    if(!CertVerifyCertificateChainPolicy(
                            CERT_CHAIN_POLICY_SSL,
                            pChainContext,
                            &PolicyPara,
                            &PolicyStatus))
    {
        Status = GetLastError();
        printf("Error 0x%x returned by CertVerifyCertificateChainPolicy!\n", Status);
        goto cleanup;
    }

    if(PolicyStatus.dwError)
    {
        Status = PolicyStatus.dwError;
        DisplayWinVerifyTrustError(Status); 
        goto cleanup;
    }


    Status = SEC_E_OK;

cleanup:

    if(pChainContext)
    {
        CertFreeCertificateChain(pChainContext);
    }

    if(pwszServerName)
    {
        LocalFree(pwszServerName);
    }

    return Status;
} // VerifyServerCertificate


//-------------------------------------------------------------
// ������� ������ ���������� � ����������.

static
void
DisplayConnectionInfo(
    CtxtHandle *phContext)
{
    SECURITY_STATUS Status;
    SecPkgContext_ConnectionInfo ConnectionInfo;

    Status = g_pSSPI->QueryContextAttributes(phContext,
                                    SECPKG_ATTR_CONNECTION_INFO,
                                    (PVOID)&ConnectionInfo);
    if(Status != SEC_E_OK)
    {
        printf("Error 0x%x querying connection info\n", Status);
        return;
    }

    printf("\n");

    switch(ConnectionInfo.dwProtocol)
    {
        case SP_PROT_TLS1_CLIENT:
            printf("Protocol: TLS1\n");
            break;

        default:
            printf("Protocol: 0x%x\n", ConnectionInfo.dwProtocol);
    }

    switch(ConnectionInfo.aiCipher)
    {
        case CALG_G28147: 
            printf("Cipher: Gost 28147-89\n");
            break;

        default: 
            printf("Cipher: 0x%x\n", ConnectionInfo.aiCipher);
    }

    printf("Cipher strength: %d\n", ConnectionInfo.dwCipherStrength);

    switch(ConnectionInfo.aiHash)
    {
        case CALG_GR3411: 
            printf("Hash: Gost R 34.11-94\n");
            break;

        default: 
            printf("Hash: 0x%x\n", ConnectionInfo.aiHash);
    }

    printf("Hash strength: %d\n", ConnectionInfo.dwHashStrength);

    switch(ConnectionInfo.aiExch)
    {
        case CALG_DH_EPHEM:
            printf("Key exchange: DH Ephemeral\n");
            break;

        default: 
            printf("Key exchange: 0x%x\n", ConnectionInfo.aiExch);
    }

    printf("Key exchange strength: %d\n", ConnectionInfo.dwExchStrength);
} //DisplayConnectionInfo


//-------------------------------------------------------------
// ������� ��������� ������ �������� �������.

static
void
GetNewClientCredentials(
    CredHandle *phCreds,
    CtxtHandle *phContext)
{
    CredHandle hCreds;
    SecPkgContext_IssuerListInfoEx IssuerListInfo;
    PCCERT_CHAIN_CONTEXT pChainContext;
    CERT_CHAIN_FIND_BY_ISSUER_PARA FindByIssuerPara;
    PCCERT_CONTEXT  pCertContext;
    TimeStamp       tsExpiry;
    SECURITY_STATUS Status;

    //
    // ������ ������ ���������� ��������� �� schannel.
    //

    Status = g_pSSPI->QueryContextAttributes(phContext,
                                    SECPKG_ATTR_ISSUER_LIST_EX,
                                    (PVOID)&IssuerListInfo);
    if(Status != SEC_E_OK)
    {
        printf("Error 0x%x querying issuer list info\n", Status);
        return;
    }

    //
    // ������������ ������������ �������.
    //

    ZeroMemory(&FindByIssuerPara, sizeof(FindByIssuerPara));

    FindByIssuerPara.cbSize = sizeof(FindByIssuerPara);
    FindByIssuerPara.pszUsageIdentifier = szOID_PKIX_KP_CLIENT_AUTH;
    FindByIssuerPara.dwKeySpec = 0;
    FindByIssuerPara.cIssuer   = IssuerListInfo.cIssuers;
    FindByIssuerPara.rgIssuer  = IssuerListInfo.aIssuers;

    pChainContext = NULL;

    for(;;)
    {
        // ����� ������� ������������.
        pChainContext = CertFindChainInStore(hMyCertStore,
                                             X509_ASN_ENCODING,
                                             0,
                                             CERT_CHAIN_FIND_BY_ISSUER,
                                             &FindByIssuerPara,
                                             pChainContext);
        if(pChainContext == NULL)
        {
            printf("Error 0x%x finding cert chain\n", GetLastError());
            break;
        }
        printf("\ncertificate chain found\n");

        // ��������� ��������� �� ������� ���������-�����.
        pCertContext = pChainContext->rgpChain[0]->rgpElement[0]->pCertContext;

        // �������� schannel �������.
        SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
        SchannelCred.cCreds = 1;
        SchannelCred.paCred = &pCertContext;

        Status = g_pSSPI->AcquireCredentialsHandleA(
                            NULL,                   // ��� ��������������
                            UNISP_NAME_A,           // ��� ������
                            SECPKG_CRED_OUTBOUND,   // ����, ������������ �������������
                            NULL,                   // ��������� �� ������������� ������
                            &SchannelCred,          // ������ ������
                            NULL,                   // ��������� �� ������� GetKey()
                            NULL,                   // ��������, ������������ ������� GetKey()
                            &hCreds,                // (out) ���������� �������
                            &tsExpiry);             // (out) ������ ������������ (�������������)
        if(Status != SEC_E_OK)
        {
            printf("**** Error 0x%x returned by AcquireCredentialsHandle\n", Status);
            continue;
        }
        printf("\nnew schannel credential created\n");

        // ����������� ������ ��������.
        g_pSSPI->FreeCredentialsHandle(phCreds);

        *phCreds = hCreds;

        break;
    }
} // ����� GetNewClientCredentials
    


