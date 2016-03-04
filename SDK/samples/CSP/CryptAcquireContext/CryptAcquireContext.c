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
#   define CONTAINER "test"
#else
#   include <string.h>
#   define CONTAINER "\\\\.\\HDIMAGE\\test"
#endif
#include <WinCryptEx.h>

//#define MAX_CONTAINER_NAME_LEN 128 // определено в wincryptex.h
// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Пример кода, использующего CryptAcquireContext.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

static HCRYPTPROV hCryptProv = 0;
static HCRYPTKEY hKey = 0;

static void CleanUp(void);
static void HandleError(char *s);

int main(int argc, char *argv[])
{
    // Объявление и инициализация переменных.
    BYTE pbData[MAX_CONTAINER_NAME_LEN+1];
    DWORD cbData;
    BOOL bSilent = FALSE;

    // Флаг режима без окон
    if (argc > 1 && strcmp(argv[1], "-silent") == 0)
        bSilent = TRUE;
    printf("Mode: %s\n", bSilent ? "SILENT" : "NORMAL");

    // Получение дескриптора криптопровайдера.
    if(!CryptAcquireContext(
	&hCryptProv, 
	CONTAINER, 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	if(!(GetLastError() == 0x80090016 ))
	{
	    SetLastError(0x80090016);
	    HandleError("A general error occurred running CryptAcquireContext");
	}

	printf(" The default key container does not exist.\n");
	printf(" Create a default container and generate keys \n");
	printf(" Using the code in \n");
	printf("  Example C Program: Creating a Key Container and Generating Keys \n");
	printf("  Before running this program. \n");
	
	CleanUp();
	exit(0);
    }
    printf("CryptAcquireContext succeeded.\n");
    
    // Чтение имени CSP.
    cbData = MAX_CONTAINER_NAME_LEN;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_NAME, 
	NULL, 
	&cbData, 
	0))
    {
	HandleError("Error reading CSP name. \n");
    }

    if(cbData > MAX_CONTAINER_NAME_LEN)
	HandleError("CSP name length is too long");

    pbData[MAX_CONTAINER_NAME_LEN] = 0;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_NAME, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading CSP name. \n");
    }

    printf("CryptGetProvParam succeeded.\n");
    printf("Provider name: %s\n", pbData);

    // Чтение имени ключевого контейнера.
    cbData = MAX_CONTAINER_NAME_LEN;
    if(!CryptGetProvParam(
	hCryptProv, 
	PP_CONTAINER, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading key container name. \n");
    }
    if(cbData > MAX_CONTAINER_NAME_LEN)
	HandleError("Container name length is too long");

    if(!CryptGetProvParam(
	hCryptProv, 
	PP_CONTAINER, 
	pbData, 
	&cbData, 
	0))
    {
	HandleError("Error reading key container name. \n");
    }

    printf("CryptGetProvParam succeeded. \n");
    printf("Key Container name: %s\n", pbData);

    // Выполнение криптографических операций.

    //  Увеличение счетчика ссылок на криптопровайдер. Когда счетчик ссылок на 
    //  криптопровайдер становится больше 1, CryptReleaseContext уменьшает
    //  счетчик ссылок, но не освобождает криптопровайдер.

    if(!CryptContextAddRef(
	hCryptProv, 
	NULL, 
	0)) 
    {
	HandleError("Error during CryptContextAddRef!\n");
    }
    printf("CryptContextAddRef succeeded. \n");

    //  Счетчик ссылок на hCryptProv сейчас больше 1. Первый вызов 
    //  функции CryptReleaseContext не освободит дескриптор криптопровайдера. 

    //  Однократное освобождение контекста.
    if(!CryptReleaseContext(hCryptProv, 0))
    {
	HandleError("Error during CryptReleaseContext! #1");
    }
    printf("The first call to CryptReleaseContext succeeded. \n");

    // Повторное освобождение дескриптора криптопровайдера.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {
	HandleError("Error during CryptReleaseContext! #2");
    }
    printf("The second call to CryptReleaseContext succeeded. \n");

    // Получение дескриптора криптопровайдера и
    // создание ключевого контейнера с именем "KC1_test".
    if(!CryptAcquireContext(
	&hCryptProv, 
#ifdef _WIN32
	"\\\\.\\REGISTRY\\KC1_test",
#else
	"\\\\.\\HDIMAGE\\KC1_test",
#endif
	NULL, 
	PROV_GOST_2001_DH, 
	CRYPT_NEWKEYSET | (bSilent ? CRYPT_SILENT : 0))) 
    {
	HandleError("Error during CryptAcquireContext for a new key "
		    "container. A container with this name probably "
		    "already exists");
    }
    printf("CryptAcquireContext (CRYPT_NEWKEYSET) succeeded. \n");

    // begin Особенность КриптоПро CSP
    // Особенностью КриптоПро CSP является то, что криптопровайдер
    // физически не создает (записывает) контейнер, если в нем
    // нет закрытых ключей. Поэтому необходимо создать ключ.
    if(!CryptGenKey(
	hCryptProv,
	AT_SIGNATURE,
	0,
	&hKey)) 
    {	
	HandleError("Error occurred creating a signature key.\n"); 
    }
    printf("Created a signature key pair.\n");
    // end Особенность КриптоПро CSP

    // Выполнение криптографических операций.

    // Освобождение дескриптора криптопровайдера и ключевого контейнера KC1.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {	
	HandleError("Error during CryptReleaseContext!\n");
    }
    printf("CryptReleaseContext succeeded. \n");
    hCryptProv = 0;
    hKey = 0;

    // Получение дескриптора криптопровайдера, используя новый ключевой контейнер. 
    // Замечание: Этот ключевой контейнер будет пустым до тех пор, пока ключи
    // не будут явно созданы с помощью функции CryptGenKey.
    if(!CryptAcquireContext(
	&hCryptProv, 
	"KC1_test", 
	NULL, 
	PROV_GOST_2001_DH,
	0)) 
    {
	HandleError("Error during CryptAcquireContext!\n");
    }
    printf("Acquired the key set just created. \n");

    // Выполнение криптографических операций.

    // Освобождение дескриптора криптопровайдера и ключевого контейнера KC1.
    if(!CryptReleaseContext(hCryptProv, 0)) 
    {	
	HandleError("Error during CryptReleaseContext!\n");
    }
    printf("CryptReleaseContext succeeded. \n");
    hCryptProv = 0;

    // Для повторного испоьзования этого примера необходимо удалить созданный
    // ключей контейнер с именем "КС1". Контейнер можно удалить при помощи
    // контрольной панели КриптоПро CSP или следующим образом:
    if(!CryptAcquireContext(
	&hCryptProv, 
	"KC1_test", 
	NULL, 
	PROV_GOST_2001_DH,
	CRYPT_DELETEKEYSET)) 
    {
	HandleError("Error during CryptAcquireContext(CRYPT_DELETEKEYSET)!\n");
    }
    printf("CryptAcquireContext (CRYPT_DELETEKEYSET) succeeded. \n");

    CleanUp();
    return 0;
}

// Конец примера 
// (не следует удалять данный комментарий, он используется 
//  для автоматической сборки документации)

//--------------------------------------------------------------------
//  В этом примере используется функция HandleError, функция обработки
//  простых ошибок, для печати сообщения и выхода из программы. 
//  В большинстве приложений эта функция заменяется другой функцией, 
//  которая выводит более полное сообщение об ошибке.
//--------------------------------------------------------------------

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    CleanUp();
    if(!err) err = 1;
    exit(err);
}

void CleanUp(void)
{
    if(hKey) 
	CryptDestroyKey(hKey); 
    if(hCryptProv)
	CryptReleaseContext(hCryptProv, 0); 
}
