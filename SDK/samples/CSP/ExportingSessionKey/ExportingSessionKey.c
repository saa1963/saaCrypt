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
// В данном примере осуществляется экспортирование сессионного ключа
// при помощи структуры BLOB.
// Для корректной работы этой программы необходимо создать два 
// контейнера: получателя (с именем "Responder") и отправителя (c именем
// "Sender").
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProvResponder = 0;     // Дескриптор CSP получателя 
static HCRYPTPROV hProvSender = 0;        // Дескриптор CSP отправителя
static HCRYPTKEY hSessionKey = 0;         // Дескриптор сессионного ключа 
static HCRYPTKEY hAgreeKey = 0;           // Дескриптор ключа согласования
static HCRYPTKEY hResponderKey = 0;       // Дескриптор открытого ключа получателя
static HCRYPTKEY hSenderKey = 0;          // Дескриптор открытого ключа отправителя
static BYTE *pbKeyBlobSender = NULL;	   // Указатель на ключевой BLOB отправителя
static BYTE *pbKeyBlobResponder = NULL;   // Указатель на ключевой BLOB получателя
static BYTE *pbKeyBlobSimple = NULL;      // Указатель на сессионный ключевой BLOB 

int main(void)
{

    DWORD dwBlobLenSender;         // Длина ключевого BLOBа отправителя
    DWORD dwBlobLenResponder;      // Длина ключевого BLOBа получателя
    DWORD dwBlobLenSimple;         // Длина сессионного ключевого BLOBа

    //--------------------------------------------------------------------
    // На стороне получателя: отправляем открытый ключ отправителю
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Получение дескриптора контейнера получателя с именем "Responder", 
    // находящегося в рамках провайдера. 

    if(CryptAcquireContext(
	&hProvResponder, 
	"Responder", 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"Responder\" has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }
    //--------------------------------------------------------------------
    // Получение дескриптора открытого ключа получателя.

    if(CryptGetUserKey(
	hProvResponder, 
	AT_KEYEXCHANGE, 
	&hResponderKey)) 
    {
	printf("The responder public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    //--------------------------------------------------------------------
    // pbKeyBlobResponder - открытый ключ получателя, передаем его оправителю.
    //
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа открытого ключа и распределение памяти.

    if(CryptExportKey(
	hResponderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&dwBlobLenResponder)) 
    {
	printf("Size of the BLOB for the responder public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobResponder = (BYTE*)malloc(dwBlobLenResponder);

    if(pbKeyBlobResponder) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // Экспортирование открытого ключа получателя в BLOB открытого ключа.

    if(CryptExportKey(
	hResponderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlobResponder, 
	&dwBlobLenResponder))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }


    //--------------------------------------------------------------------
    // На стороне отправителя: отправляем открытый ключ получателю.
    //--------------------------------------------------------------------

    // Получение дескриптора контейнера отправителя с именем "Sender", 
    // находящегося в рамках провайдера. 

    if(CryptAcquireContext(
	&hProvSender, 
	"Sender", 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"Sender\" has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }
    //--------------------------------------------------------------------
    // Получение дескриптора открытого ключа отправителя.

    if(CryptGetUserKey(
	hProvSender, 
	AT_KEYEXCHANGE, 
	&hSenderKey)) 
    {
	printf("The sender public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    //--------------------------------------------------------------------
    // pbKeyBlobSender - открытый ключ отправителя, передаем его получателю.
    //
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа открытого ключа и распределение памяти.

    if(CryptExportKey(
	hSenderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&dwBlobLenSender)) 
    {
	printf("Size of the BLOB for the sender public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobSender = (BYTE*)malloc(dwBlobLenSender);

    if(pbKeyBlobSender) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // Экспортирование открытого ключа отправителя в BLOB открытого ключа.

    if(CryptExportKey(
	hSenderKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlobSender, 
	&dwBlobLenSender))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }
    //--------------------------------------------------------------------
    // Получение ключа согласования импортом открытого ключа получателя 
    // на открытом ключе отправителя.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlobResponder, 
	dwBlobLenResponder, 
	hSenderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    //--------------------------------------------------------------------
    // Генерация сессионного ключа.

    if(CryptGenKey(     
	hProvSender,      
	CALG_G28147,      
	CRYPT_EXPORTABLE, 
	&hSessionKey))
    {   
	printf("Original session key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // pbKeyBlobSimple - зашифрованный сессионный ключ, передаем его получателю.
    //
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа сессионного ключа и распределение памяти.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey, 
	SIMPLEBLOB, 
	0, 
	NULL,
	&dwBlobLenSimple)) 
    {
	printf("Size of the BLOB for the sender session key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobSimple = (BYTE*)malloc(dwBlobLenSimple);

    if(pbKeyBlobSimple) 
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }
    //--------------------------------------------------------------------
    // Шифрование ключа согласования на сессионном ключе.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey,
	SIMPLEBLOB, 
	0, 
	pbKeyBlobSimple, 
	&dwBlobLenSimple))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }


    //--------------------------------------------------------------------
    // На стороне получателя: получили зашифрованный сессионный ключ: 
    // pbKeyBlobSimple, получили открытый ключ: pbKeyBlobSender. 
    //--------------------------------------------------------------------

    // Получение ключа согласования импортом открытого ключа отправителя 
    // на открытом ключе получателя.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSender, 
	dwBlobLenSender, 
	hResponderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    // Получение сессионного ключа импортом зашифрованного сессионного ключа 
    // на ключе Agree.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSimple, 
	dwBlobLenSimple, 
	hAgreeKey, 
	0, 
	&hSessionKey))
    {
	printf("The session key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey session key.");
    }

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}                                               

void CleanUp(void)
{
    free(pbKeyBlobSender);
    free(pbKeyBlobResponder);
    free(pbKeyBlobSimple);

    // Уничтожение сессионного ключа.
    if(hSessionKey)
	CryptDestroyKey(hSessionKey);

    // Уничтожение ключа согласования.
    if(hAgreeKey)
	CryptDestroyKey(hAgreeKey);

    // Уничтожение дескриптора открытого ключа получателя.
    if(hResponderKey)
	CryptDestroyKey(hResponderKey);

    // Уничтожение дескриптора открытого ключа отправителя.
    if(hSenderKey)
	CryptDestroyKey(hSenderKey);

    // Освобождение дескриптора провайдера.
    if(hProvResponder) 
	CryptReleaseContext(hProvResponder, 0);
    if(hProvSender) 
	CryptReleaseContext(hProvSender, 0);
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

//END-OF-FILE
