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
#   include <tchar.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#   include "reader/tchar.h"
#endif
#include <WinCryptEx.h>

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// � ������ ������� �������������� ��������������� ����������� �����
// ��� ������ ������������ ���������� � �����������.
// ��� ���������� ������ ���� ��������� ���������� ������� ��� 
// ����������� � �������� �� � ��������� ������������ MY: ���������� 
// (� ������, ���������� "Responder") � ����������� (c ������, ���������� 
// "Sender").
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(BOOL);

static HCERTSTORE hStoreHandle = 0;	    // ���������� ��������� ������������
static PCCERT_CONTEXT pCertSender = NULL;  // �������� ����������� �����������
static PCCERT_CONTEXT pCertResponder = NULL;	// �������� ����������� ����������
static HCRYPTPROV hProvResponder = 0;	    // ���������� CSP ���������� 
static HCRYPTPROV hProvSender = 0;         // ���������� CSP �����������
static HCRYPTKEY hSenderKey = 0;	    // ���������� ��������� ����� �����������
static HCRYPTKEY hResponderKey = 0;	    // ���������� ��������� ����� ����������
static HCRYPTKEY hSessionKey = 0;	    // ���������� ����������� ����� 
static HCRYPTKEY hAgreeKey = 0;	    // ���������� ����� ������������

static BYTE *pbKeyBlobSimple = NULL;       // ��������� �� ���������� �������� BLOB 
static BYTE *pbKeyBlobSender = NULL;       // ��������� �� �������� BLOB �����������
static BYTE *pbKeyBlobResponder = NULL;    // ��������� �� �������� BLOB ����������

