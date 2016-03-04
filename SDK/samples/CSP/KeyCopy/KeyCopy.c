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

#include <stdio.h>
#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// � ������ ������� ����������� ����������� ��������� ����� AT_KEYEXCHANGE.
// ����������� ����� AT_SIGNATURE ������������ ����������. ���������� �����
// ���������� ����������� ��������������� ���� ������ ������������.
// ��� ���������� ������ ���� ��������� ���������� ����� ������ ����
// ������� ��� exportable. ����������� ����� ������������ �� ���������� 
// � ������ "Source" (�������, ��������������, ������ ���� ������) � 
// ����������� ��������� � ������ "Destination" (�������, ���� �� �������, 
// �� ������ ������ ��������� ������ ���� ������).
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProvResponder = 0;     // ���������� CSP ���������� 
static HCRYPTPROV hProvSender = 0;	   // ���������� CSP �����������
static HCRYPTKEY hSenderKey = 0;	   // ���������� ���������� ����� AT_EXCHANGE
static HCRYPTKEY hResponderKey = 0;       // ���������� ����������� ����� AT_EXCHANGE
static HCRYPTKEY hSenderEphemKey = 0;     // ���������� ���������� ����� ��� �����������
static HCRYPTKEY hResponderEphemKey = 0;  // ���������� ���������� ����� ��� ����������
static HCRYPTKEY hSenderAgreeKey = 0;	   // ���������� ����� ������������ �����������
static HCRYPTKEY hResponderAgreeKey = 0;  // ���������� ����� ������������ ����������
static BYTE *pbKeyBlob = NULL;		   // ��������� �� �������� BLOB

