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
// � ������ ������� �������������� ����������� ������, ������������
// ����������� ����. ����� �������������� ����������� �������������� 
// ������ ��� ������ ������������� � �������������� ����.
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);
static void Get_And_Print_Hash(HCRYPTHASH hHash);

static HCRYPTPROV   hCryptProv = 0;
static HCRYPTHASH   hOriginalHash = 0;
static HCRYPTHASH   hDuplicateHash = 0;

int main(void)
{
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
	HandleError("Error during CryptAcquireContext!");
    }
    //--------------------------------------------------------------------
    // �������� ������� ������� �����������.

    if(CryptCreateHash(
	hCryptProv, 
	CALG_GR3411, 
	0, 
	0,
	&hOriginalHash))
    {
	printf("An empty hash object has been created. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash.");
    }
    //--------------------------------------------------------------------
    // ����������� �������� ������.

    if(CryptHashData(
	hOriginalHash, 
	(BYTE*)"Some Common Data", 
	sizeof("Some Common Data"), 0))
    {
	printf("An original hash has been created. \n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }
    //--------------------------------------------------------------------
    // ������������ ����.
    // ��� ������� �������� ������ � Windows 2000 � ������.

    if(CryptDuplicateHash(
	hOriginalHash, 
	NULL, 
	0, 
	&hDuplicateHash))
    {
	printf("The hash has been duplicated. \n");
    }
    else
    {
	HandleError("Error during CryptDuplicateHash.");
    }

    printf("The original hash -- phase 1.\n");
    Get_And_Print_Hash(hOriginalHash);

    printf("The duplicate hash -- phase 1.\n");
    Get_And_Print_Hash(hDuplicateHash);

    //--------------------------------------------------------------------
    // ����������� "Some Data" � ������������ �����.

    if(CryptHashData(
	hOriginalHash, 
	(BYTE*)"Some Data", 
	sizeof("Some Data"),
	0))
    {
	printf("Additional data has been hashed with the original. \n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }
    //--------------------------------------------------------------------
    // ����������� "Other Data" � ������������� �����.

    if(CryptHashData(
	hDuplicateHash, 
	(BYTE*)"Other Data", 
	sizeof("Other Data"),
	0))
    {
	printf("More data has been hashed with the duplicate. \n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }


    printf("The original hash -- phase 2.\n");
    Get_And_Print_Hash(hOriginalHash);

    printf("The duplicate hash -- phase 2.\n");
    Get_And_Print_Hash(hDuplicateHash);

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}

// ����������� ������� Get_And_Print_Hash.
void Get_And_Print_Hash(HCRYPTHASH hOHash)
{
    BYTE         *pbHash;
    BYTE         *pbHashSize;
    DWORD        dwHashLen = sizeof(DWORD);
    DWORD        i;
    HCRYPTHASH   hHash=0;

    //--------------------------------------------------------------------
    // ������������ �������������� ����.
    // ��� ������� �������� ������ � Windows 2000 � ������.
    // ��� ������������� ��� ����, ����� �� �������� �������� ���.

    if(!CryptDuplicateHash(
	hOHash, 
	NULL, 
	0, 
	&hHash))
    {
	HandleError("Error during CryptDuplicateHash.");
    }

    pbHashSize =(BYTE *) malloc(dwHashLen);
    if(!pbHashSize)
	HandleError("Memory allocation failed.");

    if(CryptGetHashParam(
	hHash, 
	HP_HASHSIZE, 
	pbHashSize, 
	&dwHashLen, 
	0)) 
    {
	// ��������. ������������ pbHashSize.
	free(pbHashSize);
    }
    else
    {
	free(pbHashSize);
	HandleError("CryptGetHashParam failed to get size.");
    }

    if(!CryptGetHashParam(
	hHash, 
	HP_HASHVAL, 
	NULL, 
	&dwHashLen, 
	0)) 
    {
	HandleError("CryptGetHashParam failed to get length.");
    }

    pbHash = (BYTE*)malloc(dwHashLen);
    if(!pbHash)
	HandleError("Allocation failed.");

    if(CryptGetHashParam(
	hHash, 
	HP_HASHVAL, 
	pbHash, 
	&dwHashLen, 
	0)) 
    {
	// ������ �������� ����.
	printf("The hash is:  ");
	for(i = 0 ; i < dwHashLen ; i++) 
	{
	    printf("%2.2x ",pbHash[i]);
	}
	printf("\n");
	free(pbHash);
    }
    else
    {
	free(pbHash);
	HandleError("Error during reading hash value.");
    }
    
    if(!CryptDestroyHash(hHash))
    {
	HandleError("ERROR - CryptDestroyHash");
    }
} 

void CleanUp(void)
{
    // ����������� ������������� ����.
    if (hOriginalHash)
	CryptDestroyHash(hOriginalHash);
    printf("The original hash has been destroyed. \n");

    // ����������� �������������� ����.
    if (hDuplicateHash)
	CryptDestroyHash(hDuplicateHash);
    printf("The duplicate hash has been destroyed. \n");

    // ������������ CSP.
    if(hCryptProv) 
	CryptReleaseContext(hCryptProv,0);
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
