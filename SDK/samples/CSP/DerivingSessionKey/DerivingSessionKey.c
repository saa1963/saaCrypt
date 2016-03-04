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
// Пример создания сессионного ключа, основанного на хеше, рассчитанного 
// из пароля пользователя. Пароль вводится в процессе работы программы.
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hCryptProv = 0;
static HCRYPTKEY hKey = 0;
static HCRYPTHASH hHash = 0;

int main(void)
{
    //--------------------------------------------------------------------
    //  Пароль из которого будет получен сессионный ключ.
    
    CHAR szPassword[] = "123456\0";
    DWORD dwLength;


    dwLength = (DWORD)strlen(szPassword);

    //--------------------------------------------------------------------
    // Получение дескриптора контекста криптографического провайдера.

    if(CryptAcquireContext(
	&hCryptProv, 
	NULL, 
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_VERIFYCONTEXT)) 
    {
	printf("A context has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext!");
    }
    //--------------------------------------------------------------------
    // Создание пустого объекта хеширования.

    if(CryptCreateHash(
	hCryptProv, 
	CALG_GR3411, 
	0, 
	0, 
	&hHash)) 
    {
	printf("An empty hash object has been created. \n");
    }
    else
    {
	HandleError("Error during CryptCreateHash!");
    }
    //--------------------------------------------------------------------
    // Хеширование строки пароля.

    if(CryptHashData(
	hHash, 
	(BYTE *)szPassword, 
	dwLength, 
	0)) 
    {
	printf("The password has been hashed. \n");
    }
    else
    {
	HandleError("Error during CryptHashData!");
    }
    //--------------------------------------------------------------------
    // Создание сессионного ключа, основанного на хеше, полученного из пароля.

    if(CryptDeriveKey(
	hCryptProv, 
	CALG_G28147, 
	hHash, 
	CRYPT_EXPORTABLE, 
	&hKey)) 
    {
	printf("The key has been derived. \n");
    }
    else
    {
	HandleError("Error during CryptDeriveKey!");
    }

    CleanUp();

    printf("The program to derive a key completed without error. \n");

    return 0;
} 


void CleanUp(void)
{
    if(hHash) 
	CryptDestroyHash(hHash);

    // Уничтожение сессионного ключа.
    if(hKey) 
	CryptDestroyKey(hKey);

    // Освобождение дескриптора провайдера.
    if(hCryptProv) 
	CryptReleaseContext(hCryptProv, 0);
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
