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
  #include <windows.h>
  #include <wincrypt.h>
  #include <wincryptex.h>
#else
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <WinCryptEx.h>
#endif	// _WIN32
#define TYPE_DER (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)
//--------------------------------------------------------------------
// Пример кода для подписи и проверки подписи данных 
// сообщения при помощи функции Crypt.

// Для функционирования данного кода необходим контейнер с ключом 
// AT_KEYEXCHANGE с именем Test(Unix) или контейнер по умолчанию(WIN),
// Ключ можно создать используя пример CryptAcquireContext.
//--------------------------------------------------------------------


static void HandleError(const char *s);
//--------------------------------------------------------------------
// В этой программе используется функция GetRecipientCert, объявленная 
// здесь и описанная после main.

static PCCERT_CONTEXT GetRecipientCert(HCERTSTORE hCertStore);

//--------------------------------------------------------------------
static void do_low_sign();
static BYTE* pbContent = (BYTE*) "Security is our business."; //сообщение
static DWORD cbContent = (DWORD)(strlen((char *)pbContent)+1);//длина сообщения
static DWORD cbEncodedBlob;
static BYTE  *pbEncodedBlob = NULL;
static void do_low_verify();

int main(void)
{
    do_low_sign();
    printf("Do_low_sign DONE \n\n");
    do_low_verify();
    printf("Do_low_verify DONE \n\n");
    return 0;
}//end of main

