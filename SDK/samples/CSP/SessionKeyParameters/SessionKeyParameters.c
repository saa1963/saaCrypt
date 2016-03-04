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
// В данном примере осуществляется создание случайного сессионного 
// ключа шифрования и изменение одного из его параметров (вектора
// инициализации).
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
    // Получение дескриптора контекста криптографического параметра.

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
    //  Генерация восьми байтов случайных данных в pbRandomData.

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
    // Создание случайного сессионного ключа блочного шифрования.

    if(!CryptGenKey(
	hProv, 
	CALG_G28147, 
	CRYPT_EXPORTABLE, 
	&hKey)) 
    {
	HandleError("Error during CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // Чтение режима шифрования.

    dwCount = sizeof(DWORD);
    if(CryptGetKeyParam(
	hKey, 
	KP_MODE, 
	(BYTE*)&dwMode, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------------
	// Печать режима шифрования.

	printf("Default cipher mode: %d\n", dwMode);

    }
    else
    {
	HandleError("Error during CryptGetKeyParam.");
    }

    //---------------------------------------------------------------
    // Чтение вектора инициализации.

    //-------------------------------------------------------------
    //  Получение длины вектора инициализации.

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
    // Получение IV - вектора инициализации.

    if(CryptGetKeyParam(
	hKey, 
	KP_IV, 
	pbData, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------
	// Печать вектора инициализации.
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
    //  Переустановка вектора инициализации.

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
    // Чтение нового вектора инициализации.

    //-------------------------------------------------------------
    //  Получение длины нового вектора инициализации.

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
    // Получение IV - вектора инициализации.

    if(CryptGetKeyParam(
	hKey, 
	KP_IV, 
	pbData, 
	&dwCount, 
	0))
    {
	//---------------------------------------------------------
	// Печать вектора инициализации.
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
// Конец примера 
// (не следует удалять данный комментарий, он используется 
//  для автоматической сборки документации)

//------------------------------------------------------------------------------
//  В этом примере используется функция HandleError, функция обработки
//  простых ошибок, для печати сообщения об ошибке в стандартный файл 
//  ошибок (stderr) и выхода из программы. 
//  В большинстве приложений эта функция заменяется другой функцией, 
//  которая выводит более полное сообщение об ошибке.
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
