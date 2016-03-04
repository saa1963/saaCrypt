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
	HandleError("Error during CryptAcquireContext!\n");
    }

    // Генерация ключа.
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

    // Дублирование ключа.
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

    //  Установка дополнительных параметров исходного ключа.
    //  В первую очередь, установка режима шифрования.
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

    // Генерация случайного вектора инициализации.
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

    // Установка вектора инициализации.
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

// Конец примера 
// (не следует удалять данный комментарий, он используется 
//  для автоматической сборки документации)

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
