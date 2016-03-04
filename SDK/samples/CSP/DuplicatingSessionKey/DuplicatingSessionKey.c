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
//--------------------------------------------------------------------
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV   hCryptProv = 0;
static HCRYPTKEY    hOriginalKey = 0;
static HCRYPTKEY    hDuplicateKey = 0;

int main(void)
{
    DWORD        dwMode;
    BYTE         pbData[16];

    printf("This program creates a session key and duplicates \n");
    printf("that key. Next, parameters are added to the original \n");
    printf("key. Finally, both keys are destroyed. \n\n");

    // ��������� ����������� ��������� ������������������ ����������.
    if(CryptAcquireContext(    
	&hCryptProv,
	NULL,
	NULL,
	PROV_GOST_2001_DH,
	CRYPT_VERIFYCONTEXT)) 
    {    
	printf("CryptAcquireContext succeeded. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext!\n");
    }

    // ��������� �����.
    if(CryptGenKey(
	hCryptProv, 
	CALG_G28147, 
	0, 
	&hOriginalKey))
    {
	printf("Original session key is created. \n");
    }
    else
    {
	HandleError("ERROR - CryptGenKey.");
    }

    // ������������ �����.
    if(CryptDuplicateKey(
	hOriginalKey, 
	NULL, 
	0, 
	&hDuplicateKey))
    {
	printf("The session key has been duplicated. \n");
    }
    else
    {
	HandleError("ERROR - CryptDuplicateKey");
    }

    //  ��������� �������������� ���������� ��������� �����.
    //  � ������ �������, ��������� ������ ����������.
    dwMode = CRYPT_MODE_ECB;
    if(CryptSetKeyParam(
	hOriginalKey, 
	KP_MODE, 
	(BYTE*)&dwMode, 
	0)) 
    {
	printf("Key Parameters set. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam.");
    }

    // ��������� ���������� ������� �������������.
    if(CryptGenRandom(
	hCryptProv, 
	8, 
	pbData)) 
    {
	printf("Random sequence generated. \n");
    }
    else
    {
	HandleError("Error during CryptGenRandom.");
    }

    // ��������� ������� �������������.
    if(CryptSetKeyParam(
	hOriginalKey, 
	KP_IV, 
	pbData, 
	0)) 
    {
	printf("Parameter set with random sequence as initialization vector. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam.");
    }

    CleanUp();

    printf("\nThe program ran to completion without error. \n");
    return 0;
} 

void CleanUp(void)
{
    if(hOriginalKey)
	CryptDestroyKey(hOriginalKey);

    if(hDuplicateKey)
	CryptDestroyKey(hDuplicateKey);

    if(hCryptProv)
	CryptReleaseContext(hCryptProv, 0);
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
    CleanUp();
    if(!err) err = 1;
    exit(err);
}
