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
#   include <wincryptex.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#   include <WinCryptEx.h>
#endif

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// В данном примере осуществляется хеширование строки, дублирование
// полученного хеша. Затем осуществляется хеширование дополнительных 
// данных при помощи оригинального и дублированного хеша.
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);
static void Get_And_Print_Hash(HCRYPTHASH hHash);

static HCRYPTPROV   hCryptProv = 0;
static HCRYPTHASH   hOriginalHash = 0;
static HCRYPTHASH   hDuplicateHash = 0;

int main(void)
{
    // Получение дескриптора контекста криптографического провайдера.
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
    // Создание объекта функции хеширования.

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
    // Хеширование байтовой строки.

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
    // Дублирование хеша.
    // Эта функция работает только в Windows 2000 и старше.

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
    // Хеширование "Some Data" с оригинальным хешем.

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
    // Хеширование "Other Data" с дублированным хешем.

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

// Определение функции Get_And_Print_Hash.
void Get_And_Print_Hash(HCRYPTHASH hOHash)
{
    BYTE         *pbHash;
    BYTE         *pbHashSize;
    DWORD        dwHashLen = sizeof(DWORD);
    DWORD        i;
    HCRYPTHASH   hHash=0;

    //--------------------------------------------------------------------
    // Дублирование установленного хеша.
    // Эта функция работает только в Windows 2000 и старше.
    // Хеш продублирован для того, чтобы не изменять исходный хеш.

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
	// Работает. Освобождение pbHashSize.
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
	// Печать значения хеша.
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
    // Уничтожение оригинального хеша.
    if (hOriginalHash)
	CryptDestroyHash(hOriginalHash);
    printf("The original hash has been destroyed. \n");

    // Уничтожение дублированного хеша.
    if (hDuplicateHash)
	CryptDestroyHash(hDuplicateHash);
    printf("The duplicate hash has been destroyed. \n");

    // Освобождение CSP.
    if(hCryptProv) 
	CryptReleaseContext(hCryptProv,0);
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
