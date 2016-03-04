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
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ ������ ������������� ������������� ����� �� ���������� �����.

// ��� ���������� ������ ����� ������� ������������ ��� ������� ������
// ������ �������� ����������� ������ ������� EncryptFile.
// ������������� ���������� ���� ���������� � ����� "session_EncryptedKey.bin",
// ������������� ��� ������������ ������ - � "session_SV.bin",
// MAC �������������� ����������� ����� - � "session_MacKey.bin".
// ��� ��� ������������� ��������� �������� ���� �����������, ������������
// � ����� "Sender.pub" (������ ���� ��������� ��� ���������� ������� 
// ExportInFile). 
// ��� ���������� ������ ���� ��������� ���������� �������� �������������
// ���� � ������ "encrypt.bin". ��� ����������� ������������� �����
// ��������� ������ �������������, ������� �������� � ����� "vector.bin".
// ������������� ������������ � ���� "destination.txt".
// ������� ������� �� ���, ����� ���������� "Sender" � "Responder", 
// ������������ � ���������� ExportInFile, EncryptFile � DecryptFile �� ������
// ���������� ���� �������� ������������.
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#define BLOCK_LENGTH 4096

static void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile);
static void CleanUp(void);
static void HandleError(char *s);

static HCRYPTPROV hProv = 0;		// ���������� CSP
static HCRYPTKEY hKey = 0;		// ���������� ��������� ����� 
static HCRYPTKEY hSessionKey = 0;	// ���������� ����������� �����
static HCRYPTKEY hAgreeKey = 0;        // ���������� ����� ������������

static FILE *Encrypt=NULL;		// ������������� ����
static FILE *certf=NULL;		// ����, � ������� �������� ����������
static FILE *publicf=NULL;		// ����, � ������� �������� �������� ����
static FILE *session_SV=NULL;// ���� ��� �������� ����������� ����� (������ �������������)
static FILE *session_EncryptedKey=NULL;// ���� ��� �������� ����������� ����� (������������� ����)
static FILE *session_MacKey=NULL;	// ���� ��� �������� ����������� ����� (�����)
static FILE *vectorf=NULL;		// ���� ��� �������� ������� �������������
static FILE *destination=NULL;		// �������������� ����

int main(void)
{
    BYTE  pbKeyBlob[100];	// ��������� �� �������� BLOB 
    DWORD dwBlobLen = 100;	// ����� ��������� BLOB�
    BYTE *pbKeyBlobSimple = NULL;  // ��������� �� ���������� �������� BLOB 
    DWORD dwBlobLenSimple;	// ����� ����������� ��������� BLOB�
    BYTE pbIV[100];		// ������ ������������� ����������� �����
    DWORD dwIV = 0;		// ����� ������� �������������
    BYTE pbContent[BLOCK_LENGTH];	// ��������� �� ���������� �������������� �����
    DWORD cbContent = 0;	// ����� �����������
    ALG_ID ke_alg = CALG_PRO_EXPORT; // PRO_EXPORT �������� ����� ������������
    CRYPT_SIMPLEBLOB_HEADER tSimpleBlobHeaderStandart;
    BYTE bEncryptionParamSetStandart[] = {0x30, 0x09, 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x1f, 0x01}; //���������� ����� �����

    tSimpleBlobHeaderStandart.BlobHeader.aiKeyAlg = CALG_G28147; 
    tSimpleBlobHeaderStandart.BlobHeader.bType = SIMPLEBLOB;
    tSimpleBlobHeaderStandart.BlobHeader.bVersion = BLOB_VERSION;
    tSimpleBlobHeaderStandart.BlobHeader.reserved = 0;
    tSimpleBlobHeaderStandart.EncryptKeyAlgId = CALG_G28147;
    tSimpleBlobHeaderStandart.Magic = G28147_MAGIC;    

    // �������� �����, � ������� �������� ������������� ����.
    //if(!fopen_s( &Encrypt, "encrypt.bin", "r+b" ))
    if(!(Encrypt = fopen("encrypt.bin", "r+b" )))
	HandleError( "Problem opening the file 'encrypt.bin'\n" );
    printf( "The file 'encrypt.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ ����������� �����.
    if(!(session_SV = fopen("session_SV.bin", "r+b")))
	HandleError( "Problem opening the file 'session_SV.bin'\n" );
    printf( "The file 'session_SV.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ ����������� �����.
    if(!(session_EncryptedKey = fopen("session_EncryptedKey.bin", "r+b")))
	HandleError( "Problem opening the file 'session_EncryptedKey.bin'\n" );
    printf( "The file 'session_EncryptedKey.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ ����������� �����.
    if(!(session_MacKey = fopen("session_MacKey.bin", "r+b")))
	HandleError( "Problem opening the file 'session_MacKey.bin'\n" );
    printf( "The file 'session_MacKey.bin' was opened\n" );

    // �������� �����, � ������� �������� ������ �������������.
    //if(!fopen_s(&vectorf, "vector.bin", "r+b" ))
    if(!(vectorf = fopen("vector.bin", "r+b")))
	HandleError( "Problem opening the file 'vector.bin'\n" );
    printf( "The file 'vector.bin' was opened\n" );

    // �������� �����, � ������� ����� ������� �������������� ����.
    //if(!fopen_s(&destination, "destination.txt", "w+b" ))
    if(!(destination = fopen("destination.txt", "w+b" )))
	HandleError( "Problem opening the file 'destination.txt'\n" );
    printf( "The file 'destination.txt' was opened\n" );

    // ������  ������� ������������� �� �����.
    dwIV = (DWORD)fread(pbIV, 1, 100, vectorf);
    if(!dwIV)
	HandleError( "The IV can not be reading from the 'vector.bin'\n" );
    printf( "The IV was read from the 'vector.bin'\n" );

    // ��������� ����������� ���������� ���������� � ������ "Responder", 
    // ������������ � ������ ����������. 
    if(!CryptAcquireContext(
	&hProv, 
	"Responder",
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	HandleError("Error during CryptAcquireContext");
    }
    printf("The key container \"Responder\" has been acquired. \n");

    pbKeyBlobSimple = malloc((sizeof(CRYPT_SIMPLEBLOB_HEADER)+SEANCE_VECTOR_LEN+G28147_KEYLEN+EXPORT_IMIT_SIZE) + sizeof(bEncryptionParamSetStandart));
    if(!pbKeyBlobSimple) 
	HandleError("Out of memory. \n");

    memcpy(&((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->tSimpleBlobHeader, &tSimpleBlobHeaderStandart, sizeof(CRYPT_SIMPLEBLOB_HEADER)); //�������� ���������� ����� � �����

    // ������  ���c������� ����� �� �����.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bSV, 1, SEANCE_VECTOR_LEN, session_SV);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_SV.bin'\n" );
    printf( "The session key was read from the 'session_SV.bin'\n" );

    // ������  ���c������� ����� �� �����.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptedKey, 1, G28147_KEYLEN, session_EncryptedKey);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_EncryptedKey.bin'\n" );
    printf( "The session key was read from the 'session_EncryptedKey.bin'\n" );

    // ������  ���c������� ����� �� �����.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bMacKey, 1, EXPORT_IMIT_SIZE, session_MacKey);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_MacKey.bin'\n" );
    printf( "The session key was read from the 'session_MacKey.bin'\n" );

    memcpy(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptionParamSet, bEncryptionParamSetStandart, 11); //�������� ���������� ASN ���������

    LoadPublicKey(hProv, pbKeyBlob, &dwBlobLen, "Sender.cer", "Sender.pub");

    // ��������� ����������� ��������� ����� ����������.
    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {
	printf("The private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    // ��������� ����� ������������ �������� ��������� ����� ����������� 
    // �� �������� ����� ����������.
    if(CryptImportKey(
	hProv, 
	pbKeyBlob, 
	dwBlobLen, 
	hKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    // ������������ PRO_EXPORT ��������� ����� ������������
    if(CryptSetKeyParam(
        hAgreeKey,
        KP_ALGID,
        (LPBYTE)&ke_alg,
        0))
    {
	printf("PRO_EXPORT agree key algorithm has been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }

    // ��������� ����������� ����� �������� �������������� ����������� ����� 
    // �� ����� Agree.
    if(CryptImportKey(
	hProv, 
	pbKeyBlobSimple, 
	71, 
	hAgreeKey, 
	0, 
	&hSessionKey))
    {
	printf("The session key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey session key.");
    }

    // ��������� ������� ������������� - ��� ���� ������ 8 ���� 
    // ������������ �����������.
    if(!CryptSetKeyParam(
	hSessionKey, 
	KP_IV, 	
	pbIV, 
	0))
    {
	HandleError("Error during CryptSetKeyParam.");
    }
    printf( "CryptSetKeyParam succeeded. \n");

    // ������  �������������� ����� ������� �� 4 ��. ������������� ������������
    // ����� � ������ ��� � ���� "destination.txt".
    do 
    {
	cbContent = (DWORD)fread(pbContent, 1, BLOCK_LENGTH, Encrypt);
	if(cbContent)
	{
	    BOOL bFinal = feof(Encrypt);
	    // ������������� ������������ ����� �� ���������� �����.
	    if(CryptDecrypt(
		hSessionKey,
		0,
		bFinal,
		0,
		pbContent,
		&cbContent))
	    {
		printf( "Decryption succeeded. \n");
		// ������ ��������������� ����� � ����.
		if(fwrite(
		    pbContent,
		    1,
		    cbContent,
		    destination))
		{
		    printf( "The decrypted content was written to the 'destination.txt'\n" );
		}
		else
		{
		    HandleError( "The decrypted content can not be written to the 'destination.txt'\n" );
		}  
	    }
	    else
	    {
		HandleError("Decryption failed.");
	    }
	}
	else
	{
	    HandleError( "Problem reading the file 'encrypt.bin'\n" );
	}
    }
    while(!feof(Encrypt));

    CleanUp();

    printf("The program ran to completion without error. \n");
    
    free(pbKeyBlobSimple);
    return 0;
}// ����� main

// �������� PUBLICKEYBLOB �� �����������, ���� �� ����, (���������������) ���
// �� �����, ����������� � ����.
void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile)
{
    // �������� �����, � ������� ���������� �������� ���� ����������.
    //if(fopen_s(&certf, szCertFile, "r+b" ))
    if((certf = fopen(szCertFile, "rb"))) {
	DWORD cbCert = 2000;
	BYTE  pbCert[2000];
	PCCERT_CONTEXT pCertContext = NULL;
	HCRYPTKEY hPubKey;
	printf( "The file '%s' was opened\n", szCertFile );

	cbCert = (DWORD)fread(pbCert, 1, cbCert, certf);
	if(!cbCert)
	    HandleError( "Failed to read certificate\n" );
	printf( "Certificate was read from the '%s'\n", szCertFile );

	pCertContext = CertCreateCertificateContext (
	    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pbCert, cbCert);
	if (!pCertContext) {
	    HandleError( "CertCreateCertificateContext" );
	}

	// ����������� �������� ����
	if (CryptImportPublicKeyInfoEx(
	    hProv, 
	    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	    &(pCertContext->pCertInfo->SubjectPublicKeyInfo),
	    0,
	    0,
	    NULL,
	    &hPubKey))
	{
	    printf("Public key imported from cert file\n");
	} else {
	    CertFreeCertificateContext(pCertContext);
	    HandleError( "CryptImportPublicKeyInfoEx" );
	}
	CertFreeCertificateContext(pCertContext);

	// ������������ ��� � BLOB
	if (CryptExportKey(
	    hPubKey,
	    0,
	    PUBLICKEYBLOB,
	    0,
	    pbBlob,
	    pcbBlob))
	{
	    printf("Public key exported to blob\n");
	}
	else
	{
	    HandleError( "CryptExportKey" );
	}
    } else {
	// �������� �����, � ������� ���������� �������� ���� ����������.
	//if(!fopen_s(&publicf, szKeyFile, "r+b" ))
	if(!(publicf = fopen(szKeyFile, "rb")))
	    HandleError( "Problem opening the public key blob file\n" );
	printf( "The file '%s' was opened\n", szKeyFile );

	*pcbBlob = (DWORD)fread(pbBlob, 1, *pcbBlob, publicf);
	if(!*pcbBlob)
	    HandleError( "Failed to read key blob file\n" );
	printf( "Key blob was read from the '%s'\n", szKeyFile );
    }
}

void CleanUp(void)
{
    if(Encrypt)
        fclose (Encrypt);
    if(session_SV)
	fclose (session_SV);
    if(session_EncryptedKey)
	fclose (session_EncryptedKey);
    if(session_MacKey)
	fclose (session_MacKey);
    if(vectorf)    
        fclose (vectorf);
    if(destination)    
        fclose (destination);
    if(certf)
	fclose (certf);
    if(publicf)
	fclose (publicf);

    // ����������� ����������� ��������� �����.
    if(hKey)
	CryptDestroyKey(hKey);

    // ����������� ����������� ����������� �����.
    if(hSessionKey)
	CryptDestroyKey(hSessionKey);

    // ����������� ����������� ����� ������������.
    if(hAgreeKey)
	CryptDestroyKey(hAgreeKey);

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





