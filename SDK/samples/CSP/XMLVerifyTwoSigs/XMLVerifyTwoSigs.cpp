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
// Пример создания и проверки двух независимых подписей документа MSXML 5.0. 
// Ключ подписи получается по имени ключевого контейнера. Имя контейнера, в свою очередь, 
// получается из расширенных свойств заданного сертификата.
// Ключ проверки подписи получается из DOM узла <ds:KeyInfo> подписанного XML-документа.
// Данная программа имеет 3 аргумента командной строки:
// 1) Имя исходного XML-документа (документ должен соответствовать
// шаблону, приведенному в примере).
// 2) Имя сертификата для первой подписи.
// 3) Имя сертификата для второй подписи.
//
// К примеру прилагаются два шаблона:
// doc_to_sign_two.xml шаблон для подписи всего документа
// node_to_sign_two.xml шаблон для подписи одного элемента <ElementToSign...>
//
// В состав примеров входят два документа полученные из шаблонов InfoPath
// примера XMLSignForVerify: (ip_doc_signed2.xml, ip_node_signed2.xml).
// Эти документы могут быть использованы как вход к примеру для проверки 
// подписи под документами InfoPath. Для проверки подписи под этими
// длкументами необходимо, закоментаривать кода примера связанный с 
// простановкой подписей.
//
// НЕОБХОДИМО установить CAPICOM (http://www.microsoft.com/downloads/details.aspx?FamilyID=860ee43a-a843-462f-abb5-ff88ea5896f6&DisplayLang=en)
//--------------------------------------------------------------------

#include <locale.h>
#include <atlbase.h>
#include "msxml5.h"

// Для работы следующего #import-а CAPICOM должен быть зарегистрирован правильно.
// VS регистрирует его неправильно (с некоторых пор CAPICOM устанавливается VS),
// "неправильно" означает, что не только #import не будет работать,
// но и некоторые объекты будут работать с ошибками.
// Необходимо выполнить команду (важен полный путь к библиотеке):
// regsvr32 "H:\Program Files\Common Files\Microsoft Shared\CAPICOM\CapiCom.dll"
#import "libid:bd26b198-ee42-4725-9b23-afa912434229"
using namespace CAPICOM;

#define DSIGNS L"xmlns:ds='http://www.w3.org/2000/09/xmldsig#'"
#define OUTFILE L"out_signatures.xml"

//-------------------------------------------------------------
// Объявление и инициализация переменных.

CComPtr<IXMLDOMDocument2> xmldoc;
CComPtr<IXMLDigitalSignature> xmldsig;

bool initObjects(void);
bool LoadXML(VARIANT sigFile);
bool SetSignature(BSTR queryString);
bool SignXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, TCHAR* userName,
    VARIANT outfile);
bool VerifyXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, BSTR queryString);
bool CheckCertificateValidity(IXMLDSigKey *pKey);
void HandleError(char *s);

//---------------------------------------------------------------------------
// Создание и проверка двух независимых подписей XML-документа.
//---------------------------------------------------------------------------
int _tmain(int argc, TCHAR **argv) 
{
    setlocale(LC_ALL, ".1251");
    // Инициализация COM библиотеки.
    if (FAILED(CoInitialize(NULL)))
	HandleError("can't initialize COM Lib");

    // Разбор командной строки.
    if(argc < 4)
	HandleError("usage: XMLSignForVerify test.xml certname1 certname2\n");

    //-----------------------------------------------------------------------
    // Инициализация объектов.
    if(!initObjects())
	exit(-1);

    CComVariant sigFile(argv[1]);  //имя исходного xml-документа для подписи
    printf("Signing %S\n\n", sigFile.bstrVal);

    //------------------------------------------------------------------------------
    // Создание двух независимых подписей XML-документа.

    if(!LoadXML(sigFile))
	exit(-1);
    if (!SetSignature(CComBSTR(L".//ds:Signature[@Id='FirstSignature']")))
	exit(-1);
    CComVariant outfile(OUTFILE);
    if (!SignXML(CERTIFICATES, CComBSTR(argv[2]), outfile))
	exit(-1);

    printf("\n");

    if (!SetSignature(CComBSTR(L".//ds:Signature[@Id='SecondSignature']")))
	exit(-1);
    if (!SignXML(CERTIFICATES, CComBSTR(argv[3]), outfile))
	exit(-1);

    //------------------------------------------------------------------------------
    // Проверка двух независимых подписей XML-документа.

    if (!LoadXML(outfile))
	exit(-1);
    if (!SetSignature(CComBSTR(L".//ds:Signature[@Id='FirstSignature']")))
	exit(-1);
    if (!VerifyXML(CERTIFICATES, CComBSTR(L".//ds:Signature[@Id='FirstSignature']"
	L"/ds:KeyInfo/ds:X509Data")))
    {
	exit(-1);
    }

    printf("\n");

    if (!SetSignature(CComBSTR(L".//ds:Signature[@Id='SecondSignature']")))
	exit(-1);
    if (!VerifyXML(CERTIFICATES, CComBSTR(L".//ds:Signature[@Id='SecondSignature']"
	L"/ds:KeyInfo/ds:X509Data")))
    {
	exit(-1);
    }

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
}// Конец описания функции LoadXML

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

    //-----------------------------------------------------------------------
    // Получение сертификата с заданным именем из хранилища сертификатов.
    PCCERT_CONTEXT pContext = NULL;
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

    //-----------------------------------------------------------------------
    // Получение информации из расширенных свойств контекста сертификата. Определение размера
    // возвращаемых данных.

    if(CertGetCertificateContextProperty(pContext, dwPropId, NULL, &cbData))
    {
	printf("CertGetCertificateContextProperty was started for the 1-st time\n");
    }
    else
    {
	printf("CertGetCertificateContextProperty was started for the 1-st time\n");
	HandleError("It's impossible to get the certificate property!\n ");
    }

    //-----------------------------------------------------------------------
    // Указатель на структуру CRYPT_KEY_PROV_INFO, содержащую подробную информацию
    // о ключевом контейнере.
    CRYPT_KEY_PROV_INFO *pbData;

    //-----------------------------------------------------------------------
    // Выделение памяти.

    pbData = (CRYPT_KEY_PROV_INFO*)malloc(cbData);
    if(pbData)
	printf("Memory has been allocated.\n");
    else
	HandleError("Malloc operation failed. ");

    //-----------------------------------------------------------------------
    // Получение подробной информации о ключевом контейнере из расширенных свойств
    // контекста сертификата.

    if(CertGetCertificateContextProperty(pContext, dwPropId, pbData, &cbData))
    {
	printf("CertGetCertificateContextProperty was started for the 2-nd time\n");
	printf( "ContainerName is  %S\n", pbData->pwszContainerName);
    }
    else
    {
	printf("CertGetCertificateContextProperty was started for the 2-nd time\n");
	HandleError("It's impossible to get the certificate property!\n ");
    }

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

    //-----------------------------------------------------------------------
    // Создание ключа подписи по полученному имени ключевого контейнера.

    CComPtr<IXMLDSigKey> keyRef;
    if (FAILED(xmldsig->createKeyFromCSP(pbData->dwProvType,
	pbData->pwszProvName, pbData->pwszContainerName, 0, &keyRef))
	|| !keyRef)
    {
	printf("createKeyFromCSP key\n");
	return false;
    } 

    //-----------------------------------------------------------------------
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
}
// Конец описания SignXML

//---------------------------------------------------------------------------
// Описание функции проверки подписи XML-документа. 
//---------------------------------------------------------------------------

