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
#   include <wincryptex.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#   include <WinCryptEx.h>
#endif

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ �������� ����������� �����, ����������� �� ����, ������������� 
// �� ������ ������������. ������ �������� � �������� ������ ���������.
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hCryptProv = 0;
static HCRYPTKEY hKey = 0;
static HCRYPTHASH hHash = 0;

int main(void)
{
    //--------------------------------------------------------------------
    //  ������ �� �������� ����� ������� ���������� ����.
    
    CHAR szPassword[] = "123456\0";
    DWORD dwLength;


    dwLength = (DWORD)strlen(szPassword);

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ������������������ ����������.

    if(CryptAcquireContext(
	&hCryptProv, 
	NULL, 
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_VERIFYCONTEXT)) 
    {
	printf("A context has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext!");
    }
    //--------------------------------------------------------------------
    // �������� ������� ������� �����������.

    if(CryptCreateHash(
	hCryptProv, 
	CALG_GR3411, 
	0, 
	0, 
	&hHash)) 
    {
	printf("An empty hash object has been created. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash!");
    }
    //--------------------------------------------------------------------
    // ����������� ������ ������.

    if(CryptHashData(
	hHash, 
	(BYTE *)szPassword, 
	dwLength, 
	0)) 
    {
	printf("The password has been hashed. \n");
    }
    else
    {
	HandleError("Error during CryptHashData!");
    }
    //--------------------------------------------------------------------
    // �������� ����������� �����, ����������� �� ����, ����������� �� ������.

    if(CryptDeriveKey(
	hCryptProv, 
	CALG_G28147, 
	hHash, 
	CRYPT_EXPORTABLE, 
	&hKey)) 
    {
	printf("The key has been derived. \n");
    }
    else
    {
	HandleError("Error during CryptDeriveKey!");
    }

    CleanUp();

    printf("The program to derive a key completed without error. \n");

    return 0;
} 


void CleanUp(void)
{
    if(hHash) 
	CryptDestroyHash(hHash);

    // ����������� ����������� �����.
    if(hKey) 
	CryptDestroyKey(hKey);

    // ������������ ����������� ����������.
    if(hCryptProv) 
	CryptReleaseContext(hCryptProv, 0);
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
