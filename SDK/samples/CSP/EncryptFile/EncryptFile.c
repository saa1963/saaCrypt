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
// � ������ ������� ������������ ������������ ����� �� �������� �����
// ����������.

// ��� ���������� ������ ������� ������� �������������� ���������� 
// �������� �������� ���� ���������� � ���� "Responder.pub" ��� ������
// ������� ExportInFile.
// ����� �������������� ���������� ������� ����, ���������� ������������, 
// � ������ "source.txt".
// ������� ������� �� ���, ����� ���������� "Sender" � "Responder", 
// ������������ � ���������� ExportInFile, EncryptFile � DecryptFile �� ������
// ���������� ���� �������� ������������.
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#define BLOCK_LENGTH 4096

static void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile);
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// ���������� CSP 
static HCRYPTKEY hKey = 0;		// ���������� ��������� ����� 
static HCRYPTKEY hSessionKey = 0;	// ���������� ����������� �����
static HCRYPTKEY hAgreeKey = 0;        // ���������� ����� ������������

static FILE *certf=NULL;		// ����, � ������� �������� ����������
static FILE *publicf=NULL;		// ����, � ������� �������� �������� ����
static FILE *source=NULL;		// �������� ����
static FILE *Encrypt=NULL;		// ������������� ����
static FILE *session_SV=NULL;// ���� ��� �������� ����������� ����� (������ �������������)
static FILE *session_EncryptedKey=NULL;// ���� ��� �������� ����������� ����� (������������� ����)
static FILE *session_MacKey=NULL;	// ���� ��� �������� ����������� ����� (�����)
static FILE *vectorf=NULL;		// ���� ��� �������� ������� �������������

static BYTE *pbKeyBlobSimple = NULL;   // ��������� �� ���������� �������� BLOB 
static BYTE *pbIV = NULL;		// ������ ������������� ����������� �����

int main(void)
{
    BYTE  pbKeyBlob[100];	// ��������� �� �������� BLOB 
    DWORD dwBlobLen = 100;	// ����� ��������� BLOB�
    DWORD dwBlobLenSimple;      // ����� ����������� ��������� BLOB�

    BYTE pbContent[BLOCK_LENGTH];	// ��������� �� ���������� ��������� �����
    DWORD cbContent = 0;	// ����� �����������
    DWORD dwIV = 0;		// ����� ������� �������������
    DWORD bufLen = sizeof(pbContent);   // ����� ������
    ALG_ID ke_alg = CALG_PRO_EXPORT; // PRO_EXPORT �������� ����� ������������

    // �������� �����, ������� ����� ����������.
    //if(!fopen_s(&source, "source.txt", "r+b" ))
    if(!(source = fopen("source.txt", "rb")))
	HandleError( "Problem opening the file 'source.txt'\n" );
    printf( "The file 'source.txt' was opened\n" );

    // �������� �����, � ������� ����� ������������ ������ �������������� �����.
    //if(!fopen_s(&Encrypt, "encrypt.bin", "wb"))
    if(!(Encrypt = fopen("encrypt.bin", "wb")))
	HandleError( "Problem opening the file 'encrypt.bin'\n" );
    printf( "The file 'encrypt.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ �������������.
    //if(!fopen_s(&session_SV, "session_SV.bin", "wb"))
    if(!(session_SV = fopen("session_SV.bin", "wb")))
	HandleError( "Problem opening the file 'session_SV.bin'\n" );
    printf( "The file 'session_SV.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ ����������� �����.
    //if(!fopen_s(&session_EncryptedKey, "session_EncryptedKey.bin", "wb"))
    if(!(session_EncryptedKey = fopen("session_EncryptedKey.bin", "wb")))
	HandleError( "Problem opening the file 'session_EncryptedKey.bin'\n" );
    printf( "The file 'session_EncryptedKey.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ MAC ����������� �����.
    //if(!fopen_s(&session_MacKey, "session_MacKey.bin", "wb" ))
    if(!(session_MacKey = fopen("session_MacKey.bin", "wb")))
	HandleError( "Problem opening the file 'session_MacKey.bin'\n" );
    printf( "The file 'session_MacKey.bin' was opened\n" );

    // �������� �����, � ������� ������������ ������ ������� �������������.
    //if(!fopen_s(&vectorf, "vector.bin", "wb" ))
    if(!(vectorf = fopen("vector.bin", "wb")))
	HandleError( "Problem opening the file 'vector.bin'\n" );
    printf( "The file 'vector.bin' was opened\n" );

    // ��������� ����������� ���������� ���������� � ������ "Sender", 
    // ������������ � ������ ����������. 
    if(CryptAcquireContext(
	&hProv, 
	"Sender", 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"Sender\" has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    LoadPublicKey(hProv, pbKeyBlob, &dwBlobLen, "Responder.cer", "Responder.pub");

    // ��������� ����������� ��������� ����� �����������.
    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {printf("The private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    // ��������� ����� ������������ �������� ��������� ����� ����������
    // �� �������� ����� �����������.

    if(CryptImportKey(
	hProv, 
	pbKeyBlob, 
	dwBlobLen, 
	hKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
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

    // ��������� ����������� �����.
    if(CryptGenKey(     
	hProv,      
	CALG_G28147,      
	CRYPT_EXPORTABLE, 
	&hSessionKey))
    {   
	printf("Original session key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // ������������ ����������� �����.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ����������� ����� � ������������� ������.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey, 
	SIMPLEBLOB, 
	0, 
	NULL,
	&dwBlobLenSimple)) 
    {
	printf("Size of the BLOB for the sender session key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobSimple = (BYTE*)malloc(dwBlobLenSimple);

    if(!pbKeyBlobSimple) 
	HandleError("Out of memory. \n");

    // ������������ ����������� ����� �� ����� Agree.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey,
	SIMPLEBLOB, 
	0, 
	pbKeyBlobSimple, 
	&dwBlobLenSimple))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    // ����������� ������� ������� ������������� ����������� �����. 
    if(CryptGetKeyParam(
	hSessionKey, 
	KP_IV, 
	NULL, 
	&dwIV, 
	0))
    {
	printf("Size of the IV for the session key determined. \n");
    }
    else
    {
	HandleError("Error computing IV length.");
    }

    pbIV = (BYTE*)malloc(dwIV);
    if (!pbIV)
	HandleError("Out of memory. \n");

    // ����������� ������� ������������� ����������� �����.
    if(CryptGetKeyParam(
	hSessionKey, 
	KP_IV, 
	pbIV, 
	&dwIV, 
	0))
    {
	printf( "CryptGetKeyParam succeeded. \n");
    }
    else
    {
	HandleError("Error during CryptGetKeyParam.");
    }


    //--------------------------------------------------------------------
    // ������ ������� ������������� � ����.

    if(fwrite(
	pbIV,
	1,
	dwIV,
	vectorf))
    {
	printf( "The IV was written to the 'vector.bin'\n" );
    }
    else
    {
	HandleError( "The IV can not be written to the 'vector.bin'\n" );
    }  

    //--------------------------------------------------------------------
    // ������ �������������� ����������� ����� � ����.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bSV,
	1,
	SEANCE_VECTOR_LEN,
	session_SV))
    {
	printf( "The session key was written to the 'session_SV.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session_SV.bin'\n" );
    }

    //--------------------------------------------------------------------
    // ������ �������������� ����������� ����� � ����.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptedKey,
	1,
	G28147_KEYLEN,
	session_EncryptedKey))
    {
	printf( "The session key was written to the 'session.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session.bin'\n" );
    }

    //--------------------------------------------------------------------
    // ������ �������������� ����������� ����� � ����.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bMacKey,
	1,
	EXPORT_IMIT_SIZE,
	session_MacKey))
    {
	printf( "The session key was written to the 'session.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session.bin'\n" );
    }

    //--------------------------------------------------------------------
    // ������  �����, ������� ����� ���������� ������� �� 4 ��. ������������ 
    // ������������ ����� � ������ ��� � ���� "encrypt.bin".
    //--------------------------------------------------------------------

    do 
    {
	cbContent = (DWORD)fread(pbContent, 1, BLOCK_LENGTH, source);
	if(cbContent)
	{
	    BOOL bFinal = feof(source);
	    // ������������ ������������ ����� �� ���������� �����.
	    if(CryptEncrypt(
		hSessionKey,
		0,
		bFinal,
		0,
		pbContent,
		&cbContent,
		bufLen))
	    {
		printf( "Encryption succeeded. \n");
		// ������ �������������� ����� � ����.
		if(fwrite(
		    pbContent,
		    1,
		    cbContent,
		    Encrypt))
		{
		    printf( "The encrypted content was written to the 'encrypt.bin'\n" );
		}
		else
		{
		    HandleError( "The encrypted content can not be written to the 'encrypt.bin'\n" );
		}  
	    }
	    else
	    {
		HandleError("Encryption failed.");
	    }
	}
	else
	{
	    HandleError( "Problem reading the file 'source.txt'\n" );
	}
    }
    while (!feof(source));

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}

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
    if(source)
        fclose (source);
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

    if(pbKeyBlobSimple)
	free(pbKeyBlobSimple);
    if(pbIV)
	free(pbIV);
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