bool VerifyXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, BSTR queryString)
{
    if (fwWriteKeyInfo & CERTIFICATES)
	printf("Verify with fwWriteKeyInfo = CERTIFICATES:\n");

    // Проверка DOM узла <ds:KeyInfo> XML-документа. 
    CComPtr<IXMLDOMNode> pKeyInfo;
    if (FAILED(xmldoc->selectSingleNode(queryString, &pKeyInfo)) || !pKeyInfo)
    {
	printf("Invalid <ds:KeyInfo>\n");
	return false;
    }

    // Получение ключа проверки подписи из DOM узла <ds:KeyInfo> XML-документа. 
    CComPtr<IXMLDSigKey> pKey;
    if(FAILED(xmldsig->createKeyFromNode(pKeyInfo, &pKey)) || !pKey)
    {
	printf("Invalid key from <ds:KeyInfo>\n");
	return false;
    }

    //-----------------------------------------------------------------------
    // Получение ключа проверки цепочки сертификатов и проверка подписи. 

    CComPtr<IXMLDSigKey> pKeyOut;
    if(FAILED(xmldsig->verify(pKey, &pKeyOut)) || !pKeyOut) {
	printf("Invalid signature.\n");
	return false;
    }

    printf("\n");
    printf("Signature verified on the data.\n\n");

    // Проверка цепочки сертификатов, начиная с сертификата, на котором осуществлялась подпись.
    if(fwWriteKeyInfo & CERTIFICATES)
	return CheckCertificateValidity(pKeyOut);
    return true;
}
// Конец описания VerifyXML

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

//---------------------------------------------------------------------------
// Описание функции проверки цепочки сертификатов, начиная с сертификата, на
// котором осуществлялась подпись XML-документа. 
//---------------------------------------------------------------------------
bool CheckCertificateValidity(IXMLDSigKey *pKey)
{
    if(pKey == NULL) 
    {
	printf("invalid key object.\n");
	return false;
    }

    // Получение сертификата, на котором осуществлялась подпись, из ключа проверки.
    CComPtr<IUnknown> certUnk;
    if(FAILED(pKey->getVerifyingCertificate(&certUnk)))
    {
	printf("Can't get verifying certificate\n");
	return false;
    }

    ICertificatePtr pCert(certUnk.Detach());

    //-----------------------------------------------------------------------
    // Построение и прохождение по доверенной цепочке сертификатов. 

    IChain2Ptr pChain = NULL;
    HRESULT hr = pChain.CreateInstance(__uuidof(Chain));
    if(FAILED(hr)) 
    {
	printf("Can't instantiate a trust chain.\n");
	return false;
    }

    if(VARIANT_FALSE == pChain->Build(pCert)) 
    {
	printf("broken trust chain.\n");
	return false;
    }

    printf("Examining certificate chain:\n");
    long count = pChain->Certificates->Count;
    for(long i=1; i<=count; i++) 
    {
	printf("  Certificate No. %d:\n", i);
	if(pCert) 
	{
	    pCert.Release(); 
	    pCert=NULL; 
	}
	pCert = pChain->Certificates->Item[i];
	if(pCert == NULL) 
	{
	    printf("Can't get the %d-th certificate.\n", i);
	    return false;
	}
	printf("     subject: %S\n", (LPWSTR)pCert->SubjectName); 
	printf("     issuer:  %S\n\n", (LPWSTR)pCert->IssuerName);
    }

    //-----------------------------------------------------------------------
    // Проверка корневого сертификата в цепочке.

    if(pCert) 
    {
	pCert.Release();
	pCert=NULL;
    }

    pCert = pChain->Certificates->Item[count];
    if(pCert == NULL) 
    {
	printf("Can't get root certificate.\n");
	return false;
    }
    printf("Display the Root certificate:\n");
    printf("   subject:  %S\n", (LPWSTR)pCert->SubjectName);
    printf("   issuer:   %S\n\n", (LPWSTR)pCert->IssuerName);

    if(pCert)
	pCert.Release();
    if(pChain)
	pChain.Release();
    printf("Certificate used is valid.\n");
    return true;
}
// Конец описания CheckCertificateValidity

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


