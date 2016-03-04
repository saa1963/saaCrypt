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
// � ������ ������� �������������� �������� ���������� ����������� 
// ����� ���������� � ��������� ������ �� ��� ���������� (�������
// �������������).
//---------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;
static HCRYPTKEY hKey = 0;

int main(void)
{
    DWORD dwMode;
    BYTE pbData[16];
    BYTE pbRandomData[8];
    DWORD dwCount;
    DWORD i;

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� ������������������ ���������.

    if(!CryptAcquireContext(
	&hProv, 
	NULL, 
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_VERIFYCONTEXT)) 
    {
	HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------
    //  ��������� ������ ������ ��������� ������ � pbRandomData.

    if(CryptGenRandom(
	hProv,
	8,
	pbRandomData))
    {
	printf("Eight bytes of random data have been generated.\n");
    }
    else
    {
	HandleError("Random bytes were not correctly generated.");
    }
    // �������� ���������� ����������� ����� �������� ����������.

    if(!CryptGenKey(
	hProv, 
	CALG_G28147, 
	CRYPT_EXPORTABLE, 
	&hKey)) 
    {
	HandleError("Error during CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // ������ ������ ����������.

    dwCount = sizeof(DWORD);
    if(CryptGetKeyParam(
	hKey, 
	KP_MODE, 
	(BYTE*)&dwMode, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------------
	// ������ ������ ����������.

	printf("Default cipher mode: %d\n", dwMode);

    }
    else
    {
	HandleError("Error during CryptGetKeyParam.");
    }

    //---------------------------------------------------------------
    // ������ ������� �������������.

    //-------------------------------------------------------------
    //  ��������� ����� ������� �������������.

    if(!CryptGetKeyParam(
	hKey, 
	KP_IV, 
	NULL,     
	&dwCount, 
	0)) 
    {
	HandleError("Error getting the IV length");
    }

    //--------------------------------------------------------------
    // ��������� IV - ������� �������������.

    if(CryptGetKeyParam(
	hKey, 
	KP_IV, 
	pbData, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------
	// ������ ������� �������������.
	printf("Default IV:");
	for(i=0;i<dwCount;i++) 
	    printf("%2.2x ",pbData[i]);
	printf("\n");
    }
    else
    {
	HandleError("Error getting the IV.");
    }

    //-------------------------------------------------------------
    //  ������������� ������� �������������.

    if(CryptSetKeyParam(
	hKey,
	KP_IV,
	pbRandomData,
	0))
    {
	printf("New intialization vector is set.\n");
    }
    else
    {
	HandleError("The new IV was not set.");
    }
    //---------------------------------------------------------------
    // ������ ������ ������� �������������.

    //-------------------------------------------------------------
    //  ��������� ����� ������ ������� �������������.

    if(!CryptGetKeyParam(
	hKey, 
	KP_IV, 
	NULL,     
	&dwCount, 
	0)) 
    {
	HandleError("Error getting the IV length");
    }

    //--------------------------------------------------------------
    // ��������� IV - ������� �������������.

    if(CryptGetKeyParam(
	hKey, 
	KP_IV, 
	pbData, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------
	// ������ ������� �������������.
	printf("RE-set IV:");
	for(i=0;i<dwCount;i++) 
	    printf("%2.2x ",pbData[i]);
	printf("\n");
    }
    else
    {
	HandleError("Error getting the IV.");
    }


    CleanUp();

    printf("\nProgram completing without error.\n\n");

    return 0;
}

void CleanUp(void)
{
    if(hKey)
	CryptDestroyKey(hKey);

    if(hProv)
	CryptReleaseContext(hProv, 0);
}
// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//------------------------------------------------------------------------------
//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� �� ������ � ����������� ���� 
//  ������ (stderr) � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.
//------------------------------------------------------------------------------
void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp();
    if(!err) err = 1;
    exit(err);
}
