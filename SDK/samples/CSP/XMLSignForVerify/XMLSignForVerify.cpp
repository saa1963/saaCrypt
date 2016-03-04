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
// ����� ��������� ����������. ��� ����������, � ���� �������, ����������
// �� ����������� ������� ��������� �����������.
// ������ ��������� ����� ��� ��������� ��������� ������:
// 1) ��� ��������� XML-��������� (�������� ������ ���������������
// �������, ������������ � �������).
// 2) ��� �����������.
// ����������� (��������) XML-���������� �������� ���� "out_signature.xml".
// ���������: ��� win32 ������������� ������������ _s ������� CRT �������.
//--------------------------------------------------------------------

#include <atlbase.h>
#include "msxml5.h"

#define DSIGNS L"xmlns:ds='http://www.w3.org/2000/09/xmldsig#'"
#define OUTFILE L"out_signature.xml"

//-------------------------------------------------------------
// ���������� � ������������� ����������. 

CComPtr<IXMLDOMDocument2> xmldoc;
CComPtr<IXMLDigitalSignatureEx> xmldsig;

bool initObjects(void);
bool LoadXML(VARIANT sigFile);
bool SetSignature(BSTR queryString);
bool SignXML(XMLDSIG_WRITEKEYINFO fwWriteKeyInfo, TCHAR* userName,
    VARIANT outfile);
void HandleError(char *s);

//------------------------------------------------------------------------------
// ������� XML-��������� �� ������ �����������.
//------------------------------------------------------------------------------
int _tmain(int argc, TCHAR **argv) 
{
    // ������������� COM ����������.
    if (FAILED(CoInitialize(NULL)))
	HandleError("can't initialize COM Lib");

    // ������ ��������� ������.
    if(argc < 2)
	HandleError("usage: XMLSignForVerify test.xml certname\n");

    // ������������� ��������.
    if(!initObjects())
	exit(-1);

    CComVariant sigFile(argv[1]);
    printf("Signing %S\n\n", sigFile.bstrVal);
    if(!LoadXML(sigFile))
	exit(-1);

    if(!SetSignature(CComBSTR(L".//ds:Signature")))
	exit(-1);

    // ������� XML-���������.
    CComVariant outfile(OUTFILE);
    if (!SignXML(CERTIFICATES, argv[2], outfile))
	exit(-1);

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
}
// ����� �������� ������� LoadXML

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
//
// ������ ������� ����� �������� fwWriteKeyInfo, ���������� 
// �� ���������� ������� ������������ (���������) XML-���������.
// ������ ������������ (���������) XML-��������� ���������� �������� � ���� 
// ��� ������ ��������: <ds:DigestValue>, <ds:SignatureValue>  �  <ds:KeyInfo>.
// ������ ��� ����������� ����� ���������� ������� SignXML(fwWriteKeyInfo).
// ��������� ����� �������� ������ � ��� ������, ���� ������� 
// � �������� ��������� fwWriteKeyInfo ��������� KEYVALUE. 
// �������� �� ��������, ���� fwWriteKeyInfo = NOKEYINFO � �� 
// ���������� ��������� ��������� � ������  fwWriteKeyInfo = PURGE.
// ����� fwWriteKeyInfo = CERTIFICATES,  ����������� ������� <X509Data> 
// ��� �������, ��� ���������� �����, ������� ������������, ��������.
// 
// ����� �������� fwWriteKeyInfo ����� ������������ ����� ���������� 
// ������������� ���� ��������, ����������� ������ "|" :
// KEYVALUE | CERTIFICATES  - ��������� �������� ����� � ����������� � <ds:KeyInfo>, 
// 			�� ������� ������ ��������.
// KEYVALUE | CERTIFICATES | PURGE - ������� ������� �� �� <ds:KeyInfo>, � ����� 
// 			��������� �������� ����� � �����������.
// KEYVALUE | PURGE - ������� ������� �� �� <ds:KeyInfo>,
//			�  ����� ��������� �������� �����. 
// CERTIFICATES | PURGE - ������� ������� �� �� <ds:KeyInfo>,
//			� ����� ��������� �����������.
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

    //-----------------------------------------------------------------
    // ��������� ����������� � �������� ������ �� ��������� ������������. 
    PCCERT_CONTEXT pContext = NULL ; // �������� �����������
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

    //--------------------------------------------------------------------------
    // ��������� ���������� �� ����������� ������� ��������� �����������.
    // ����������� ������� ������������ ������.
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
    // ��������� �� ��������� CRYPT_KEY_PROV_INFO, ���������� ��������� ����������
    // � �������� ����������.
    CRYPT_KEY_PROV_INFO *pbData;

    //-------------------------------------------------------------------------  
    // ��������� ������.
    if(pbData = (CRYPT_KEY_PROV_INFO*)malloc(cbData))
	printf("Memory has been allocated.\n");
    else
	HandleError("Malloc operation failed. ");

    //------------------------------------------------------------------------
    // ��������� ��������� ���������� � �������� ���������� �� ����������� �������
    // ��������� �����������.
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
    // �������� ����� ������� �� ��������� �����������.

    CComPtr<IXMLDSigKeyEx> keyRef;
    if (FAILED(xmldsig->createKeyFromCertContext((void*)pContext, &keyRef))
	|| !keyRef)
    {
	printf("createKeyFromCertContext failed.\n");
	return false;
    } 

    //----------------------------------------------------
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
}// ����� �������� SignXML

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
