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
// ������ �������� ������� ��������� MSXML 5.0 �� �����, ���������� �� 
// DOM ���� <ds:KeyInfo> ������������ XML-���������. 
// ����� ������ ��������� ������������ �������� ���������������� �������
// ������������, ������� � �����������, �� ������� �������������� �������,
// � ���������� �������� ������������.
// � �������� ������������� ��������� ��������� ������ ��������� 
// "out_signature.xml" - ��� XML-���������, ����������� �� ����� �������.
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

//-------------------------------------------------------------
// ���������� � ������������� ����������. 

CComPtr<IXMLDOMDocument2> xmldoc;
CComPtr<IXMLDigitalSignature> xmldsig;

bool initObjects(void);
bool LoadXML(VARIANT sigFile);
bool SetSignature(BSTR queryString);
bool VerifyXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, BSTR queryString);
bool CheckCertificateValidity(IXMLDSigKey *pKey);
void HandleError(char *s);

// �������� ������� XML-���������.
int _tmain(int argc, TCHAR **argv)
{
    setlocale(LC_ALL, ".1251");
    // ������������� COM ����������.
    if (FAILED(CoInitialize(NULL)))
	HandleError("can't initialize COM Lib");

    // ������ ��������� ������.
    if(argc < 2)
	HandleError("usage: xmlverify test.xml");

    // ������������� ��������.
    if(!initObjects())
	exit(-1);

    CComVariant sigFile(argv[1]);
    printf("Verifying %S\n\n", sigFile.bstrVal);
    if(!LoadXML(sigFile))
	exit(-1);

    if(!SetSignature(CComBSTR(L".//ds:Signature")))
	exit(-1);

    // �������� ������� XML-���������.
    if(!VerifyXML(CERTIFICATES, L".//ds:KeyInfo/ds:X509Data"))
	exit(-1);
    //if(!VerifyXML(KEYVALUE, L".//ds:KeyInfo/ds:KeyValue"))
    //    exit(-1);

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

// �������� ������� �������� ������� XML-���������. 
bool VerifyXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, BSTR queryString)
{
    // �������� DOM ���� <ds:KeyInfo> XML-���������. 
    CComPtr<IXMLDOMNode> pKeyInfo;
    if (FAILED(xmldoc->selectSingleNode(queryString, &pKeyInfo)) || !pKeyInfo)
    {
	printf("Invalid <ds:KeyInfo>\n");
	return false;
    }

    CComPtr<IXMLDSigKey> pKey;
    if(FAILED(xmldsig->createKeyFromNode(pKeyInfo, &pKey)) || !pKey)
    {
	printf("Invalid key from <ds:KeyInfo>\n");
	return false;
    }

   // ��������� ����� �������� ������� ������������ � �������� �������. 
    CComPtr<IXMLDSigKey> pKeyOut;
    if(FAILED(xmldsig->verify(pKey, &pKeyOut)) || !pKeyOut) {
	printf("Invalid signature.\n");
	return false;
    }

    printf("\n");
    printf("Signature verified on the data.\n\n");

    // �������� ������� ������������, ������� � �����������, �� ������� �������������� �������. 
   if (fwWriteKeyInfo & CERTIFICATES)
      return CheckCertificateValidity(pKeyOut);
   return true;
}

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
