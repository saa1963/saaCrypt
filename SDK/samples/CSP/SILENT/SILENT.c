/*
* Copyright(C) 2000-2010 Проект ИОК
*
* Этот файл содержит информацию, являющуюся
* собственностью компании Крипто Про.
*
* Любая часть этого файла не может быть скопирована,
* исправлена, переведена на другие языки,
* локализована или модифицирована любым способом,
* откомпилирована, передана по сети с или на
* любую компьютерную систему без предварительного
* заключения соглашения с компанией Крипто Про.
*
* Программный код, содержащийся в этом файле, предназначен
* исключительно для целей обучения и не может быть использован
* для защиты информации.
*
* Компания Крипто-Про не несет никакой
* ответственности за функционирование этого кода.
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

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Пример кода для работы в режиме SILENT, без окон.
// В данном примере осуществляется подпись строки.
// Для корректной работы этой программы необходимо создать контейнер с 
// именем "Source" и с паролем "1234". При этом в контейнере должны 
// существовать оба ключа (ключ обмена и ключ подписи).
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// Дескриптор CSP
static HCRYPTKEY hKey = 0;		// Дескриптор ключа
static HCRYPTHASH hHash = 0;
static BYTE *pbHash = NULL;
static BYTE *pbKeyBlob = NULL;      
static BYTE *pbSignature = NULL;
#ifdef _WIN32        
#define CONTAINER "\\\\.\\Registry\\Source" 
#else
#define CONTAINER "\\\\.\\HDIMAGE\\test" 
#endif

int main(void)
{
    BYTE *pbBuffer = (BYTE *)"The data that is to be hashed and signed.";
    DWORD dwBufferLen = (DWORD)(strlen((char *)pbBuffer)+1);
    DWORD cbHash;
    DWORD dwSigLen;
    DWORD dwBlobLen;
    FILE *signature;

    // Получение дескриптора провайдера.
    if(CryptAcquireContext(
	&hProv, 
        CONTAINER,
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_SILENT)) 
    {
	printf("CryptAcquireContext succeeded.\n");
    }
    else
    {
	if(GetLastError() == 0x80090016)
	{
	    printf(" The key container with name \"Source\" does not exist.\n");
	    printf(" Create a container and generate keys \n");
	    exit(0);
	}
	else
	{
	    HandleError("A general error occurred in CryptAcquireContext.");
	}
    }

    //--------------------------------------------------------------------
    // Установка параметров в соответствии с паролем.

    if(CryptSetProvParam(
	hProv,
	PP_KEYEXCHANGE_PIN,
	(BYTE*)"1234",
	0))
    {
	printf("CryptSetProvParam succeeded.\n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    // Получение ключа пользователя.

    if(CryptGetUserKey(
	hProv,
	AT_KEYEXCHANGE,
	&hKey))
    {
	printf("CryptGetUserKey succeeded.\n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey.");
    }

    //--------------------------------------------------------------------
    // Экпорт открытого ключа. Здесь открытый ключ экспортируется в 
    // PUBLICKEYBLOB для того, чтобы получатель подписанного хеша мог 
    // проверить подпись. Этот BLOB может быть записан в файл и передан
    // другому пользователю.

    if(CryptExportKey(   
	hKey,    
	0,    
	PUBLICKEYBLOB,
	0,    
	NULL, 
	&dwBlobLen)) 
    {
	printf("Size of the BLOB for the public key determined. \n");
    }
    else
    {
	if(GetLastError() == 0x8010006B) /* SCARD_W_WRONG_CHV */
	{
	    printf(" The password of key container is differ from \"1234\".\n");
	    exit(0);
	}
	else
	{
	    HandleError("Error computing BLOB length.");
	}
    }

    // Распределение памяти под pbKeyBlob.
    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // Сам экспорт в ключевой BLOB.
    if(CryptExportKey(   
	hKey, 
	0,    
	PUBLICKEYBLOB,    
	0,    
	pbKeyBlob,    
	&dwBlobLen))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }
    //--------------------------------------------------------------------
    // Создание объекта функции хеширования.

    if(CryptCreateHash(
	hProv, 
	CALG_GR3411, 
	0, 
	0, 
	&hHash)) 
    {
	printf("Hash object created. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash.");
    }

    //--------------------------------------------------------------------
    // Передача параметра HP_OID объекта функции хеширования.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа и распределение памяти.

    if(CryptGetHashParam(hHash,
	HP_OID,
	NULL,
	&cbHash,
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbHash = (BYTE*)malloc(cbHash);
    if(!pbHash) 
	HandleError("Out of memory. \n");

    // Копирование параметра HP_OID в pbHash.

    if(CryptGetHashParam(hHash,
	HP_OID,
	pbHash,
	&cbHash,
	0))
    {
	printf("Parameters have been written to the pbHash. \n");
    }
    else
    {
	HandleError("Error during CryptGetHashParam.");
    }

    //--------------------------------------------------------------------
    // Вычисление криптографического хеша буфера.

    if(CryptHashData(
	hHash, 
	pbBuffer, 
	dwBufferLen, 
	0)) 
    {
	printf("The data buffer has been hashed.\n");
    }
    else
    {
	HandleError("Error during CryptHashData.");
    }
    //--------------------------------------------------------------------
    // Определение размера подписи и распределение памяти.

    dwSigLen = 0;
    if(CryptSignHash(
	hHash, 
	AT_SIGNATURE, 
	NULL, 
	0, 
	NULL, 
	&dwSigLen)) 
    {
	printf("Signature length %d found.\n",dwSigLen);
    }
    else
    {
	HandleError("Error during CryptSignHash.");
    }
    //--------------------------------------------------------------------
    // Распределение памяти под буфер подписи.

    pbSignature = (BYTE *)malloc(dwSigLen);
    if(!pbSignature)
	HandleError("Out of memory.");

    // Подпись объекта функции хеширования.
    if(CryptSignHash(
	hHash, 
	AT_SIGNATURE, 
	NULL, 
	0, 
	pbSignature, 
	&dwSigLen)) 
    {
	printf("pbSignature is the hash signature.\n");
    }
    else
    {
	HandleError("Error during CryptSignHash.");
    }

    //if(!fopen_s(&signature, "signature.txt", "w+b" ))
    if(!(signature = fopen("signature.txt", "w+b")))
	HandleError( "Problem opening the file\n" );
    fwrite (pbSignature, 1, dwSigLen, signature);
    fclose(signature);

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
} 

void CleanUp(void)
{
    if(pbHash)
	free(pbHash);
    if(pbKeyBlob)
	free(pbKeyBlob);
    if(pbSignature)
	free(pbSignature);

    // Уничтожение объекта функции хеширования.
    if(hHash) 
	CryptDestroyHash(hHash);

    printf("The hash object has been destroyed.\n");
    printf("The signature is created.\n\n");

    // Уничтожение дескриптора ключа пользователя.

    if(hKey)
	CryptDestroyKey(hKey);

    // Освобождение дескриптора провайдера.

    if(hProv) 
	CryptReleaseContext(hProv, 0);
}

// Конец примера 
// (не следует удалять данный комментарий, он используется 
//  для автоматической сборки документации)

//--------------------------------------------------------------------
//  В этом примере используется функция HandleError, функция обработки
//  простых ошибок, для печати сообщения и выхода из программы. 
//  В большинстве приложений эта функция заменяется другой функцией, 
//  которая выводит более полное сообщение об ошибке.

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp();
    if(!err) err = 1;
    exit(err);
}

