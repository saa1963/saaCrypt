/*
 * Copyright(C) 2000-2007 Проект ИОК
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

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Пример создания подписи документа MSXML 5.0 на ключе, получаемом по 
// имени ключевого контейнера. Имя контейнера, в свою очередь, получается
// из расширенных свойств заданного сертификата.
// Данная программа имеет два аргумента командной строки:
// 1) Имя исходного XML-документа (документ должен соответствовать
// шаблону, приведенному в примере).
// 2) Имя сертификата.
// Подписанным (выходным) XML-документом является файл "out_signature.xml".
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

#include <atlbase.h>
#include "msxml5.h"

#define DSIGNS L"xmlns:ds='http://www.w3.org/2000/09/xmldsig#'"
#define OUTFILE L"out_signature.xml"

//-------------------------------------------------------------
// Объявление и инициализация переменных. 

CComPtr<IXMLDOMDocument2> xmldoc;
CComPtr<IXMLDigitalSignatureEx> xmldsig;

bool initObjects(void);
bool LoadXML(VARIANT sigFile);
bool SetSignature(BSTR queryString);
bool SignXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, TCHAR* userName,
    VARIANT outfile);
void HandleError(char *s);

//------------------------------------------------------------------------------
// Подпись XML-документа на основе сертификата.
//------------------------------------------------------------------------------
int _tmain(int argc, TCHAR **argv) 
{
    // Инициализация COM библиотеки.
    if (FAILED(CoInitialize(NULL)))
	HandleError("can't initialize COM Lib");

    // Разбор командной строки.
    if(argc < 2)
	HandleError("usage: XMLSignForVerify test.xml certname\n");

    // Инициализация объектов.
    if(!initObjects())
	exit(-1);

    CComVariant sigFile(argv[1]);
    printf("Signing %S\n\n", sigFile.bstrVal);
    if(!LoadXML(sigFile))
	exit(-1);

    if(!SetSignature(CComBSTR(L".//ds:Signature")))
	exit(-1);

    // Подпись XML-документа.
    CComVariant outfile(OUTFILE);
    if (!SignXML(CERTIFICATES, argv[2], outfile))
	exit(-1);

    return 0;
}

//---------------------------------------------------------------------------
// Описание функции загрузки XML-документа. 
//---------------------------------------------------------------------------

bool LoadXML(VARIANT sigFile)
{
    VARIANT_BOOL isSuccessful = VARIANT_FALSE;
    if(FAILED(xmldoc->load(sigFile, &isSuccessful)) || isSuccessful == VARIANT_FALSE) 
    {
	printf("Can't load %S\n", sigFile.bstrVal);
	return false;
    }
    if (FAILED(xmldoc->setProperty(L"SelectionNamespaces", CComVariant(DSIGNS))))
    {
	printf("setProperty failed\n");
	return false;
    }
    return true;
}
// Конец описания функции LoadXML

//------------------------------------------------------------------------------
// Определение свойств подписи в соответствии с DOM узлом <ds:Signature>
// XML-документа.

bool SetSignature(BSTR queryString)
{
    CComPtr<IXMLDOMNode> signatureNode;
    if (FAILED(xmldoc->selectSingleNode(queryString, &signatureNode)))
    {
	printf("Failed to get Signature node.\n");
	return false;
    }
    if (FAILED(xmldsig->putref_signature(signatureNode)))
    {
	printf("Failed to set the signature property.\n");
	return false;
    }
    signatureNode.Release();
    // Попробуем тут же получить обратно и проверить
    if(FAILED(xmldsig->get_signature(&signatureNode)) || !signatureNode)
    {
	printf("Invalid signature template\n");
	return false;
    }
    return true;
}


//---------------------------------------------------------------------------
// Описание функции подписи XML-документа. 
//
// Данная функция имеет параметр fwWriteKeyInfo, отвечающий 
// за заполнение шаблона подписанного (выходного) XML-документа.
// Шаблон подписанного (выходного) XML-документа изначально включает в себя 
// три пустых элемента: <ds:DigestValue>, <ds:SignatureValue>  и  <ds:KeyInfo>.
// Первые два заполняются после выполнения функции SignXML(fwWriteKeyInfo).
// Последний будет заполнен только в том случае, если функции 
// в качестве параметра fwWriteKeyInfo передаётся KEYVALUE. 
// Параметр не меняется, если fwWriteKeyInfo = NOKEYINFO и всё 
// содержимое элементов очищается в случае  fwWriteKeyInfo = PURGE.
// Когда fwWriteKeyInfo = CERTIFICATES,  заполняется элемент <X509Data> 
// при условии, что сертификат ключа, который используется, доступен.
// 
// Также параметр fwWriteKeyInfo может представлять собой комбинацию 
// перечисленных выше значений, объединённых знаком "|" :
// KEYVALUE | CERTIFICATES  - Добавляет значение ключа и сертификаты в <ds:KeyInfo>, 
// 			не изменяя другие элементы.
// KEYVALUE | CERTIFICATES | PURGE - Сначала удаляет всё из <ds:KeyInfo>, а затем 
// 			добавляет значение ключа и сертификаты.
// KEYVALUE | PURGE - Сначала удаляет всё из <ds:KeyInfo>,
//			а  затем добавляет значение ключа. 
// CERTIFICATES | PURGE - Сначала удаляет всё из <ds:KeyInfo>,
//			а затем добавляет сертификаты.
//---------------------------------------------------------------------------

bool SignXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo,
    TCHAR* userName, VARIANT outfile)
{
    if (fwWriteKeyInfo & CERTIFICATES)
	printf("Sign with fwWriteKeyInfo = CERTIFICATES:\n");

    HCERTSTORE hStore = NULL; // Дескритор хранилища сертификатов
    //------------------------------------------------------------------------------
    // Открытие хранилища сертификатов "MY" текущего пользователя.
    hStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, 0,
	CERT_STORE_OPEN_EXISTING_FLAG |CERT_STORE_READONLY_FLAG |
	CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
    if(!hStore)
    {
	HandleError("The store could not be opened!\n");
    }
    else
    {
	printf("The store is open!\n");
    }

    //-----------------------------------------------------------------
    // Получение сертификата с заданным именем из хранилища сертификатов. 
    PCCERT_CONTEXT pContext = NULL ; // Контекст сертификата
    pContext = CertFindCertificateInStore(hStore,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_STR_W,
	userName, NULL);
    if(pContext == NULL)
    {
	HandleError("There is no such a certificate in the store\n");
    }

    // Параметр для получения информации из свойств сертификата
    DWORD dwPropId = CERT_KEY_PROV_INFO_PROP_ID;
    DWORD cbData; // Длина буфера данных

    //--------------------------------------------------------------------------
    // Получение информации из расширенных свойств контекста сертификата.
    // Определение размера возвращаемых данных.
    if(CertGetCertificateContextProperty(pContext, dwPropId, NULL, &cbData))
    {
	printf("CertGetCertificateContextProperty was started for the 1-st time\n");
    }
    else
    {
	printf("CertGetCertificateContextProperty was started for the 1-st time\n");
	HandleError("It's impossible to get the certificate property!\n ");
    }

    //--------------------------------------------------------------------------		
    // Указатель на структуру CRYPT_KEY_PROV_INFO, содержащую подробную информацию
    // о ключевом контейнере.
    CRYPT_KEY_PROV_INFO *pbData;

    //-------------------------------------------------------------------------  
    // Выделение памяти.
    if(pbData = (CRYPT_KEY_PROV_INFO*)malloc(cbData))
	printf("Memory has been allocated.\n");
    else
	HandleError("Malloc operation failed. ");

    //------------------------------------------------------------------------
    // Получение подробной информации о ключевом контейнере из расширенных свойств
    // контекста сертификата.
    if(CertGetCertificateContextProperty(pContext, dwPropId, pbData, &cbData))
    {
	printf("CertGetCertificateContextProperty was started for the 2-nd time\n");
	printf( "ContainerName is  %S ProvType = %d\n", pbData->pwszContainerName,
	    pbData->dwProvType);
    }
    else
    {
	printf("CertGetCertificateContextProperty was started for the 2-nd time\n");
	HandleError("It's impossible to get the certificate property!\n ");
    }
    

    CComPtr<IXMLDSigKey> keyCSPRef;
    if (FAILED(xmldsig->createKeyFromCSP(pbData->dwProvType,
	pbData->pwszProvName, pbData->pwszContainerName, 0, &keyCSPRef))
	|| !keyCSPRef)
    {
	printf("createKeyFromCSP failed.\n");
	return false;
    }

    //----------------------------------------------------
    // Создание ключа подписи по контексту сертификата.

    CComPtr<IXMLDSigKeyEx> keyRef;
    if (FAILED(xmldsig->createKeyFromCertContext((void*)pContext, &keyRef))
	|| !keyRef)
    {
	printf("createKeyFromCertContext failed.\n");
	return false;
    } 

    //----------------------------------------------------
    // Подпись XML-документа на полученном ключе.

    CComPtr<IXMLDSigKey> signKeyRef;
    if (FAILED(xmldsig->sign(keyRef, fwWriteKeyInfo, &signKeyRef))
	|| !signKeyRef)
    {
	printf("sign failed.\n");
	return false;
    }

    printf("The specified data was signed successfully.\n");
    printf("Resultant signature:\n");

    //--------------------------------------------------------------------------
    // Освобождение контекста сертификата.
    if(pContext)
	CertFreeCertificateContext(pContext);

    //--------------------------------------------------------------------------
    // Закрытие хранилища сертификатов.

    if(CertCloseStore(hStore, 0))
    {
	printf("Store was closed successfully!\n");
	printf("\n");
    }
    else
    {
	printf("Attempt to close the store is failed\n");
	exit(1);
    }

    CComBSTR bstrXml;
    if (SUCCEEDED(xmldoc->get_xml(&bstrXml)))
    {
	printf("%S\n", bstrXml.m_str);
	// Запись подписи в файл outfile.
	if(FAILED(xmldoc->save(outfile))) {
	    printf("can't save the signed signature to file: %S.\n", outfile.bstrVal);
	    return false;
	}
	printf("signature saved to file: %S\n", outfile.bstrVal);
    }
    return true;
}// Конец описания SignXML

//-------------------------------------------------------------------------------
// Описание функции инициализации объектов.
//-------------------------------------------------------------------------------
bool initObjects(void)
{
    if(FAILED(xmldsig.CoCreateInstance(__uuidof(MXDigitalSignature50)))) 
    {
	printf("Installation of msxml5 is required to run this app.\n");
	return false;
    }
    if(FAILED(xmldoc.CoCreateInstance(__uuidof(DOMDocument50)))) {
	printf("Installation of msxml5 is required to run this app.\n");
	return false;
    }
    xmldoc->put_async(VARIANT_FALSE);
    xmldoc->put_validateOnParse(VARIANT_FALSE);
    xmldoc->put_preserveWhiteSpace(VARIANT_TRUE);
    xmldoc->put_resolveExternals(VARIANT_FALSE);
    return true;
}
// Конец описания initObjects

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
    if(!err) err = 1;
    exit(err);
}