int  main (void)
{
    //--------------------------------------------------------------------
    //  ���������� � ������������� ����������. 

    DWORD dwProvType = PROV_GOST_2001_DH;   // ��� ���������� �� ���������
    DWORD data_len = 0;			    // ����� ������
    BYTE *oid = NULL;			    // ����� ��� �������� ����������
    DWORD dwBlobLen = 0;		    // ����� ��������� BLOB�
    DWORD cAlg = CALG_PRO_EXPORT;


    //--------------------------------------------------------------------
    // ��������� ����������� ����������, � ������� �������������� ����������� 
    // ����� (����������). 

    if(CryptAcquireContext(
	&hProvResponder, 
#ifdef _WIN32
	"Destination", 
#else
	"\\\\.\\HDIMAGE\\Destination",
#endif
	NULL, 
	dwProvType, 
	CRYPT_NEWKEYSET)) 
    {
	printf("The responder key container has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ����������, �� �������� �������������� ����������� 
    // ����� (�����������). 


    if(CryptAcquireContext(
	&hProvSender, 
	"Source", 
	NULL, 
	dwProvType, 
	0)) 
    {
	printf("The sender key container has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // ����������� ���������� ����������-����������� � ���������-����������
    // ��� ��������� PP_HASHOID.

    //--------------------------------------------------------------------
    // ����������� ������� ������ � ������������� ������.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_HASHOID, 
	NULL, 
	&data_len, 
	0))
    {
	printf("Size of the buffer determined. \n");
    }
    else
    {
	HandleError("Error computing buffer length.");
    }

    oid = (BYTE *)malloc( data_len );
    if( !oid )
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // ����������� ���������� ����������-����������� � �����.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_HASHOID, 
	oid, 
	&data_len, 
	0))
    {
	printf("Parameters have been written to the buffer. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������, ����������� � ������, �����������.

    if(CryptSetProvParam(
	hProvResponder, 
	PP_HASHOID, 
	oid, 
	0 ))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    //  ������������ ������, ������������ �������.
    free( oid );

    // ��������� ����� ������ ��� ���������� �������������.
    data_len = 0;

    //--------------------------------------------------------------------
    // ����������� ���������� ����������-����������� � ���������-����������
    // ��� ��������� PP_CIPHEROID.

    //--------------------------------------------------------------------
    // ����������� ������� ������ � ������������� ������.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_CIPHEROID, 
	NULL, 
	&data_len, 
	0))
    {
	printf("Size of the buffer determined. \n");
    }
    else
    {
	HandleError("Error computing buffer length.");
    }

    oid = (BYTE *)malloc( data_len );
    if( !oid )
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // ����������� ���������� ����������-����������� � �����.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_CIPHEROID, 
	oid, 
	&data_len, 
	0))
    {
	printf("Parameters have been written to the buffer. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������, ����������� � ������, �����������.

    if(CryptSetProvParam(
	hProvResponder, 
	PP_CIPHEROID, 
	oid, 
	0 ))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptSetProvParam.");
    }
    free( oid );

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� �����, ������� ����� ����������.
    if(CryptGetUserKey( 
	hProvSender, 
	AT_KEYEXCHANGE, 
	&hSenderKey )) 
    {
	printf("The sender private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }		

    //--------------------------------------------------------------------
    // ��������� ���������� ����� � ����������-�����������.

    if(CryptGenKey(
	hProvSender, 
	CALG_DH_EL_EPHEM, 
	CRYPT_EXPORTABLE, 
	&hSenderEphemKey)) 
    {   
	printf("Ephemeral sender key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ���������� ����� � ����������-����������.

    if(CryptGenKey(
	hProvResponder, 
	CALG_DH_EL_EPHEM, 
	CRYPT_EXPORTABLE | CRYPT_PREGEN,
	&hResponderEphemKey))
    {   
	printf("Ephemeral responder key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }


    //--------------------------------------------------------------------
    // ������������ ��������� KP_DHOID, ��������������� � ��������� 
    // �����-�������.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� � ������������� ������.

    if(CryptGetKeyParam( 
	hSenderEphemKey, 
	KP_DHOID, 
	NULL, 
	&dwBlobLen, 
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob)
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // ����������� ���������� ����� � BLOB.

    if(CryptGetKeyParam( 
	hSenderEphemKey, 
	KP_DHOID, 
	pbKeyBlob, 
	&dwBlobLen, 
	0))
    {
	printf("Parameters have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // ����������� ���������� � hResponderEphemKey.

    if(CryptSetKeyParam(
	hResponderEphemKey, 
	KP_DHOID, 
	pbKeyBlob, 
	0))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    //  ������������ ������, ������������ BLOB��.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;


    //--------------------------------------------------------------------
    // ��������� �����

    if(CryptSetKeyParam(
	hResponderEphemKey, 
	KP_X, 
	NULL, 
	0))
    {   
	printf("X parameter is generated. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������ � ���������� - ����������.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ���������� ����� � ������������� ������.

    if(CryptExportKey(
	hSenderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the sender ephemeral key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // ��������������� ���������� ����� ����������� � BLOB.
    if(CryptExportKey(
	hSenderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������ ���������� �������� ���������� ����� ����������� 
    // �� ��������� ����� ����������.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlob, 
	dwBlobLen, 
	hResponderEphemKey, 
	0, 
	&hResponderAgreeKey))
    {
	printf("The sender ephemeral key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey ephemeral key.");
    }

    //--------------------------------------------------------------------
    //  ������������ ������, ������������ BLOB��.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // ��������� ����� ������������ � ���������� - �����������.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ���������� ����� � ������������� ������.

    if(CryptExportKey(
	hResponderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the responder ephemeral key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // ��������������� ���������� ����� ���������� � BLOB.
    if(CryptExportKey(
	hResponderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������ ���������� �������� ���������� ����� ���������� 
    // �� ��������� ����� �����������.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlob, 
	dwBlobLen, 
	hSenderEphemKey, 
	0, 
	&hSenderAgreeKey))
    {
	printf("The responder ephemeral key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey ephemeral key.");
    }

    //--------------------------------------------------------------------
    //  ������������ ������, ������������ BLOB��.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // ��������������� ����� ������������ � ���������� � ���������� - �����������.

    if(CryptSetKeyParam(
	hSenderAgreeKey,
	KP_ALGID, 
	(BYTE*)&cAlg,
	0 ))
    {
	printf("The sender agree key has been converted. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }

    //--------------------------------------------------------------------
    // ��������������� ����� ������������ � ���������� � ���������� - ����������.

    if(CryptSetKeyParam(
	hResponderAgreeKey,
	KP_ALGID, 
	(BYTE*)&cAlg,
	0 ))
    {
	printf("The responder agree key has been converted. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }


    //--------------------------------------------------------------------
    // ������� ����������� ����������������� �����  �� ����������.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ����� � ������������� ������.

    if(CryptExportKey(
	hSenderKey,
	hSenderAgreeKey, 
	PRIVATEKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the sender key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // ������� ����������� ����������������� �����  �� ����������.
    if(CryptExportKey(
	hSenderKey,
	hSenderAgreeKey, 
	PRIVATEKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ����� �������� ����������������� ����� �� �����
    // ������������.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlob, 
	dwBlobLen, 
	hResponderAgreeKey, 
	0, 
	&hResponderKey))
    {
	printf("The private key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey private key.");
    }

    //--------------------------------------------------------------------
    //  ������������ ������, ������������ BLOB��.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // ����������� ���������� ����� hSenderKey � ��������� ����� hResponderKey
    // ��� KP_CERTIFICATE.

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� � ������������� ������.

    if(CryptGetKeyParam( 
	hSenderKey, 
	KP_CERTIFICATE, 
	NULL, 
	&dwBlobLen, 
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);

    if(pbKeyBlob)
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }

    //--------------------------------------------------------------------
    // ����������� ���������� ����� � BLOB.

    if(CryptGetKeyParam( 
	hSenderKey, 
	KP_CERTIFICATE, 
	pbKeyBlob, 
	&dwBlobLen, 
	0))
    {
	printf("Parameters have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // ����������� ���������� � hResponderKey.

    if(CryptSetKeyParam(
	hResponderKey, 
	KP_CERTIFICATE, 
	pbKeyBlob, 
	0))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    CleanUp();
    return 0;
}

void CleanUp(void)
{
    free(pbKeyBlob);

    // ������������ ����������� ����������.
    if(hProvResponder) 
	CryptReleaseContext(hProvResponder, 0);
    if(hProvSender) 
	CryptReleaseContext(hProvSender, 0);

    // ����������� ������������ ������.
    if(hSenderKey)
	CryptDestroyKey(hSenderKey);

    if(hResponderKey)
	CryptDestroyKey(hResponderKey);

    if(hSenderEphemKey)
	CryptDestroyKey(hSenderEphemKey);

    if(hResponderEphemKey)
	CryptDestroyKey(hResponderEphemKey);

    if(hSenderAgreeKey)
	CryptDestroyKey(hSenderAgreeKey);

    if(hResponderAgreeKey)
	CryptDestroyKey(hResponderAgreeKey);
}

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//--------------------------------------------------------------------
//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp();
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE

