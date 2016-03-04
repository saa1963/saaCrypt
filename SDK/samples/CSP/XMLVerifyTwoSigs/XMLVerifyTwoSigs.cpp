/*
 * Copyright(C) 2000-2007 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 *
 * ����������� ���, ������������ � ���� �����, ������������
 * ������������� ��� ����� �������� � �� ����� ���� �����������
 * ��� ������ ����������.
 *
 * �������� ������-��� �� ����� �������
 * ��������������� �� ���������������� ����� ����.
 */

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ �������� � �������� ���� ����������� �������� ��������� MSXML 5.0. 
// ���� ������� ���������� �� ����� ��������� ����������. ��� ����������, � ���� �������, 
// ���������� �� ����������� ������� ��������� �����������.
// ���� �������� ������� ���������� �� DOM ���� <ds:KeyInfo> ������������ XML-���������.
// ������ ��������� ����� 3 ��������� ��������� ������:
// 1) ��� ��������� XML-��������� (�������� ������ ���������������
// �������, ������������ � �������).
// 2) ��� ����������� ��� ������ �������.
// 3) ��� ����������� ��� ������ �������.
//
// � ������� ����������� ��� �������:
// doc_to_sign_two.xml ������ ��� ������� ����� ���������
// node_to_sign_two.xml ������ ��� ������� ������ �������� <ElementToSign...>
//
// � ������ �������� ������ ��� ��������� ���������� �� �������� InfoPath
// ������� XMLSignForVerify: (ip_doc_signed2.xml, ip_node_signed2.xml).
// ��� ��������� ����� ���� ������������ ��� ���� � ������� ��� �������� 
// ������� ��� ����������� InfoPath. ��� �������� ������� ��� �����
// ����������� ����������, ��������������� ���� ������� ��������� � 
// ������������ ��������.
//
// ���������� ���������� CAPICOM (http://www.microsoft.com/downloads/details.aspx?FamilyID=860ee43a-a843-462f-abb5-ff88ea5896f6&DisplayLang=en)
//--------------------------------------------------------------------

#include <locale.h>
#include <atlbase.h>
#include "msxml5.h"

// ��� ������ ���������� #import-� CAPICOM ������ ���� ��������������� ���������.
// VS ������������ ��� ����������� (� ��������� ��� CAPICOM ��������������� VS),
// "�����������" ��������, ��� �� ������ #import �� ����� ��������,
// �� � ��������� ������� ����� �������� � ��������.
// ���������� ��������� ������� (����� ������ ���� � ����������):
// regsvr32 "H:\Program Files\Common Files\Microsoft Shared\CAPICOM\CapiCom.dll"
#import "libid:bd26b198-ee42-4725-9b23-afa912434229"
using namespace CAPICOM;

#define DSIGNS L"xmlns:ds='http://www.w3.org/2000/09/xmldsig#'"
#define OUTFILE L"out_signatures.xml"

//-------------------------------------------------------------
// ���������� � ������������� ����������.

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
// �������� � �������� ���� ����������� �������� XML-���������.
//---------------------------------------------------------------------------
int _tmain(int argc, TCHAR **argv) 
{
    setlocale(LC_ALL, ".1251");
    // ������������� COM ����������.
    if (FAILED(CoInitialize(NULL)))
	HandleError("can't initialize COM Lib");

    // ������ ��������� ������.
    if(argc < 4)
	HandleError("usage: XMLSignForVerify test.xml certname1 certname2\n");

    //-----------------------------------------------------------------------
    // ������������� ��������.
    if(!initObjects())
	exit(-1);

    CComVariant sigFile(argv[1]);  //��� ��������� xml-��������� ��� �������
    printf("Signing %S\n\n", sigFile.bstrVal);

    //------------------------------------------------------------------------------
    // �������� ���� ����������� �������� XML-���������.

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
    // �������� ���� ����������� �������� XML-���������.

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
// �������� ������� �������� XML-���������.
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
}// ����� �������� ������� LoadXML

//------------------------------------------------------------------------------
// ����������� ������� ������� � ������������ � DOM ����� <ds:Signature>
// XML-���������.

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
    // ��������� ��� �� �������� ������� � ���������
    if(FAILED(xmldsig->get_signature(&signatureNode)) || !signatureNode)
    {
	printf("Invalid signature template\n");
	return false;
    }
    return true;
}

//---------------------------------------------------------------------------
// �������� ������� ������� XML-���������.
//---------------------------------------------------------------------------

bool SignXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo,
    TCHAR* userName, VARIANT outfile)
{
    if (fwWriteKeyInfo & CERTIFICATES)
	printf("Sign with fwWriteKeyInfo = CERTIFICATES:\n");

    HCERTSTORE hStore = NULL; // ��������� ��������� ������������
    //------------------------------------------------------------------------------
    // �������� ��������� ������������ "MY" �������� ������������.
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
    // ��������� ����������� � �������� ������ �� ��������� ������������.
    PCCERT_CONTEXT pContext = NULL;
    pContext = CertFindCertificateInStore(hStore,
	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_STR_W,
	userName, NULL);
    if(pContext == NULL)
    {
	HandleError("There is no such a certificate in the store\n");
    }

    // �������� ��� ��������� ���������� �� ������� �����������
    DWORD dwPropId = CERT_KEY_PROV_INFO_PROP_ID;
    DWORD cbData; // ����� ������ ������

    //-----------------------------------------------------------------------
    // ��������� ���������� �� ����������� ������� ��������� �����������. ����������� �������
    // ������������ ������.

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
    // ��������� �� ��������� CRYPT_KEY_PROV_INFO, ���������� ��������� ����������
    // � �������� ����������.
    CRYPT_KEY_PROV_INFO *pbData;

    //-----------------------------------------------------------------------
    // ��������� ������.

    pbData = (CRYPT_KEY_PROV_INFO*)malloc(cbData);
    if(pbData)
	printf("Memory has been allocated.\n");
    else
	HandleError("Malloc operation failed. ");

    //-----------------------------------------------------------------------
    // ��������� ��������� ���������� � �������� ���������� �� ����������� �������
    // ��������� �����������.

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
    // ������������ ��������� �����������.

    if(pContext)
	CertFreeCertificateContext(pContext);

    //--------------------------------------------------------------------------
    // �������� ��������� ������������.

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
    // �������� ����� ������� �� ����������� ����� ��������� ����������.

    CComPtr<IXMLDSigKey> keyRef;
    if (FAILED(xmldsig->createKeyFromCSP(pbData->dwProvType,
	pbData->pwszProvName, pbData->pwszContainerName, 0, &keyRef))
	|| !keyRef)
    {
	printf("createKeyFromCSP key\n");
	return false;
    } 

    //-----------------------------------------------------------------------
    // ������� XML-��������� �� ���������� �����.

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
	// ������ ������� � ���� outfile.
	if(FAILED(xmldoc->save(outfile))) {
	    printf("can't save the signed signature to file: %S.\n", outfile.bstrVal);
	    return false;
	}
	printf("signature saved to file: %S\n", outfile.bstrVal);
    }
    return true;
}
// ����� �������� SignXML

//---------------------------------------------------------------------------
// �������� ������� �������� ������� XML-���������. 
//---------------------------------------------------------------------------

bool VerifyXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, BSTR queryString)
{
    if (fwWriteKeyInfo & CERTIFICATES)
	printf("Verify with fwWriteKeyInfo = CERTIFICATES:\n");

    // �������� DOM ���� <ds:KeyInfo> XML-���������. 
    CComPtr<IXMLDOMNode> pKeyInfo;
    if (FAILED(xmldoc->selectSingleNode(queryString, &pKeyInfo)) || !pKeyInfo)
    {
	printf("Invalid <ds:KeyInfo>\n");
	return false;
    }

    // ��������� ����� �������� ������� �� DOM ���� <ds:KeyInfo> XML-���������. 
    CComPtr<IXMLDSigKey> pKey;
    if(FAILED(xmldsig->createKeyFromNode(pKeyInfo, &pKey)) || !pKey)
    {
	printf("Invalid key from <ds:KeyInfo>\n");
	return false;
    }

    //-----------------------------------------------------------------------
    // ��������� ����� �������� ������� ������������ � �������� �������. 

    CComPtr<IXMLDSigKey> pKeyOut;
    if(FAILED(xmldsig->verify(pKey, &pKeyOut)) || !pKeyOut) {
	printf("Invalid signature.\n");
	return false;
    }

    printf("\n");
    printf("Signature verified on the data.\n\n");

    // �������� ������� ������������, ������� � �����������, �� ������� �������������� �������.
    if(fwWriteKeyInfo & CERTIFICATES)
	return CheckCertificateValidity(pKeyOut);
    return true;
}
// ����� �������� VerifyXML

//-------------------------------------------------------------------------------
// �������� ������� ������������� ��������.
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
// ����� �������� initObjects

//---------------------------------------------------------------------------
// �������� ������� �������� ������� ������������, ������� � �����������, ��
// ������� �������������� ������� XML-���������. 
//---------------------------------------------------------------------------
bool CheckCertificateValidity(IXMLDSigKey *pKey)
{
    if(pKey == NULL) 
    {
	printf("invalid key object.\n");
	return false;
    }

    // ��������� �����������, �� ������� �������������� �������, �� ����� ��������.
    CComPtr<IUnknown> certUnk;
    if(FAILED(pKey->getVerifyingCertificate(&certUnk)))
    {
	printf("Can't get verifying certificate\n");
	return false;
    }

    ICertificatePtr pCert(certUnk.Detach());

    //-----------------------------------------------------------------------
    // ���������� � ����������� �� ���������� ������� ������������. 

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
    // �������� ��������� ����������� � �������.

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
// ����� �������� CheckCertificateValidity

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//------------------------------------------------------------------------------
//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� �� ������ � ����������� ���� 
//  ������ (stderr) � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.
//------------------------------------------------------------------------------
void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    if(!err) err = 1;
    exit(err);
}


