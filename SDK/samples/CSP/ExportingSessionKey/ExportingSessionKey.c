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
// � ������ ������� �������������� ��������������� ����������� �����
// ��� ������ ��������� BLOB.
// ��� ���������� ������ ���� ��������� ���������� ������� ��� 
// ����������: ���������� (� ������ "Responder") � ����������� (c ������
// "Sender").
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProvResponder = 0;     // ���������� CSP ���������� 
static HCRYPTPROV hProvSender = 0;        // ���������� CSP �����������
static HCRYPTKEY hSessionKey = 0;         // ���������� ����������� ����� 
static HCRYPTKEY hAgreeKey = 0;           // ���������� ����� ������������
static HCRYPTKEY hResponderKey = 0;       // ���������� ��������� ����� ����������
static HCRYPTKEY hSenderKey = 0;          // ���������� ��������� ����� �����������
static BYTE *pbKeyBlobSender = NULL;	   // ��������� �� �������� BLOB �����������
static BYTE *pbKeyBlobResponder = NULL;   // ��������� �� �������� BLOB ����������
static BYTE *pbKeyBlobSimple = NULL;      // ��������� �� ���������� �������� BLOB 

int main(void)
{

    DWORD dwBlobLenSender;         // ����� ��������� BLOB� �����������
    DWORD dwBlobLenResponder;      // ����� ��������� BLOB� ����������
    DWORD dwBlobLenSimple;         // ����� ����������� ��������� BLOB�

    //--------------------------------------------------------------------
    // �� ������� ����������: ���������� �������� ���� �����������
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // ��������� ����������� ���������� ���������� � ������ "Responder", 
    // ������������ � ������ ����������. 

    if(CryptAcquireContext(
	&hProvResponder, 
	"Responder", 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"Responder\" has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }
    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� ����������.

    if(CryptGetUserKey(
	hProvResponder, 
	AT_KEYEXCHANGE, 
	&hResponderKey)) 
    {
	printf("The responder public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    //--------------------------------------------------------------------
    // pbKeyBlobResponder - �������� ���� ����������, �������� ��� ����������.
    //
    //--------------------------------------------------------------------

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
    // �� ������� �����������: ���������� �������� ���� ����������.
    //--------------------------------------------------------------------

    // ��������� ����������� ���������� ����������� � ������ "Sender", 
    // ������������ � ������ ����������. 

    if(CryptAcquireContext(
	&hProvSender, 
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
    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ����� �����������.

    if(CryptGetUserKey(
	hProvSender, 
	AT_KEYEXCHANGE, 
	&hSenderKey)) 
    {
	printf("The sender public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    //--------------------------------------------------------------------
    // pbKeyBlobSender - �������� ���� �����������, �������� ��� ����������.
    //
    //--------------------------------------------------------------------

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
    // ��������� ����� ������������ �������� ��������� ����� ���������� 
    // �� �������� ����� �����������.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlobResponder, 
	dwBlobLenResponder, 
	hSenderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
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
    // pbKeyBlobSimple - ������������� ���������� ����, �������� ��� ����������.
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
    // ���������� ����� ������������ �� ���������� �����.

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
    // �� ������� ����������: �������� ������������� ���������� ����: 
    // pbKeyBlobSimple, �������� �������� ����: pbKeyBlobSender. 
    //--------------------------------------------------------------------

    // ��������� ����� ������������ �������� ��������� ����� ����������� 
    // �� �������� ����� ����������.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSender, 
	dwBlobLenSender, 
	hResponderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
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

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}                                               

void CleanUp(void)
{
    free(pbKeyBlobSender);
    free(pbKeyBlobResponder);
    free(pbKeyBlobSimple);

    // ����������� ����������� �����.
    if(hSessionKey)
	CryptDestroyKey(hSessionKey);

    // ����������� ����� ������������.
    if(hAgreeKey)
	CryptDestroyKey(hAgreeKey);

    // ����������� ����������� ��������� ����� ����������.
    if(hResponderKey)
	CryptDestroyKey(hResponderKey);

    // ����������� ����������� ��������� ����� �����������.
    if(hSenderKey)
	CryptDestroyKey(hSenderKey);

    // ������������ ����������� ����������.
    if(hProvResponder) 
	CryptReleaseContext(hProvResponder, 0);
    if(hProvSender) 
	CryptReleaseContext(hProvSender, 0);
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
