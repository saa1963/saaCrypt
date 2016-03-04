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
#   include <stdlib.h>
#   include <string.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ ������ ������������� ��������������� ��������� ����� � ����.
// ��� ��������� ������������ ��� ��������������� ��� ��������� ����� 
// �����������, ��� � ��������� ����� ����������. 
// ��� ���������� ��� ������ ����� ��������, ��� ������������ � 
// ������������� ����������� �����.
// � PUBLICKEYBLOB �� �������� ������������������ ���������� � �� ��������
// �����, ������� ������������� ������������ ����������� ��� ������ ���������
// �������. ���������� ����� ������������ � ����, ���� �� ���� � ����������.
// ��� ���������� ������ ���� ��������� ���������� ������� ���������
// � ������, ������������ ���������� �� ��������� ������.
// ��������� ����� ���� ������ ��� ������ ������� CreatingKeyContainer
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

static void WriteBlobToFile(FILE *file, char *szFileName, BYTE *pbData, DWORD cbData);
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// ���������� CSP 
static HCRYPTKEY hKey = 0;		// ���������� ��������� ����� ���������� 
static FILE *Public = NULL;		// ���� ��� �������� ��������� �����
static BYTE *pbKeyBlob = NULL;		// ��������� �� �������� BLOB 
static FILE *Cert = NULL;		// ���� ��� �������� �����������
static BYTE *pbCertBlob = NULL;	// ��������� BLOB �����������
static char *szFileName = NULL;        // ��� ����� ��� ��������

int main(int argc, char *argv[])
{
    DWORD cbKeyBlob;		// ����� ��������� BLOB� 
    DWORD cbCertBlob;		// ����� BLOB� �����������
    char  *szCont = argv[1];	// �������� ��� ����������

    if (argc < 2 || strchr(szCont, '\\'))
        HandleError("Short container name required.");

    // ��������� ����������� ���������� � ������ szCont, 
    // ������������ � ������ ����������. 
    if(CryptAcquireContext(
	&hProv, 
	szCont, 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"%s\" has been acquired.\n", szCont);
    }
    else
    {
        HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // ��������� ����������� ��������� �����.

    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {
	printf("The public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    // ������ ��������� ����� � �������� BLOB.
    // ����������� ������� BLOB� ��������� ����� � ������������� ������.
    if(CryptExportKey(
	hKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&cbKeyBlob)) 
    {
	printf("Size of the BLOB for the public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(cbKeyBlob);
    if(!pbKeyBlob) 
    {
	HandleError("Out of memory. \n");
    }

    // ��������������� ��������� ����� � BLOB ��������� �����.
    if(CryptExportKey(
	hKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlob, 
	&cbKeyBlob))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    szFileName = malloc((strlen(szCont) + 5) * sizeof(char));
    if(!szFileName) 
    {
	HandleError("Out of memory. \n");
    }
    strcpy(szFileName, szCont);
    strcat(szFileName, ".pub");
    WriteBlobToFile(Public, szFileName, pbKeyBlob, cbKeyBlob);

    // ��������� ����������� ����� �� ���������� (���� �� ����)
    if(CryptGetKeyParam(
	hKey,
	KP_CERTIFICATE,
	NULL,
	&cbCertBlob,
	0))
    {
	pbCertBlob = malloc(cbCertBlob);
	if(!pbCertBlob) 
	{
	    HandleError("Out of memory. \n");
	}
	if(CryptGetKeyParam(
	    hKey,
	    KP_CERTIFICATE,
	    pbCertBlob,
	    &cbCertBlob,
	    0))
	{
	    printf("Got certificate from container.\n");
	}
	else
	{
	    HandleError("Error during CryptGetKeyParam.");
	}
	strcpy(szFileName, szCont);
	strcat(szFileName, ".cer");
	WriteBlobToFile(Cert, szFileName, pbCertBlob, cbCertBlob);
    }
    else
    {
	printf("No certificate in container.\n");
    }

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}

void WriteBlobToFile(FILE *file, char *szFileName, BYTE *pbData, DWORD cbData)
{
    // �������� ����� �� ������ � ���� BLOB-�
    //if(!fopen_s(&file, szFileName, "r+b" ))
    if((file = fopen(szFileName, "wb")))
    {
	printf("The file '%s' was opened\n", szFileName);
    }
    else
    {
	HandleError("Problem opening the file\n");
    }

    // ������ BLOB-� � ����
    if(fwrite(pbData, 1, cbData, file))
    {
	printf("The public key was written to the '%s'\n", szFileName);
    }
    else
    {
	HandleError("The public key can not be written to file.\n");
    }
}

void CleanUp(void)
{
    if(Public)
        fclose (Public);

    if(Cert)
	fclose (Cert);

    // ������������ ������, ������������ �������� BLOB��.
    free(pbKeyBlob);

    // ������������ ������, ������������ BLOB�� �����������.
    free(pbCertBlob);

    // ����������� ������ ��� ��� �����
    free(szFileName);

    // ����������� ����������� ��������� �����.
    if(hKey)
	CryptDestroyKey(hKey);

    // ������������ ����������� ����������.
    if(hProv) 
	CryptReleaseContext(hProv, 0);
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


