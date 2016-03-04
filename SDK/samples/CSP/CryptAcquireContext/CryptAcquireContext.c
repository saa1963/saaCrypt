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
#   define CONTAINER "test"
#else
#   include <string.h>
#   define CONTAINER "\\\\.\\HDIMAGE\\test"
#endif
#include <WinCryptEx.h>

//#define MAX_CONTAINER_NAME_LEN 128 // ���������� � wincryptex.h
// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ ����, ������������� CryptAcquireContext.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

static HCRYPTPROV hCryptProv = 0;
static HCRYPTKEY hKey = 0;

static void CleanUp(void);
static void HandleError(char *s);

int main(int argc, char *argv[])
{
    // ���������� � ������������� ����������.
    BYTE pbData[MAX_CONTAINER_NAME_LEN+1];
    DWORD cbData;
    BOOL bSilent = FALSE;

    // ���� ������ ��� ����
    if (argc > 1 && strcmp(argv[1], "-silent") == 0)
        bSilent = TRUE;
    printf("Mode: %s\n", bSilent ? "SILENT" : "NORMAL");

    // ��������� ����������� ����������������.
    if(!CryptAcquireContext(
	&hCryptProv, 
	CONTAINER, 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	if(!(GetLastError() == 0x80090016 ))
	{
	    SetLastError(0x80090016);
	    HandleError("A general error occurred running CryptAcquireContext");
	}

	printf(" The default key container does not exist.\n");
	printf(" Create a default container and generate keys \n");
	printf(" Using the code in \n");
	printf("  Example C Program: Creating a Key Container and Generating Keys \n");
	printf("  Before running this program. \n");
	
	CleanUp();
	exit(0);
    }
    printf("CryptAcquireContext succeeded.\n");
    
    // ������ ����� CSP.
    cbData = MAX_CONTAINER_NAME_LEN;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_NAME, 
	NULL, 
	&cbData, 
	0))
    {
	HandleError("Error reading CSP name. \n");
    }

    if(cbData > MAX_CONTAINER_NAME_LEN)
	HandleError("CSP name length is too long");

    pbData[MAX_CONTAINER_NAME_LEN] = 0;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_NAME, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading CSP name. \n");
    }

    printf("CryptGetProvParam succeeded.\n");
    printf("Provider name: %s\n", pbData);

    // ������ ����� ��������� ����������.
    cbData = MAX_CONTAINER_NAME_LEN;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_CONTAINER, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading key container name. \n");
    }
    if(cbData > MAX_CONTAINER_NAME_LEN)
	HandleError("Container name length is too long");

    if(!CryptGetProvParam(
	hCryptProv, 
	PP_CONTAINER, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading key container name. \n");
    }

    printf("CryptGetProvParam succeeded. \n");
    printf("Key Container name: %s\n", pbData);

    // ���������� ����������������� ��������.

    //  ���������� �������� ������ �� ���������������. ����� ������� ������ �� 
    //  ��������������� ���������� ������ 1, CryptReleaseContext ���������
    //  ������� ������, �� �� ����������� ���������������.

    if(!CryptContextAddRef(
	hCryptProv, 
	NULL, 
	0)) 
    {
	HandleError("Error during CryptContextAddRef!\n");
    }
    printf("CryptContextAddRef succeeded. \n");

    //  ������� ������ �� hCryptProv ������ ������ 1. ������ ����� 
    //  ������� CryptReleaseContext �� ��������� ���������� ����������������. 

    //  ����������� ������������ ���������.
    if(!CryptReleaseContext(hCryptProv, 0))
    {
	HandleError("Error during CryptReleaseContext! #1");
    }
    printf("The first call to CryptReleaseContext succeeded. \n");

    // ��������� ������������ ����������� ����������������.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {
	HandleError("Error during CryptReleaseContext! #2");
    }
    printf("The second call to CryptReleaseContext succeeded. \n");

    // ��������� ����������� ���������������� �
    // �������� ��������� ���������� � ������ "KC1_test".
    if(!CryptAcquireContext(
	&hCryptProv, 
#ifdef _WIN32
	"\\\\.\\REGISTRY\\KC1_test",
#else
	"\\\\.\\HDIMAGE\\KC1_test",
#endif
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_NEWKEYSET | (bSilent ? CRYPT_SILENT : 0))) 
    {
	HandleError("Error during CryptAcquireContext for a new key "
		    "container. A container with this name probably "
		    "already exists");
    }
    printf("CryptAcquireContext (CRYPT_NEWKEYSET) succeeded. \n");

    // begin ����������� ��������� CSP
    // ������������ ��������� CSP �������� ��, ��� ���������������
    // ��������� �� ������� (����������) ���������, ���� � ���
    // ��� �������� ������. ������� ���������� ������� ����.
    if(!CryptGenKey(
	hCryptProv,
	AT_SIGNATURE,
	0,
	&hKey)) 
    {	
	HandleError("Error occurred creating a signature key.\n"); 
    }
    printf("Created a signature key pair.\n");
    // end ����������� ��������� CSP

    // ���������� ����������������� ��������.

    // ������������ ����������� ���������������� � ��������� ���������� KC1.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {	
	HandleError("Error during CryptReleaseContext!\n");
    }
    printf("CryptReleaseContext succeeded. \n");
    hCryptProv = 0;
    hKey = 0;

    // ��������� ����������� ����������������, ��������� ����� �������� ���������. 
    // ���������: ���� �������� ��������� ����� ������ �� ��� ���, ���� �����
    // �� ����� ���� ������� � ������� ������� CryptGenKey.
    if(!CryptAcquireContext(
	&hCryptProv, 
	"KC1_test", 
	NULL, 
	PROV_GOST_2001_DH,
	0)) 
    {
	HandleError("Error during CryptAcquireContext!\n");
    }
    printf("Acquired the key set just created. \n");

    // ���������� ����������������� ��������.

    // ������������ ����������� ���������������� � ��������� ���������� KC1.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {	
	HandleError("Error during CryptReleaseContext!\n");
    }
    printf("CryptReleaseContext succeeded. \n");
    hCryptProv = 0;

    // ��� ���������� ������������ ����� ������� ���������� ������� ���������
    // ������ ��������� � ������ "��1". ��������� ����� ������� ��� ������
    // ����������� ������ ��������� CSP ��� ��������� �������:
    if(!CryptAcquireContext(
	&hCryptProv, 
	"KC1_test", 
	NULL, 
	PROV_GOST_2001_DH,
	CRYPT_DELETEKEYSET)) 
    {
	HandleError("Error during CryptAcquireContext(CRYPT_DELETEKEYSET)!\n");
    }
    printf("CryptAcquireContext (CRYPT_DELETEKEYSET) succeeded. \n");

    CleanUp();
    return 0;
}

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//--------------------------------------------------------------------
//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.
//--------------------------------------------------------------------

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp();
    if(!err) err = 1;
    exit(err);
}

void CleanUp(void)
{
    if(hKey) 
	CryptDestroyKey(hKey); 
    if(hCryptProv)
	CryptReleaseContext(hCryptProv, 0); 
}
