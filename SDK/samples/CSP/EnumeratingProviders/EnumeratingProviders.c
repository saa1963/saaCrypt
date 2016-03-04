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

#define NAME_LENGTH 100

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
//  � ������� ������������ ������� CryptEnumProviderTypes, CryptEnumProviders, 
//  CryptGetDefaultProvider � CryptGetProvParam.
//  ����������� �� ���� ������� �������� ������ ��� ������ Windows 2000 
//  � ������.
//
//  ��� ���������� ������ ������� ������� ���������� �������������� ������� 
//  ��������� � ������ "OUR_Container" (��������, ��� ������ ��������� 
//  CreatingKeyContainer).
//  ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV  hProv = 0;         // ���������� CSP
static LPTSTR      pszName = NULL;
static LPTSTR      pbProvName = NULL;

int main(void)
{

    //--------------------------------------------------------------------
    //  ���������� � ������������� ����������.

    
    DWORD       dwType;
    DWORD       cbName;
    DWORD       dwIndex = 0;
    BYTE        *ptr = NULL;
    ALG_ID      aiAlgid;
    DWORD       dwBits;
    DWORD       dwNameLen;
    CHAR        szName[NAME_LENGTH]; // ������������ �����������
    BYTE        pbData[1024];// ������������ �����������
    DWORD       cbData=1024;
    DWORD       dwIncrement = sizeof(DWORD);
    DWORD       dwFlags = CRYPT_FIRST;
    CHAR        *pszAlgType = NULL;
    BOOL        fMore = TRUE;  
    DWORD       cbProvName;

    //   ������ ��������� ������������ ����� �����������.
    printf("\n          Listing Available Provider Types.\n");
    printf("Provider type      Provider Type Name\n");
    printf("_____________    _____________________________________\n");  

    // ���� �� ������������� ����� �����������.
    dwIndex = 0;
    while(CryptEnumProviderTypes(
	dwIndex,     // in -- dwIndex
	NULL,        // in -- pdwReserved- ��������������� � NULL
	0,           // in -- dwFlags -- ��������������� � ����
	&dwType,     // out -- pdwProvType
	NULL,        // out -- pszProvName -- NULL ��� ������ ������
	&cbName      // in, out -- pcbProvName
	))
    {
	//  cbName - ����� ����� ���������� ���� ����������.
	//  ������������� ������ � ������ ��� �������������� ����� �����.
	pszName = (LPTSTR)malloc(cbName);
	if(!pszName)
	    HandleError("ERROR - malloc failed!");

	memset(pszName, 0, cbName);

	//--------------------------------------------------------------------
	//  ��������� ����� ���� ����������.

	if(CryptEnumProviderTypes(
	    dwIndex++,
	    NULL,
	    0,
	    &dwType,   
	    pszName,
	    &cbName))     
	{
	    printf ("     %4.0d        %s\n",dwType, pszName);
	}
	else
	{
	    HandleError("ERROR - CryptEnumProviders");
	}
    }

    //   ������ ��������� ������������ �����������.
    printf("\n\n          Listing Available Providers.\n");
    printf("Provider type      Provider Name\n");
    printf("_____________    _____________________________________\n");   

    // ���� �� ������������� �����������.
    dwIndex = 0;
    while(CryptEnumProviders(
	dwIndex,     // in -- dwIndex
	NULL,        // in -- pdwReserved- ��������������� � NULL
	0,           // in -- dwFlags -- ��������������� � ����
	&dwType,     // out -- pdwProvType
	NULL,        // out -- pszProvName -- NULL ��� ������ ������
	&cbName      // in, out -- pcbProvName
	))
    {
	//  cbName - ����� ����� ���������� ����������.
	//  ������������� ������ � ������ ��� �������������� ����� �����.
	pszName = (LPTSTR)malloc(cbName);
	if(!pszName)
	    HandleError("ERROR - malloc failed!");

	memset(pszName, 0, cbName);

	//  ��������� ����� ����������.
	if(CryptEnumProviders(
	    dwIndex++,
	    NULL,
	    0,
	    &dwType,
	    pszName,
	    &cbName     // pcbProvName -- ����� pszName
	    ))
	{
	    printf ("     %4.0d        %s\n",dwType, pszName);
	}
	else
	{
	    HandleError("ERROR - CryptEnumProviders");
	}

    } // ����� ����� while

    printf("\n\nProvider types and provider names have been listed.\n");

    // ��������� ����� CSP, ������������� ��� ���������� �� ���������.

    // ��������� ����� ����� ���������� �� ���������.
    if(!CryptGetDefaultProvider(
	PROV_GOST_2001_DH, 
	NULL, 
	CRYPT_MACHINE_DEFAULT,
	NULL, 
	&cbProvName)) 
    { 
	HandleError("Error getting the length of the default provider name.");
    }

    // ������������� ��������� ������ ��� ��� ���������� �� ���������.

    pbProvName = (LPTSTR)malloc(cbProvName);
    if(!pbProvName)
	HandleError("Error during memory allocation for provider name.");

    memset(pbProvName, 0, cbProvName);

    // ��������� ����� ���������� �� ���������.
    if(CryptGetDefaultProvider(
	PROV_GOST_2001_DH, 
	NULL, 
	CRYPT_MACHINE_DEFAULT,
	pbProvName,
	&cbProvName)) 
    {
	printf("The default provider name is %s\n\n",pbProvName);
    }
    else
    {
	HandleError("Getting the name of the provider failed.");
    }

    //-----------------------------------------------------
    //  ��������� ������������������ ���������.

    if(!CryptAcquireContext(
	&hProv, 
	NULL,
	NULL,
	PROV_GOST_2001_DH, 
	CRYPT_VERIFYCONTEXT))  
    {
	HandleError("Error during CryptAcquireContext!");
    }

    //------------------------------------------------------
    // ������������ �������������� ����������.

    //------------------------------------------------------
    // ������ ��������� �������, ���������� ���������� �� ���������� 
    printf("\n               Enumerating the supported algorithms\n\n");
    printf("     Algid      Bits      Type        Name         Algorithm\n");
    printf("                                     Length          Name\n");
    printf("    ________________________________________________________\n");

    while(fMore)
    {
	//------------------------------------------------------
	// ���������� ���������� �� ���������.

	if(CryptGetProvParam(hProv, PP_ENUMALGS, pbData, &cbData, dwFlags))
	{       
	    //-----------------------------------------------------------
	    // ���������� ���������� �� ��������� �� ������ 'pbData'.

	    dwFlags=0;
	    ptr = pbData;
	    aiAlgid = *(ALG_ID *)ptr;
	    ptr += sizeof(ALG_ID);
	    dwBits = *(DWORD *)ptr;
	    ptr += dwIncrement;
	    dwNameLen = *(DWORD *)ptr;
	    ptr += dwIncrement;
	    //strncpy_s(szName, NAME_LENGTH, (char *) ptr, dwNameLen);
	    strncpy(szName, (char *) ptr, dwNameLen);
	    // ����������� ���� ���������.

	    switch(GET_ALG_CLASS(aiAlgid)) {
		case ALG_CLASS_DATA_ENCRYPT: pszAlgType = "Encrypt  ";
		    break;
		case ALG_CLASS_HASH:         pszAlgType = "Hash     ";
		    break;
		case ALG_CLASS_KEY_EXCHANGE: pszAlgType = "Exchange ";
		    break;
		case ALG_CLASS_SIGNATURE:    pszAlgType = "Signature";
		    break;
		default:                     pszAlgType = "Unknown  ";
	    }

	    // ������ ���������� �� ���������.
	    printf("    %8.8xh    %-4d    %s     %-2d          %s\n",
		aiAlgid, dwBits, pszAlgType, dwNameLen, szName);
	}
	else
	    fMore = FALSE;
    }

    if(GetLastError() == ERROR_NO_MORE_ITEMS)
    { 
	printf("\nThe program completed without error.\n");
    }
    else
    { 
	HandleError("Error reading algorithm!");
    }

    CleanUp();
    return 0;
}

void CleanUp(void)
{
    // ������������ ����������� ����������.
    if(hProv) 
	CryptReleaseContext(hProv, 0);
    if(pszName)
	free(pszName);
    if(pbProvName)
	free(pbProvName);
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
