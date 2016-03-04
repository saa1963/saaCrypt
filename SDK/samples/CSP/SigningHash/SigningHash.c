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
// � ���� ������� ����������� �������� ������� ������� ������� �����������
// � �������� ���� ����������-�������� �������.
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#ifdef _WIN32
#define CONTAINER "test"
#else
#define CONTAINER "\\\\.\\HDIMAGE\\test"
#endif
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;
static HCRYPTHASH hHash = 0;
static HCRYPTKEY hKey = 0;
static HCRYPTKEY hPubKey = 0;
static BYTE *pbHash = NULL;
static BYTE *pbSignature = NULL;
static BYTE *pbKeyBlob = NULL; 

int main(void)
{
    //-------------------------------------------------------------
    // ���������� � ������������� ����������.

    
    BYTE *pbBuffer= (BYTE *)"The data that is to be hashed and signed.";
    DWORD dwBufferLen = (DWORD)(strlen((char *)pbBuffer)+1);       
    DWORD dwSigLen;
    DWORD dwBlobLen;
    DWORD cbHash;
    FILE *signature;    

    // ��������� ����������� ��������� ������������������ ����������.
    if(CryptAcquireContext(
	&hProv, 
	CONTAINER, 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("CSP context acquired.\n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }
    //--------------------------------------------------------------------
    // ��������� ��������� ����� �������. ���� �������� ���� ����� 
    // �������������� ����������� ���� ��� �������� �������.
    // � ������, ����� ���������� ����� ������ � ��������� �����
    // ����������� � ������� �����������, ���� ��� �� �����.

    if(CryptGetUserKey(   
	hProv,    
	AT_SIGNATURE,    
	&hKey)) 
    {
	printf("The signature key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey for signkey.");
    }
    //--------------------------------------------------------------------
    // ������ ��������� �����. ����� �������� ���� �������������� � 
    // PUBLICKEYBOLB ��� ����, ����� ���������� ������������ ���� ��� 
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
	HandleError("Error computing BLOB length.");
    }
    //--------------------------------------------------------------------
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
	HandleError( "Problem opening the file signature.txt\n" );

    fwrite(pbSignature, 1, dwSigLen, signature);

    // ����������� ������� ������� �����������.
    if(hHash) 
	CryptDestroyHash(hHash);

    printf("The hash object has been destroyed.\n");
    printf("The signing phase of this program is completed.\n\n");

    //--------------------------------------------------------------------
    // �� ������ ����� ��������� ����������� �������.
    // ���� ����� �������� �������������� � ������, ����� ��������� 
    // ������������ ���������� ���� � �� �� ���������. ���, �������, 
    // � ����� PUBLICKEYBLOB ����� ���� ��������� �� �����, e-mail ��������� 
    // ��� �� ������� ���������.

    // ����� ����������� ������������ ����� pbBuffer, pbSignature, 
    // szDescription, pbKeyBlob � �� �����.

    // ���������� ������ pbBuffer ������������ �� ���� ��������� 
    // ����������� ����� ������.

    // ��������� szDescription �� �����, ����������� ������, �������������. 
    // ��� ��� �� ����� ����� ��������, ������� ��� ����� �������
    // ������� CryptSignHash.

    //--------------------------------------------------------------------
    // ��������� �������� ����� ������������, ������� ������ �������� �������, 
    // � �������������� ��� � CSP � ������� ������� CryptImportKey. ��� 
    // ���������� ���������� ��������� ����� � hPubKey.

    if(CryptImportKey(
	hProv,
	pbKeyBlob,
	dwBlobLen,
	0,
	0,
	&hPubKey))
    {
	printf("The key has been imported.\n");
    }
    else
    {
	HandleError("Public key import failed.");
    }
    //--------------------------------------------------------------------
    // �������� ������ ������� ������� �����������.

    if(CryptCreateHash(
	hProv, 
	CALG_GR3411, 
	0, 
	0, 
	&hHash)) 
    {
	printf("The hash object has been recreated. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash.");
    }

    //--------------------------------------------------------------------
    // ��������� ��������� HP_OID.

    // �� ��������� ��������� �������� �� ������ ���������� 1.2.643.2.2.30.1.
    // ��� ��������� ��������� HP_OID ��������� ����� ������� ��������, 
    // ���� ��������� ����������� �� ����� �������� ����������� 
    // �� ���������.

    if(CryptSetHashParam(
	hHash, 
	HP_OID, 
	pbHash, 
	0))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	HandleError("Error during SetHashParam.");
    }

    //--------------------------------------------------------------------
    // ���������� ������������������ ���� ������.

    if(CryptHashData(
	hHash, 
	pbBuffer, 
	dwBufferLen, 
	0)) 
    {
	printf("The new has been created.\n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }
    //--------------------------------------------------------------------
    // �������� �������� �������.

    if(CryptVerifySignature(
	hHash, 
	pbSignature, 
	dwSigLen, 
	hPubKey,
	NULL, 
	0)) 
    {
	printf("The signature has been verified.\n");
    }
    else
    {
	printf("Signature not validated!\n");
    }

    CleanUp();
    return 0;
}

void CleanUp(void)
{
    if(pbSignature)
	free(pbSignature);
    if(pbHash)
	free(pbHash);
    if(pbKeyBlob)
	free(pbKeyBlob);

    // ����������� ������� ������� �����������.
    if(hHash) 
	CryptDestroyHash(hHash);

    if(hKey)
	CryptDestroyKey(hKey);

    if(hPubKey)
	CryptDestroyKey(hPubKey);

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
