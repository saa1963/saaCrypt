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
#include <WinCryptEx.h>

#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ ���� ��� ������ � ������ SILENT, ��� ����.
// � ������ ������� �������������� ������� ������.
// ��� ���������� ������ ���� ��������� ���������� ������� ��������� � 
// ������ "Source" � � ������� "1234". ��� ���� � ���������� ������ 
// ������������ ��� ����� (���� ������ � ���� �������).
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#define DEF_CONT_DISK "\\\\.\\HDIMAGE\\Encryption"
#define DEF_CONT_KEY "\\\\.\\FLASH\\Encryptor"
static BOOL OpenEncryptor(HCRYPTPROV * hProv, CHAR * lpContainer, CHAR * lpPin )
{
    HCRYPTPROV hProvK = 0;
    HCRYPTKEY hKeyK = 0;
    if (!CryptAcquireContext(&hProvK, lpContainer, NULL, PROV_GOST_2001_DH, 0))
    {
	HRESULT Err=GetLastError();
	if ( Err == NTE_KEYSET_NOT_DEF )
	{
	    printf("Encryptor container not found.Creating...\n");
	
	    if (!CryptAcquireContext(&hProvK, lpContainer, NULL, PROV_GOST_2001_DH, CRYPT_NEWKEYSET))
	    {
		printf("CryptAcquireContext(Encryptor) failed: %x\n", GetLastError());
		return FALSE;
	    }
	    if (lpPin)
	    {
	      if (!CryptSetProvParam(hProvK, PP_KEYEXCHANGE_PIN, (LPBYTE)lpPin, 0 ))
		{
		    printf("CryptSetProvParam(PP_KEYEXCHANGE_PIN,Encryptor) failed: %x\n", GetLastError());
		    CryptReleaseContext(hProvK, 0);
		    return FALSE;
		}
	    }
	    if (!CryptGenKey(hProvK, AT_KEYEXCHANGE, 0, &hKeyK))
	    {
		printf("CryptGenKey(Encryptor) failed: %x\n", GetLastError());
		CryptReleaseContext(hProvK, 0);
		return FALSE;
	    }
	}
	else
	{
	    printf("CryptAcquireContext(Encryptor) failed: %x\n", Err);
	    CryptReleaseContext(hProvK, 0);
	    return FALSE;
	}
    }
    else
    {
	if (lpPin)
	{
	  if (!CryptSetProvParam(hProvK, PP_KEYEXCHANGE_PIN, (LPBYTE)lpPin, 0 ))
	    {
		printf("CryptSetProvParam(PP_KEYEXCHANGE_PIN,Encryptor) failed: %x\n", GetLastError());
		CryptReleaseContext(hProvK, 0);
		return FALSE;
	    }
	}
	if (!CryptGetUserKey(hProvK, AT_KEYEXCHANGE, &hKeyK))
	{
		printf("CryptGetUserKey(AT_KEYEXCHANGE,Encryptor) failed: %x\n", GetLastError());
		CryptReleaseContext(hProvK, 0);
		return FALSE;

	}
    }

    CryptDestroyKey(hKeyK);
    hKeyK = 0;
    *hProv = hProvK;
    return TRUE;
}

static BOOL GetEncryptionHandle(HCRYPTPROV hProv, DWORD * phProvInt)
{
    DWORD dwLen = sizeof(DWORD);
    if (!CryptGetProvParam(hProv,PP_HCRYPTPROV, (BYTE *)phProvInt,&dwLen,0))
    {
		printf("CryptGetProvParam(PP_HCRYPTPROV,Encryptor) failed: %x\n", GetLastError());
		return FALSE;

    }
    return TRUE;
}

static BOOL EncryptContainer(HCRYPTPROV hEncryption, HCRYPTPROV hEncryptor)
{
    CRYPT_PIN_PARAM set_pin;
    DWORD hProvInt;
    if (!GetEncryptionHandle(hEncryptor, &hProvInt))
	return FALSE;
    set_pin.type = CRYPT_PIN_ENCRYPTION;
    set_pin.dest.prov = hProvInt;
    if (!CryptSetProvParam(hEncryption, PP_CHANGE_PIN, (BYTE *)&set_pin, 0))
    {
	printf("CryptSetProvParam(Encryption,PP_CHANGE_PIN) failed: %x\n", GetLastError());
	return FALSE;
    }
    return TRUE;
}

static BOOL DecryptContainer(HCRYPTPROV hEncryption, HCRYPTPROV hEncryptor)
{
    CRYPT_PIN_PARAM set_pin;
    DWORD hProvInt;
    if (!GetEncryptionHandle(hEncryptor, &hProvInt))
	return FALSE;
    set_pin.type = CRYPT_PIN_ENCRYPTION;
    set_pin.dest.prov = hProvInt;
    if (!CryptSetProvParam(hEncryption, PP_SET_PIN, (BYTE *)&set_pin, 0))
    {
	printf("CryptSetProvParam(Encryption,PP_SET_PIN) failed: %x\n", GetLastError());
	return FALSE;
    }
    return TRUE;
}