void do_low_sign() {
    HCRYPTPROV      hCryptProv = 0;         /* Дескриптор провайдера*/
    HCERTSTORE	    hStoreHandle=0;		//Дeскриптор сторе
    PCCERT_CONTEXT  pRecipientCert = NULL;       /* Сертификат, используемый для формирования ЭЦП*/
    DWORD           keytype = AT_KEYEXCHANGE;            /* Тип ключа (возвращается)*/
    HCRYPTMSG       hMsg = 0;               /* Дескриптор сообщения*/
    CRYPT_ALGORITHM_IDENTIFIER  HashAlgorithm;  /* Идентификатор алгоритма хэширования*/
    DWORD                       HashAlgSize;
    CMSG_SIGNER_ENCODE_INFO     SignerEncodeInfo;   /* Структура, описывающая отправителя*/
    CMSG_SIGNER_ENCODE_INFO     SignerEncodeInfoArray[1]; /* Массив структур, описывающих отправителя*/
    CERT_BLOB                   SignerCertBlob;
    CERT_BLOB                   SignerCertBlobArray[1];
    CMSG_SIGNED_ENCODE_INFO     SignedMsgEncodeInfo;    /* Структура, описывающая подписанное сообщение*/
    BOOL                        bReleaseContext;
    DWORD                       flags = 0;
//--------------------------------------------------------------------
//  Начало обработки данных.

    printf("About to begin with the message %s.\n",pbContent);
    printf("The message length is %d bytes. \n", cbContent);

//--------------------------------------------------------------------
// Открытие системного хранилища сертификатов.

    hStoreHandle = CertOpenSystemStore( 0, "My");

    if(hStoreHandle)
    {
        printf("The MY store is open. \n");
    }
    else
    {
        HandleError( "Error getting store handle.");
    }
//--------------------------------------------------------------------
// Получение указателя на сертификат издателя с помощью
// функции GetRecipientCert. 

    pRecipientCert = GetRecipientCert(hStoreHandle);

    if(pRecipientCert)
    {
        printf("A recipient's certificate has been acquired. \n");
    }
    else
    {
        printf("No certificate with a CERT_KEY_CONTEXT_PROP_ID \n");
        printf("property and an AT_KEYEXCHANGE private key available. \n");
        printf("While the message could be sign, in this case, \n");
        printf("it could not be veryfy in this program. \n");
        printf("For more information, see the documentation  \n");
        HandleError( "No Certificate with AT_KEYEXCHANGE key in store.");
    }
//--------------------------------------------------------------------
// Получение закрытого ключа
    if (!CryptAcquireCertificatePrivateKey(pRecipientCert,
            0,
            NULL,
            &hCryptProv,
            &keytype,
            &bReleaseContext)) 
    {
        HandleError( "Cannot acquire the certificate private key");
    }

//--------------------------------------------------------------------
// Инициализация структуры идентификатора алгоритма.

    HashAlgSize = sizeof(HashAlgorithm);

//--------------------------------------------------------------------
// Инициализация структуры с нулем.

    memset(&HashAlgorithm, 0, HashAlgSize);

//--------------------------------------------------------------------
// Установка необходимого элемента.

    HashAlgorithm.pszObjId = (LPSTR)szOID_CP_GOST_R3411;

/*--------------------------------------------------------------------*/
    /* Инициализируем структуру CMSG_SIGNER_ENCODE_INFO*/


    memset(&SignerEncodeInfo, 0, sizeof(CMSG_SIGNER_ENCODE_INFO));
    SignerEncodeInfo.cbSize = sizeof(CMSG_SIGNER_ENCODE_INFO);
    SignerEncodeInfo.pCertInfo = pRecipientCert->pCertInfo;
    SignerEncodeInfo.hCryptProv = hCryptProv;
    SignerEncodeInfo.dwKeySpec = keytype;
    SignerEncodeInfo.HashAlgorithm = HashAlgorithm;
    SignerEncodeInfo.pvHashAuxInfo = NULL;

/*--------------------------------------------------------------------*/
/* Создадим массив отправителей. Сейчас только из одного.*/

    SignerEncodeInfoArray[0] = SignerEncodeInfo;

/*--------------------------------------------------------------------*/
/* Инициализируем структуру CMSG_SIGNED_ENCODE_INFO*/

    SignerCertBlob.cbData = pRecipientCert->cbCertEncoded;
    SignerCertBlob.pbData = pRecipientCert->pbCertEncoded;

/*--------------------------------------------------------------------*/
/* Инициализируем структуру массив структур CertBlob.*/

    SignerCertBlobArray[0] = SignerCertBlob;
    memset(&SignedMsgEncodeInfo, 0, sizeof(CMSG_SIGNED_ENCODE_INFO));
    SignedMsgEncodeInfo.cbSize = sizeof(CMSG_SIGNED_ENCODE_INFO);
    SignedMsgEncodeInfo.cSigners = 1;
    SignedMsgEncodeInfo.rgSigners = SignerEncodeInfoArray;
    SignedMsgEncodeInfo.cCertEncoded = 0;
    SignedMsgEncodeInfo.rgCertEncoded = NULL;
    SignedMsgEncodeInfo.rgCrlEncoded = NULL;

/*--------------------------------------------------------------------*/
/* Создадим дескриптор сообщения*/
    hMsg = CryptMsgOpenToEncode(
                TYPE_DER,               /* Encoding type*/
                flags,                  /* Flags (CMSG_DETACHED_FLAG )*/
                CMSG_SIGNED,            /* Message type*/
                &SignedMsgEncodeInfo,   /* Pointer to structure*/
                NULL,                   /* Inner content object ID*/
                NULL);                  /* Stream information (not used)*/
        if(hMsg) {
        printf("The message to be encoded has been opened. \n");
    } else {
                HandleError("OpenToEncode failed");
    }
/*--------------------------------------------------------------------*/
/* Поместим в сообщение подписываемые данные*/

    if(CryptMsgUpdate(
        hMsg,               /* Handle to the message*/
        pbContent,            /* Pointer to the content*/
        cbContent,     /* Size of the content*/
                TRUE)) {            /* Last call*/
                printf("Content has been added to the encoded message. \n");
    } else {
                HandleError("MsgUpdate failed");
    }
/*--------------------------------------------------------------------*/
/* Определим длину подписанного сообщения*/
    if(CryptMsgGetParam(
                hMsg,                      /* Handle to the message*/
                CMSG_CONTENT_PARAM,        /* Parameter type*/
                0,                         /* Index*/
                NULL,             /* Pointer to the blob*/
                &cbEncodedBlob)) {          /* Size of the blob*/
                printf("The length of the data has been calculated.\n");
    } else {
                HandleError("MsgGetParam failed");
    }

/*--------------------------------------------------------------------*/
/* Резервируем память, требуемой длины*/

    pbEncodedBlob = (BYTE *)malloc(cbEncodedBlob);
    if (!pbEncodedBlob)
                HandleError("Memory allocation failed");

/*--------------------------------------------------------------------*/
/* Вернем подписанное сообщение*/

    if(CryptMsgGetParam(
                hMsg,                      /* Handle to the message*/
                CMSG_CONTENT_PARAM,        /* Parameter type*/
                0,                         /* Index*/
                pbEncodedBlob,             /* Pointer to the blob*/
                &cbEncodedBlob)) {          /* Size of the blob*/
                printf("Message encoded successfully. \n");
    } else {
                HandleError("MsgGetParam failed");
    }

//освобождение памяти

    if(hMsg)
	CryptMsgClose(hMsg);
    if(hCryptProv && bReleaseContext)
	CryptReleaseContext(hCryptProv,0);

}//end of do_low_sign

void do_low_verify() {
    HCRYPTPROV          hCryptProv = 0;         /* Дескриптор провайдера*/
    PCCERT_CONTEXT      pRecipientCert = NULL;       /* Сертификат, используемый для проверки ЭЦП*/
    BYTE                *mem_tbs = pbEncodedBlob;
    size_t              mem_len = cbEncodedBlob;
    HCRYPTMSG           hMsg = 0;               /* Дескриптор сообщения*/
    DWORD               cbDecoded;
    BYTE                *pbDecoded = NULL;
    PCERT_INFO          pSignerCertInfo = NULL;
    PCCERT_CONTEXT      pSignerCertContext = NULL;
    PCERT_INFO          pSignerCertificateInfo = NULL;
    HCERTSTORE          hStoreHandle = NULL;
    DWORD		flags=0;
    BOOL		bResult;	

    if(CryptAcquireContext(
	   &hCryptProv,         // Адрес возврашаемого дескриптора.
	   NULL,// Используется контейнер Test
	   NULL,                // Используется провайдер по умолчанию.
	   75,   // Необходимо для зашифрования и подписи.
	   CRYPT_VERIFYCONTEXT))               // Никакие флаги не нужны.
    {
        printf("A CSP has been acquired. \n");
    }
    else
    {
        HandleError("Cryptographic context could not be acquired.");
    }
//--------------------------------------------------------------------
// Открытие системного хранилища сертификатов.

    hStoreHandle = CertOpenSystemStore( 0, "MY");

    if(hStoreHandle)
    {
        printf("The MY store is open. \n");
    }
    else
    {
        HandleError( "Error getting store handle.");
    }
//--------------------------------------------------------------------
// Получение указателя на сертификат издателя с помощью
// функции GetRecipientCert.

    pRecipientCert = GetRecipientCert(hStoreHandle);

    if(pRecipientCert)
    {
        printf("A recipient's certificate has been acquired. \n");
    }
    else
    {
        printf("No certificate with a CERT_KEY_CONTEXT_PROP_ID \n");
        printf("property and an AT_KEYEXCHANGE private key available. \n");
        printf("While the message could be sign, in this case, \n");
        printf("it could not be veryfy in this program. \n");
        printf("For more information, see the documentation \n");
        HandleError( "No Certificate with AT_KEYEXCHANGE key in store.");
    }

/*--------------------------------------------------------------------*/
/* Откроем сообщение для декодирования*/

    hMsg = CryptMsgOpenToDecode(
	TYPE_DER,           /* Encoding type.*/
	flags,              /* Flags.*/
	0,                  /* Use the default message type.*/
	hCryptProv,         /* Cryptographic provider.*/
	NULL,               /* Recipient information.*/
	NULL);              /* Stream information.*/
    if (hMsg)
        printf("The message to decode is open. \n");
    else {
        HandleError("OpenToDecode failed");
    }

/*--------------------------------------------------------------------*/
/* Поместим в сообщение проверяемые данные*/
    bResult = CryptMsgUpdate(
        hMsg,           /* Handle to the message*/
        mem_tbs,        /* Pointer to the encoded blob*/
        (DWORD)mem_len,        /* Size of the encoded blob*/
        TRUE);          /* Last call*/
    if (bResult)
        printf("The encoded blob has been added to the message. \n");
    else {
            HandleError("Decode MsgUpdate failed");
    }
/*--------------------------------------------------------------------*/
/* Определим длину подписанных данных*/

    bResult = CryptMsgGetParam(
	hMsg,                  /* Handle to the message*/
	CMSG_CONTENT_PARAM,    /* Parameter type*/
	0,                     /* Signed Index*/
	NULL,                  /* Address for returned info*/
	&cbDecoded);           /* Size of the returned info*/
    if (bResult)
	printf("The message parameter (CMSG_CONTENT_PARAM) has been acquired. Message size: %u\n", cbDecoded);
    else {
	HandleError("Decode CMSG_CONTENT_PARAM failed");
    }

/*--------------------------------------------------------------------*/
/* Резервируем память*/
    pbDecoded = (BYTE *) malloc(cbDecoded);
    if (!pbDecoded)
	HandleError("Decode memory allocation failed");
/*--------------------------------------------------------------------*/
/* Вернем подписанные данные*/

    bResult = CryptMsgGetParam(
	hMsg,                 /* Handle to the message*/
	CMSG_CONTENT_PARAM,   /* Parameter type*/
	0,                    /* Signer Index*/
	pbDecoded,            /* Address for returned info*/
	&cbDecoded);          /* Size of the returned info*/
    if (bResult)
	printf("The message param (CMSG_CONTENT_PARAM) returned. Length is %lu.\n", (unsigned long)cbDecoded);
    else {
	HandleError("Decode CMSG_CONTENT_PARAM #2 failed");
    }

/*--------------------------------------------------------------------*/
/* Проверка ЭЦП*/
/* Сначала определим информация CERT_INFO об отправителе.*/

/*--------------------------------------------------------------------*/
/* Если сертификат задан */
/* создадим справочник в памяти с этим сертификатом.*/
/* Это сделано только для того, чтобы затем вернуть сертификат функцией */
/* CertGetSubjectCertificateFromStore, которая также используется, если*/
/* сертификат отправителя находится в самом сообщении.*/
    if (pRecipientCert) {
	hStoreHandle = CertOpenStore(CERT_STORE_PROV_MEMORY, TYPE_DER, 0, CERT_STORE_CREATE_NEW_FLAG,NULL);
	if (!hStoreHandle)
	    HandleError("Cannot create temporary store in memory.");
	/* Добавим сертификат в справочник*/
	if (pRecipientCert) {
	    bResult = CertAddCertificateContextToStore(hStoreHandle, pRecipientCert, CERT_STORE_ADD_ALWAYS, NULL);
	    pSignerCertInfo = pRecipientCert->pCertInfo;
	} else
	    bResult = 0;
	if (!bResult) {
	    HandleError("Cannot add user certificate to store.");
	}
    }

/*--------------------------------------------------------------------*/
/* Найдем сертификат отправителя в справочнике*/

    pSignerCertContext = CertGetSubjectCertificateFromStore(
	hStoreHandle,       /* Handle to store*/
	TYPE_DER,           /* Encoding type*/
	pSignerCertInfo);
    if(pSignerCertContext) {  /* Pointer to retrieved CERT_CONTEXT*/
	printf("A signer certificate has been retrieved.");
    } else {
	HandleError("Verify GetSubjectCert failed");
    }
 
/*--------------------------------------------------------------------*/
/* Используя структуру CERT_INFO проверяем ЭЦП сообщения*/
    pSignerCertificateInfo = pSignerCertContext->pCertInfo;
    bResult = CryptMsgControl(
	hMsg,                       /* Handle to the message*/
	0,                          /* Flags*/
	CMSG_CTRL_VERIFY_SIGNATURE, /* Control type*/
	pSignerCertificateInfo);    /* Pointer to the CERT_INFO*/
    if(bResult) {
	printf("\nSignature was VERIFIED.\n");
    } else {
	printf("\nThe signature was NOT VERIFIED.\n");
    }
//освобождение памяти
    CertFreeCertificateContext(pRecipientCert);
    if(CertCloseStore(
	   hStoreHandle,
	   CERT_CLOSE_STORE_CHECK_FLAG))
	{
	    printf("The MY store was closed without incident. \n");
	}
    else
	{
	    printf("Store closed after encryption -- \n"
		   "but not all certificates or CRLs were freed. \n");
	}
    if(hMsg)
	CryptMsgClose(hMsg);
    if(hCryptProv)
	CryptReleaseContext(hCryptProv,0);
}//end of do_low_verify

//  В этом примере используется функция HandleError, функция обработки
//  простых ошибок, для печати сообщения об ошибке в стандартный файл 
//  ошибок (stderr) и выхода из программы. 
//  В большинстве приложений эта функция заменяется другой функцией, 
//  которая выводит более полное сообщение об ошибке.

void HandleError(const char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    if(!err) err = 1;
    exit(err);
} // Конец опредления HandleError

//--------------------------------------------------------------------
// GetRecipientCert перечисляет сертификаты в хранилище и находит
// первый сертификат, обладающий ключем AT_EXCHANGE. Если сертификат
// сертификат найден, то возвращается указатель на него.  

PCCERT_CONTEXT GetRecipientCert(HCERTSTORE hCertStore) 
//-------------------------------------------------------------------- 
// Устанавлеваемый параметр: 
// hCertStore, дескриптор хранилища, в котором будет осуществлен поиск. 
{ 
//-------------------------------------------------------------------- 
// Объявление и инициализация локальных переменных. 

    PCCERT_CONTEXT pCertContext = NULL; 
    BOOL fMore = TRUE; 
    DWORD dwSize = 0; 
    CRYPT_KEY_PROV_INFO* pKeyInfo = NULL; 
    DWORD PropId = CERT_KEY_PROV_INFO_PROP_ID; 

//-------------------------------------------------------------------- 
// Поиск сертификатов в хранилище до тех пор, пока не будет достигнут 
// конец хранилища, или сертификат с ключем AT_KEYEXCHANGE не будет найден. 

    do { 
//------------------------------------------------------------- 
// Для простоты в этом коде реализован только поиск первого 
// вхождения ключа AT_KEYEXCHANGE. Во многих случаях, помимо 
// поиска типа ключа, осуществляется также поиск определенного 
// имени субъекта. 

        pCertContext = CertFindCertificateInStore( 
	    hCertStore, // Дескриптор хранилища, в котором будет осуществлен поиск. 
	    TYPE_DER,   // Тип зашифрования. В этом поиске не используется. 
	    0,          // dwFindFlags. Специальный критерий поиска. 
	                // в этом поиске не используется. 
	    CERT_FIND_PROPERTY, 
                        // Тип поиска. Задает вид поиска, который будет 
                        // осуществлен. В этом случае, поиск сертификатов,  
                        // имеющих определенные расширенные свойства. 
	    &PropId,    // pvFindPara. Выдает определенное значение
                        // поиска, в данном случае идентификатор  
                        // расширенного свойства. 
	    pCertContext); 
                        // pCertContext равен NULL для первого вызова  
                        // функции. Если функция вызывается в цикле,
                        // после первого ее вызова pCertContext становится
                        // указателем, возвращенным предыдущим вызовом.

	if (!pCertContext) 
	    break;

//------------------------------------------------------------- 
// Однократный вызов функции CertGetCertificateContextProperty  
// для получения возврашенного размера структуры. 

        if(!(CertGetCertificateContextProperty( 
             pCertContext, 
             CERT_KEY_PROV_INFO_PROP_ID, 
             NULL, &dwSize))) 
        {
             HandleError("Error getting key property."); 
        } 

//-------------------------------------------------------------- 
// распределение памяти под возвращенную структуру. 

        if(pKeyInfo) 
            free(pKeyInfo); 

        pKeyInfo = (CRYPT_KEY_PROV_INFO*)malloc(dwSize);

        if(!pKeyInfo) 
        { 
             HandleError("Error allocating memory for pKeyInfo."); 
        }

//-------------------------------------------------------------- 
// Получение структуры информации о ключе. 

        if(!(CertGetCertificateContextProperty( 
           pCertContext, 
           CERT_KEY_PROV_INFO_PROP_ID, 
           pKeyInfo, 
           &dwSize))) 
        { 
            HandleError("The second call to the function failed."); 
        } 

//------------------------------------------- 
// Проверка члена dwKeySpec на расширенный ключ и типа провайдера.

        if(pKeyInfo->dwKeySpec == AT_KEYEXCHANGE && pKeyInfo->dwProvType == PROV_GOST_2001_DH) 
        { 
            fMore = FALSE; 
        } 
    } while(fMore && pCertContext);

    if(pKeyInfo) 
          free(pKeyInfo); 
    return (pCertContext); 
} // Конец определения GetRecipientCert 

//END-OF-FILE