int main(void)
{
    DWORD dwBlobLenSimple;	    // ����� ����������� ��������� BLOB�
    DWORD dwBlobLenSender;	    // ����� ��������� BLOB� �����������
    DWORD dwBlobLenResponder;	    // ����� ��������� BLOB� ����������
    DWORD dwKeySpecSender;
    DWORD dwKeySpecResponder;

    // �� ������� �����������:

    // �������� ���������� ��������� ������������.
    hStoreHandle = CertOpenSystemStore(0, "MY");

    if(hStoreHandle)
    {
	printf("The MY store is open. \n");
    }
    else
    {
	HandleError( "Error getting store handle.");
    }

    //--------------------------------------------------------------------
    // ��������� ��������� ���������, � �������� �������� ���������� "Sender", 
    // ������������ � ��������� ������������ "MY".

    pCertSender = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Sender"),
	NULL);

    if(pCertSender)
    {
	printf("The \" Sender \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� CSP, ������� ������ � ���������� � ��� ���������
    // ���������� ��� ��������� ����������� pCertSender.

    if(CryptAcquireCertificatePrivateKey(
	pCertSender,
	0,
	NULL,
	&hProvSender,
	&dwKeySpecSender,
	NULL))
    {
	printf("The CSP handle has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireCertificatePrivateKey.");
    }

    //--------------------------------------------------------------------
    // ����� ���������, � �������� �������� ���������� "Responder", 
    // ������������ � ��������� ������������ "MY".

    pCertResponder = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Responder"),
	NULL);

    if(pCertResponder)
    {
	printf("The \" Responder \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� ����������.

    if(CryptImportPublicKeyInfoEx( 
	hProvSender,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	&(pCertResponder->pCertInfo->SubjectPublicKeyInfo),
	0,
	0,
	NULL,
	&hResponderKey))
    {
	printf("The responder public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptImportPublicKeyInfoEx public key.");
    }

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ��������� ����� � ������������� ������.

    if(CryptExportKey(
	hResponderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&dwBlobLenResponder)) 
    {
	printf("Size of the BLOB for the responder public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobResponder = (BYTE*)malloc(dwBlobLenResponder);

    if(pbKeyBlobResponder) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // ��������������� ��������� ����� ���������� � BLOB ��������� �����.

    if(CryptExportKey(
	hResponderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlobResponder, 
	&dwBlobLenResponder))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� �����������.

    if(CryptGetUserKey(
	hProvSender, 
	dwKeySpecSender, 
	&hSenderKey)) 
    {
	printf("The sender private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������ �������� ��������� ����� ���������� �� BLOB� 
    // �� �������� ����� �����������.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlobResponder, 
	dwBlobLenResponder, 
	hSenderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� �����.

    if(CryptGenKey(     
	hProvSender,      
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
    // pbKeyBlobSimple - ������������� ���������� ����, �������� ��� ����������
    //
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

    if(pbKeyBlobSimple) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // ���������� ����������� ����� �� ����� Agree.

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


    //--------------------------------------------------------------------
    // �������� ���������� ��������� ������������.

    if(CertCloseStore(
	hStoreHandle, 
	0))
    {
	printf("The MY store is closed. \n");
	hStoreHandle=0;
    }
    else
    {
	HandleError( "Error closing store handle.");
    }

    CleanUp(FALSE);


    //--------------------------------------------------------------------
    // �� ������� ����������: �������� ������������� ���������� ����
    // pbKeyBlobSimple.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // �������� ���������� ��������� ������������.

    hStoreHandle = CertOpenSystemStore(0, "MY");
    if(hStoreHandle)
    {
	printf("The MY store is open. \n");
    }
    else
    {
	HandleError( "Error getting store handle.");
    }

    //--------------------------------------------------------------------
    // ��������� ��������� ���������, � �������� �������� ���������� "Responder", 
    // ������������ � ��������� ������������ "MY".

    pCertResponder = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Responder"),
	NULL);

    if(pCertResponder)
    {
	printf("The \" Responder \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� CSP, ������� ������ � ���������� � ��� ���������
    // ���������� ��� ��������� ����������� pCertResponder.

    if(CryptAcquireCertificatePrivateKey(
	pCertResponder,
	0,
	NULL,
	&hProvResponder,
	&dwKeySpecResponder,
	NULL))
    {
	printf("The CSP handle has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireCertificatePrivateKey.");
    }

    //--------------------------------------------------------------------
    // ����� ���������, � �������� �������� ���������� "Sender", 
    // ������������ � ��������� ������������ "MY".

    pCertSender = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Sender"),
	NULL);

    if(pCertSender)
    {
	printf("The \" Sender \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }
    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� �����������.

    if(CryptImportPublicKeyInfoEx( 
	hProvResponder,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	&(pCertSender->pCertInfo->SubjectPublicKeyInfo),
	0,
	0,
	NULL,
	&hSenderKey))
    {
	printf("The sender public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptImportPublicKeyInfoEx public key.");
    }

    //--------------------------------------------------------------------
    // ����������� ������� BLOB� ��������� ����� � ������������� ������.

    if(CryptExportKey(
	hSenderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&dwBlobLenSender)) 
    {
	printf("Size of the BLOB for the sender public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobSender = (BYTE*)malloc(dwBlobLenSender);

    if(pbKeyBlobSender) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // ��������������� ��������� ����� ����������� � BLOB ��������� �����.

    if(CryptExportKey(
	hSenderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlobSender, 
	&dwBlobLenSender))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� ����������.

    if(CryptGetUserKey(
	hProvResponder, 
	dwKeySpecResponder, 
	&hResponderKey)) 
    {
	printf("The responder private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    //--------------------------------------------------------------------
    // ��������� ����� ������������ �������� ��������� ����� ����������� �� BLOB� 
    // �� �������� ����� ����������.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSender, 
	dwBlobLenSender, 
	hResponderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    // ��������� ����������� ����� �������� �������������� ����������� ����� 
    // �� ����� Agree.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSimple, 
	dwBlobLenSimple, 
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


    CleanUp(TRUE);

    printf("The program ran to completion without error. \n");
    return 0;
}

void CleanUp(BOOL all)
{

    if(pbKeyBlobResponder)
    {
	    free(pbKeyBlobResponder);
	    pbKeyBlobResponder=NULL;
    }
    if(pbKeyBlobSimple && all)
    {
	    free(pbKeyBlobSimple);
	    pbKeyBlobSimple=NULL;
    }
    if(pbKeyBlobSender)
    {
	    free(pbKeyBlobSender);
	    pbKeyBlobSender=NULL;
    }

    if(hStoreHandle)
    {
	    CertCloseStore(hStoreHandle, 0);
	    hStoreHandle=0;
    }

    // ������������ ����������� ����������.
    if(hProvSender) 
    {
	CryptReleaseContext(hProvSender, 0);
	hProvSender=0;
    }

    // ����������� ����������� ��������� ����� ����������.
    if(hResponderKey)
    {
	CryptDestroyKey(hResponderKey);
	hResponderKey=0;
    }

    // ����������� ����������� ��������� ����� �����������.
    if(hSenderKey)
    {
	CryptDestroyKey(hSenderKey);
	hSenderKey=0;
    }

    // ����������� ����������� �����.
    if(hSessionKey)
    {
	CryptDestroyKey(hSessionKey);
	hSessionKey=0;
    }

    // ����������� ����� ������������.
    if(hAgreeKey)
    {
	CryptDestroyKey(hAgreeKey);
	hAgreeKey=0;
    }

    // ������������ ����������� ����������.
    if(hProvResponder) 
    {
	CryptReleaseContext(hProvResponder, 0);
	hProvResponder=0;
    }

    // ������������ ��������� �����������. 
    if(pCertSender)
    {
	CertFreeCertificateContext(pCertSender);
	pCertSender=NULL;
    }

    if(pCertResponder)
    {
	CertFreeCertificateContext(pCertResponder);
	pCertResponder=NULL;
    }
}

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp(TRUE);
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE
