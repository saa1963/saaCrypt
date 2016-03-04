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
// ������ ���� ��� ������ � ������ SILENT, ��� ����.
// � ������ ������� �������������� ������� ������.
// ��� ���������� ������ ���� ��������� ���������� ������� ��������� � 
// ������ "Source" � � ������� "1234". ��� ���� � ���������� ������ 
// ������������ ��� ����� (���� ������ � ���� �������).
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// ���������� CSP
static HCRYPTKEY hKey = 0;		// ���������� �����
static HCRYPTHASH hHash = 0;
static BYTE *pbHash = NULL;
static BYTE *pbKeyBlob = NULL;      
static BYTE *pbSignature = NULL;
#ifdef _WIN32        
#define CONTAINER "\\\\.\\Registry\\Source" 
#else
#define CONTAINER "\\\\.\\HDIMAGE\\test" 
#endif

int main(void)
{
    BYTE *pbBuffer = (BYTE *)"The data that is to be hashed and signed.";
    DWORD dwBufferLen = (DWORD)(strlen((char *)pbBuffer)+1);
    DWORD cbHash;
    DWORD dwSigLen;
    DWORD dwBlobLen;
    FILE *signature;

    // ��������� ����������� ����������.
    if(CryptAcquireContext(
	&hProv, 
        CONTAINER,
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_SILENT)) 
    {
	printf("CryptAcquireContext succeeded.\n");
    }
    else
    {
	if(GetLastError() == 0x80090016)
	{
	    printf(" The key container with name \"Source\" does not exist.\n");
	    printf(" Create a container and generate keys \n");
	    exit(0);
	}
	else
	{
	    HandleError("A general error occurred in CryptAcquireContext.");
	}
    }

    //--------------------------------------------------------------------
    // ��������� ���������� � ������������ � �������.

    if(CryptSetProvParam(
	hProv,
	PP_KEYEXCHANGE_PIN,
	(BYTE*)"1234",
	0))
    {
	printf("CryptSetProvParam succeeded.\n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������.

    if(CryptGetUserKey(
	hProv,
	AT_KEYEXCHANGE,
	&hKey))
    {
	printf("CryptGetUserKey succeeded.\n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey.");
    }

    //--------------------------------------------------------------------
    // ������ ��������� �����. ����� �������� ���� �������������� � 
    // PUBLICKEYBLOB ��� ����, ����� ���������� ������������ ���� ��� 
    // ��������� �������. ���� BLOB ����� ���� ������� � ���� � �������
    // ������� ������������.

    if(CryptExportKey(   
	hKey,    
	0,    
	PUBLICKEYBLOB,
	0,    
	NULL, 
	&dwBlobLen)) 
    {
	printf("Size of the BLOB for the public key determined. \n");
    }
    else
    {
	if(GetLastError() == 0x8010006B) /* SCARD_W_WRONG_CHV */
	{
	    printf(" The password of key container is differ from \"1234\".\n");
	    exit(0);
	}
	else
	{
	    HandleError("Error computing BLOB length.");
	}
    }

    // ������������� ������ ��� pbKeyBlob.
    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // ��� ������� � �������� BLOB.
    if(CryptExportKey(   
	hKey, 
	0,    
	PUBLICKEYBLOB,    
	0,    
	pbKeyBlob,    
	&dwBlobLen))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }
    //--------------------------------------------------------------------
    // �������� ������� ������� �����������.

    if(CryptCreateHash(
	hProv, 
	CALG_GR3411, 
	0, 
	0, 
	&hHash)) 
    {
	printf("Hash object created. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash.");
    }

    //--------------------------------------------------------------------
    // �������� ��������� HP_OID ������� ������� �����������.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� � ������������� ������.

    if(CryptGetHashParam(hHash,
	HP_OID,
	NULL,
	&cbHash,
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbHash = (BYTE*)malloc(cbHash);
    if(!pbHash) 
	HandleError("Out of memory. \n");

    // ����������� ��������� HP_OID � pbHash.

    if(CryptGetHashParam(hHash,
	HP_OID,
	pbHash,
	&cbHash,
	0))
    {
	printf("Parameters have been written to the pbHash. \n");
    }
    else
    {
	HandleError("Error during CryptGetHashParam.");
    }

    //--------------------------------------------------------------------
    // ���������� ������������������ ���� ������.

    if(CryptHashData(
	hHash, 
	pbBuffer, 
	dwBufferLen, 
	0)) 
    {
	printf("The data buffer has been hashed.\n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }
    //--------------------------------------------------------------------
    // ����������� ������� ������� � ������������� ������.

    dwSigLen = 0;
    if(CryptSignHash(
	hHash, 
	AT_SIGNATURE, 
	NULL, 
	0, 
	NULL, 
	&dwSigLen)) 
    {
	printf("Signature length %d found.\n",dwSigLen);
    }
    else
    {
	HandleError("Error during CryptSignHash.");
    }
    //--------------------------------------------------------------------
    // ������������� ������ ��� ����� �������.

    pbSignature = (BYTE *)malloc(dwSigLen);
    if(!pbSignature)
	HandleError("Out of memory.");

    // ������� ������� ������� �����������.
    if(CryptSignHash(
	hHash, 
	AT_SIGNATURE, 
	NULL, 
	0, 
	pbSignature, 
	&dwSigLen)) 
    {
	printf("pbSignature is the hash signature.\n");
    }
    else
    {
	HandleError("Error during CryptSignHash.");
    }

    //if(!fopen_s(&signature, "signature.txt", "w+b" ))
    if(!(signature = fopen("signature.txt", "w+b")))
	HandleError( "Problem opening the file\n" );
    fwrite (pbSignature, 1, dwSigLen, signature);
    fclose(signature);

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
} 

void CleanUp(void)
{
    if(pbHash)
	free(pbHash);
    if(pbKeyBlob)
	free(pbKeyBlob);
    if(pbSignature)
	free(pbSignature);

    // ����������� ������� ������� �����������.
    if(hHash) 
	CryptDestroyHash(hHash);

    printf("The hash object has been destroyed.\n");
    printf("The signature is created.\n\n");

    // ����������� ����������� ����� ������������.

    if(hKey)
	CryptDestroyKey(hKey);

    // ������������ ����������� ����������.

    if(hProv) 
	CryptReleaseContext(hProv, 0);
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