static BOOL CreateEncryptedContainer(HCRYPTPROV * phProv, CHAR * lpContainer, HCRYPTPROV hEncryptor)
{
    HCRYPTPROV hProvE;
    HCRYPTKEY hKeyE;
    if (!CryptAcquireContext(&hProvE, lpContainer, NULL, PROV_GOST_2001_DH, CRYPT_NEWKEYSET))
    {
	printf("CryptAcquireContext(Encryption) failed: %x\n", GetLastError());
	return FALSE;
    }
    if (!CryptSetProvParam(hProvE, PP_KEYEXCHANGE_PIN, (BYTE *)"", 0 ))
    {
	printf("CryptSetProvParam(PP_KEYEXCHANGE_PIN,Encryption) failed: %x\n", GetLastError());
	CryptReleaseContext(hProvE,0);
	return FALSE;
    }
    if (!CryptGenKey(hProvE, AT_KEYEXCHANGE, 0, &hKeyE))
    {
	printf("CryptGenKey(Encryption) failed: %x\n", GetLastError());
	CryptReleaseContext(hProvE,0);
	return FALSE;
    }
    CryptDestroyKey(hKeyE);
    hKeyE = 0;
    if (!EncryptContainer(hProvE,hEncryptor))
    {
	CryptReleaseContext(hProvE,0);
	return FALSE;
    }
    *phProv=hProvE;
    return TRUE;
}

static CHAR * GetEncryptorName (HCRYPTPROV hProv)
{
    DWORD dwSize;
    BYTE * buf = NULL;
    CRYPT_PIN_INFO * pi = NULL;
    CHAR * res;

    if (!CryptGetProvParam(hProv, PP_PIN_INFO, 0, &dwSize,0))
    {
	printf("CryptGetProvParam(PP_PIN_INFO,Encryption,size) failed: %x\n", GetLastError());
	return NULL;
    }
    buf = (BYTE *)malloc(dwSize);
    if (buf == NULL)
	return NULL;
    if (!CryptGetProvParam(hProv, PP_PIN_INFO, buf, &dwSize,0 ))
    {
	printf("CryptGetProvParam(PP_PIN_INFO,Encryption) failed: %x\n", GetLastError());
	return NULL;
    }
    pi = (CRYPT_PIN_INFO *)buf;
    if (pi->dest.encryption == NULL)
	res = NULL;
    else
	res = strdup(pi->dest.encryption);
    free(buf);
    return res;
}


int main(int argc, char ** argv)
{
    CHAR * lpEncryption = DEF_CONT_DISK;
    CHAR * lpEncryptor = DEF_CONT_KEY;
    CHAR * lpPin = "";
    CHAR * EncryptorName = NULL;
    HCRYPTPROV hProvE=0,hProvK=0;
    HCRYPTKEY hKeyE=0;
    HCRYPTHASH hHash=0;
    BYTE *pbBuffer = (BYTE *)"The data that is to be hashed and signed.";
    DWORD dwBufferLen = (DWORD)(strlen((char *)pbBuffer)+1);
    BYTE * pbSignature = NULL;
    DWORD dwSigLen = 0;
    switch (argc)
    {
	case 4: lpPin=argv[3];
	case 3: lpEncryptor=argv[2];
	case 2: lpEncryption=argv[1];
	case 1:
		break;
	default: printf("Invalid number of arguments\n");
		 exit(1);
    }
// Open the container to be Encrypted
    if (!CryptAcquireContext(&hProvE, lpEncryption, NULL, PROV_GOST_2001_DH, 0))
    {
	HRESULT Err=GetLastError();
	if ( Err == NTE_KEYSET_NOT_DEF )
	{
	    printf("Encrypted container not found. Creating...\n");
	    if (!OpenEncryptor(&hProvK, lpEncryptor, lpPin))
	    {
		printf("Cannot open Encryptor\n");
		exit(1);
	    }
	    if (!CreateEncryptedContainer(&hProvE, lpEncryption, hProvK))
	    {
		printf("Cannot create Encryption\n");
		exit(1);
	    }
	}
	else
	{
	    printf("CryptAcquireContext(Encryption) failed: %x\n", Err);
	    exit(1);
	}
    }
    else
    {
	EncryptorName = GetEncryptorName(hProvE);
	if (EncryptorName == NULL)
	{
	    printf("Container not encrypted\n");
	    exit(1);
	}
	printf("Encryptor name :%s\n", EncryptorName);
	if (!OpenEncryptor(&hProvK, EncryptorName, lpPin))
	{
		printf("Cannot open Encryptor\n");
		exit(1);
	}
	if (!DecryptContainer(hProvE,hProvK))
	{
		printf("Cannot decrypt container\n");
		exit(1);
	}
    }
// Open the resulting Container;
    if  (!CryptGetUserKey(hProvE, AT_KEYEXCHANGE, &hKeyE ))
    {
	    printf("CryptGetUserKey(Encryption,AT_KEYEXCHANGE) failed: %x\n", GetLastError());
	    exit(1);
    }
    printf("User Key opened successfully!\n");
    if (!CryptCreateHash(hProvE, CALG_GR3411, 0, 0, &hHash))
    {
	    printf("CryptCreateHash failed: %x\n", GetLastError());
	    exit(1);
    }
    if (!CryptHashData(hHash, pbBuffer,dwBufferLen,0))
    {
	    printf("CryptHashData failed: %x\n", GetLastError());
	    exit(1);
    }
    if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, NULL, &dwSigLen))
    {
	    printf("CryptSignHash (size) failed: %x\n", GetLastError());
	    exit(1);
    }
    pbSignature = (BYTE *)malloc(dwSigLen);
    if (pbSignature == NULL)
    {
	    printf("Not enough memory\n");
	    exit(1);
    }

    if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, pbSignature, &dwSigLen))
    {
	    printf("CryptSignHash (size) failed: %x\n", GetLastError());
	    exit(1);
    }
    printf("Hash signed\n");


    return 0;
}

