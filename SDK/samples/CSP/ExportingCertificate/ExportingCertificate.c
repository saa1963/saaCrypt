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
#   include <tchar.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#   include "reader/tchar.h"
#endif
#include <WinCryptEx.h>

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// В данном примере осуществляется экспортирование сессионного ключа
// при помощи сертификатов получателя и отправителя.
// Для корректной работы этой программы необходимо создать два 
// сертификата и добавить их в хранилище сертификатов MY: получателя 
// (с именем, содержащим "Responder") и отправителя (c именем, содержащим 
// "Sender").
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(BOOL);

static HCERTSTORE hStoreHandle = 0;	    // Дескриптор хранилища сертификатов
static PCCERT_CONTEXT pCertSender = NULL;  // Контекст сертификата отправителя
static PCCERT_CONTEXT pCertResponder = NULL;	// Контекст сертификата получателя
static HCRYPTPROV hProvResponder = 0;	    // Дескриптор CSP получателя 
static HCRYPTPROV hProvSender = 0;         // Дескриптор CSP отправителя
static HCRYPTKEY hSenderKey = 0;	    // Дескриптор открытого ключа отправителя
static HCRYPTKEY hResponderKey = 0;	    // Дескриптор открытого ключа получателя
static HCRYPTKEY hSessionKey = 0;	    // Дескриптор сессионного ключа 
static HCRYPTKEY hAgreeKey = 0;	    // Дескриптор ключа согласования

static BYTE *pbKeyBlobSimple = NULL;       // Указатель на сессионный ключевой BLOB 
static BYTE *pbKeyBlobSender = NULL;       // Указатель на ключевой BLOB отправителя
static BYTE *pbKeyBlobResponder = NULL;    // Указатель на ключевой BLOB получателя

