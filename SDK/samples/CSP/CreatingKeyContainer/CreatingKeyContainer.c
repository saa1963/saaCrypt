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
// ������ �������� ��������� ���������� � ������, ������������ � ��������
// ��������� ��������� ������. ���� �������� �� ������, �� ����� ������ 
// ��������� � ������ �� ���������. � ���������� ����������� ��� �����
// (���� ������ � ���� �������).
//--------------------------------------------------------------------

static HCRYPTPROV hCryptProv = 0;        // ���������� ���������  ����������������� ����������.
static HCRYPTKEY hKey = 0;               // ���������� ���������/��������� �����.

static void CleanUp(void);
static void HandleError(char *s);

int main(int argc, char *argv[]) 
{ 
    // ���������� � ������������� ����������.
    LPSTR pszUserName;		  // ����� ��� �������� �����  ��������� ����������.
    DWORD dwUserNameLen;	  // ����� ������.
    LPCSTR UserName;	          // ����������� �� ������ ��� ������������ 
			          // ����� ����� ������������ ��� ���
				  // ��������� ���������� (����������� �� 100 ��������).

    // ������ ����������. ��������� ����� ������������ ����������.
    if(argc < 2) 
	HandleError(" using: CreatingKeyContainer.exe <container_name>");

    UserName = argv[1];

    //  ��� �������� ������ ��������� ���������� ������ ������� ���������
    //  ���������� �� NULL ����� � ��� ��������� ������ �������:
    if(CryptAcquireContext(
	&hCryptProv,               // ���������� CSP
	UserName,                  // ��� ���������� 
	NULL,                      // ������������� ���������� �� ���������
	PROV_GOST_2001_DH,	   // ��� ����������
	0))                        // �������� ������
    {
	printf("A cryptcontext with the %s key container has been acquired.\n", UserName);
    }
    else
    { 
	// �������� ������ ����������. 
	if(!CryptAcquireContext(
	    &hCryptProv, 
	    UserName, 
	    NULL, 
	    PROV_GOST_2001_DH, 
	    CRYPT_NEWKEYSET)) 
	{
	     HandleError("Could not create a new key container.\n");
	}
	printf("A new key container has been created.\n");
    } 

    // ����������������� �������� � �������� ����������� ��������. ���������
    // ����� ��������� ����������. 
    if(!CryptGetProvParam(
	hCryptProv,               // ���������� CSP
	PP_CONTAINER,             // ��������� ����� ��������� ����������
	NULL,		          // ��������� �� ��� ��������� ����������
	&dwUserNameLen,           // ����� �����
	0)) 
    {
	// ������ ��������� ����� ��������� ����������
	HandleError("error occurred getting the key container name.");
    } 
    
    // ����� ������������ auto_ptr:
    //std::auto_ptr<char> aptrUserName(new char[dwUserNameLen+1]);
    //szUserName = aptrUserName.get();
    pszUserName=(char *)malloc((dwUserNameLen+1));
    
    if(!CryptGetProvParam(
	hCryptProv,               // ���������� CSP
	PP_CONTAINER,             // ��������� ����� ��������� ����������
	(LPBYTE)pszUserName,	  // ��������� �� ��� ��������� ����������
	&dwUserNameLen,           // ����� �����
	0)) 
    {
	// ������ ��������� ����� ��������� ����������
        free(pszUserName);
	HandleError("error occurred getting the key container name.");
    } 

    printf("A crypto context has been acquired and \n");
    printf("The name on the key container is %s\n\n", pszUserName);
    free(pszUserName);

    // �������� � �������� ����������� ��������,
    // ������� ��������� ����������� ����� �������
    if(CryptGetUserKey(
	hCryptProv,                     // ���������� CSP
	AT_SIGNATURE,                   // ������������ �����
	&hKey))                         // ���������� �����
    {
	printf("A signature key is available.\n");
    }
    else
    {
	printf("No signature key is available.\n");

	// ������ � ���, ��� ��������� �� �������� �����.
	if(!(GetLastError() == (DWORD)NTE_NO_KEY)) 
	    HandleError("An error other than NTE_NO_KEY getting signature key.\n");

	// �������� ����������� �������� ����. 
	printf("The signature key does not exist.\n");
	printf("Creating a signature key pair...\n"); 

	if(!CryptGenKey(
	    hCryptProv,
	    AT_SIGNATURE,
	    0,
	    &hKey)) 
	{
	    HandleError("Error occurred creating a signature key.\n"); 
	}
	printf("Created a signature key pair.\n");

    }

    //�������� ����� ������:
    if(CryptGetUserKey(
	hCryptProv,
	AT_KEYEXCHANGE,
	&hKey)) 
    {
	printf("An exchange key exists. \n");
    }
    else
    {
	printf("No exchange key is available.\n");

	// �������� �� ��, ����� �� ���-���� ���������.
	if(!(GetLastError() == (DWORD)NTE_NO_KEY))
	    HandleError("Error occurred attempting to create an exchange key.\n");

	// �������� �������� ���� ������.
	printf("The exchange key does not exist.\n");
	printf("Attempting to create an exchange key pair.\n");

	if(!CryptGenKey(
	    hCryptProv,
	    AT_KEYEXCHANGE,
	    0,
	    &hKey)) 
	{
	    HandleError("Error occurred attempting to create an exchange key.\n");
	}
	printf("Exchange key pair created.\n");
    }

    printf("An exchange key pair existed, or one was created.\n\n");

    CleanUp();

    printf("Everything is okay. A signature key\n");
    printf("pair and an exchange key exist in\n");
    printf("the %s key container.\n", UserName);  

    return 0;
}

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

void CleanUp(void)
{
    if(hKey) 
	CryptDestroyKey(hKey); 
    if(hCryptProv) 
	CryptReleaseContext(hCryptProv, 0); 
}

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