int main(void)
{
    DWORD dwBlobLenSimple;	    // Длина сессионного ключевого BLOBа
    DWORD dwBlobLenSender;	    // Длина ключевого BLOBа отправителя
    DWORD dwBlobLenResponder;	    // Длина ключевого BLOBа получателя
    DWORD dwKeySpecSender;
    DWORD dwKeySpecResponder;

    // На стороне отправителя:

    // Открытие системного хранилища сертификатов.
    hStoreHandle = CertOpenSystemStore(0, "MY");

    if(hStoreHandle)
    {
	printf("The MY store is open. \n");
    }
    else
    {
	HandleError( "Error getting store handle.");
    }

    //--------------------------------------------------------------------
    // Получение контекста сертифика, в названии которого содержится "Sender", 
    // находящегося в хранилище сертификатов "MY".

    pCertSender = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Sender"),
	NULL);

    if(pCertSender)
    {
	printf("The \" Sender \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // Получение дескриптора CSP, включая доступ к связанному с ним ключевому
    // контейнеру для контекста сертификата pCertSender.

    if(CryptAcquireCertificatePrivateKey(
	pCertSender,
	0,
	NULL,
	&hProvSender,
	&dwKeySpecSender,
	NULL))
    {
	printf("The CSP handle has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireCertificatePrivateKey.");
    }

    //--------------------------------------------------------------------
    // Поиск сертифика, в названии которого содержится "Responder", 
    // находящегося в хранилище сертификатов "MY".

    pCertResponder = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Responder"),
	NULL);

    if(pCertResponder)
    {
	printf("The \" Responder \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // Получение дескриптора открытого ключа получателя.

    if(CryptImportPublicKeyInfoEx( 
	hProvSender,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	&(pCertResponder->pCertInfo->SubjectPublicKeyInfo),
	0,
	0,
	NULL,
	&hResponderKey))
    {
	printf("The responder public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptImportPublicKeyInfoEx public key.");
    }

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
    // Получение дескриптора закрытого ключа отправителя.

    if(CryptGetUserKey(
	hProvSender, 
	dwKeySpecSender, 
	&hSenderKey)) 
    {
	printf("The sender private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    //--------------------------------------------------------------------
    // Получение ключа согласования импортом открытого ключа получателя из BLOBа 
    // на закрытом ключе отправителя.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlobResponder, 
	dwBlobLenResponder, 
	hSenderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
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
    // pbKeyBlobSimple - зашифрованный сессионный ключ, передаем его получателю
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
    // Шифрование сессионного ключа на ключе Agree.

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
    // Закрытие системного хранилища сертификатов.

    if(CertCloseStore(
	hStoreHandle, 
	0))
    {
	printf("The MY store is closed. \n");
	hStoreHandle=0;
    }
    else
    {
	HandleError( "Error closing store handle.");
    }

    CleanUp(FALSE);


    //--------------------------------------------------------------------
    // На стороне получателя: получили зашифрованный сессионный ключ
    // pbKeyBlobSimple.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Открытие системного хранилища сертификатов.

    hStoreHandle = CertOpenSystemStore(0, "MY");
    if(hStoreHandle)
    {
	printf("The MY store is open. \n");
    }
    else
    {
	HandleError( "Error getting store handle.");
    }

    //--------------------------------------------------------------------
    // Получение контекста сертифика, в названии которого содержится "Responder", 
    // находящегося в хранилище сертификатов "MY".

    pCertResponder = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Responder"),
	NULL);

    if(pCertResponder)
    {
	printf("The \" Responder \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }

    //--------------------------------------------------------------------
    // Получение дескриптора CSP, включая доступ к связанному с ним ключевому
    // контейнеру для контекста сертификата pCertResponder.

    if(CryptAcquireCertificatePrivateKey(
	pCertResponder,
	0,
	NULL,
	&hProvResponder,
	&dwKeySpecResponder,
	NULL))
    {
	printf("The CSP handle has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireCertificatePrivateKey.");
    }

    //--------------------------------------------------------------------
    // Поиск сертифика, в названии которого содержится "Sender", 
    // находящегося в хранилище сертификатов "MY".

    pCertSender = CertFindCertificateInStore(
	hStoreHandle,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	0,
	CERT_FIND_SUBJECT_STR,
	_TEXT("Sender"),
	NULL);

    if(pCertSender)
    {
	printf("The \" Sender \" certificate is found. \n");
    }
    else
    {
	HandleError( "Error finding certificate.");
    }
    //--------------------------------------------------------------------
    // Получение дескриптора открытого ключа отправителя.

    if(CryptImportPublicKeyInfoEx( 
	hProvResponder,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	&(pCertSender->pCertInfo->SubjectPublicKeyInfo),
	0,
	0,
	NULL,
	&hSenderKey))
    {
	printf("The sender public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptImportPublicKeyInfoEx public key.");
    }

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
    // Получение дескриптора закрытого ключа получателя.

    if(CryptGetUserKey(
	hProvResponder, 
	dwKeySpecResponder, 
	&hResponderKey)) 
    {
	printf("The responder private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    //--------------------------------------------------------------------
    // Получение ключа согласования импортом открытого ключа отправителя из BLOBа 
    // на закрытом ключе получателя.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlobSender, 
	dwBlobLenSender, 
	hResponderKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
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


    CleanUp(TRUE);

    printf("The program ran to completion without error. \n");
    return 0;
}

void CleanUp(BOOL all)
{

    if(pbKeyBlobResponder)
    {
	    free(pbKeyBlobResponder);
	    pbKeyBlobResponder=NULL;
    }
    if(pbKeyBlobSimple && all)
    {
	    free(pbKeyBlobSimple);
	    pbKeyBlobSimple=NULL;
    }
    if(pbKeyBlobSender)
    {
	    free(pbKeyBlobSender);
	    pbKeyBlobSender=NULL;
    }

    if(hStoreHandle)
    {
	    CertCloseStore(hStoreHandle, 0);
	    hStoreHandle=0;
    }

    // Освобождение дескриптора провайдера.
    if(hProvSender) 
    {
	CryptReleaseContext(hProvSender, 0);
	hProvSender=0;
    }

    // Уничтожение дескриптора закрытого ключа получателя.
    if(hResponderKey)
    {
	CryptDestroyKey(hResponderKey);
	hResponderKey=0;
    }

    // Уничтожение дескриптора открытого ключа отправителя.
    if(hSenderKey)
    {
	CryptDestroyKey(hSenderKey);
	hSenderKey=0;
    }

    // Уничтожение сессионного ключа.
    if(hSessionKey)
    {
	CryptDestroyKey(hSessionKey);
	hSessionKey=0;
    }

    // Уничтожение ключа согласования.
    if(hAgreeKey)
    {
	CryptDestroyKey(hAgreeKey);
	hAgreeKey=0;
    }

    // Освобождение дескриптора провайдера.
    if(hProvResponder) 
    {
	CryptReleaseContext(hProvResponder, 0);
	hProvResponder=0;
    }

    // Освобождение контекста сертификата. 
    if(pCertSender)
    {
	CertFreeCertificateContext(pCertSender);
	pCertSender=NULL;
    }

    if(pCertResponder)
    {
	CertFreeCertificateContext(pCertResponder);
	pCertResponder=NULL;
    }
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
    CleanUp(TRUE);
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE
