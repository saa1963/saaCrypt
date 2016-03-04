/*
 * Copyright(C) 2000-2010 ������ ���
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
// ������ ���� ��� ������� � �������� ������� ������ 
// ��������� ��� ������ ������� Crypt.

// ��� ���������������� ������� ���� ��������� ��������� � ������ 
// AT_KEYEXCHANGE � ������ Test(Unix) ��� ��������� �� ���������(WIN),
// ���� ����� ������� ��������� ������ CryptAcquireContext.
//--------------------------------------------------------------------


static void HandleError(const char *s);
//--------------------------------------------------------------------
// � ���� ��������� ������������ ������� GetRecipientCert, ����������� 
// ����� � ��������� ����� main.

static PCCERT_CONTEXT GetRecipientCert(HCERTSTORE hCertStore);

//--------------------------------------------------------------------
static void do_low_sign();
static BYTE* pbContent = (BYTE*) "Security is our business."; //���������
static DWORD cbContent = (DWORD)(strlen((char *)pbContent)+1);//����� ���������
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
    HCRYPTPROV      hCryptProv = 0;         /* ���������� ����������*/
    HCERTSTORE	    hStoreHandle=0;		//�e�������� �����
    PCCERT_CONTEXT  pRecipientCert = NULL;       /* ����������, ������������ ��� ������������ ���*/
    DWORD           keytype = AT_KEYEXCHANGE;            /* ��� ����� (������������)*/
    HCRYPTMSG       hMsg = 0;               /* ���������� ���������*/
    CRYPT_ALGORITHM_IDENTIFIER  HashAlgorithm;  /* ������������� ��������� �����������*/
    DWORD                       HashAlgSize;
    CMSG_SIGNER_ENCODE_INFO     SignerEncodeInfo;   /* ���������, ����������� �����������*/
    CMSG_SIGNER_ENCODE_INFO     SignerEncodeInfoArray[1]; /* ������ ��������, ����������� �����������*/
    CERT_BLOB                   SignerCertBlob;
    CERT_BLOB                   SignerCertBlobArray[1];
    CMSG_SIGNED_ENCODE_INFO     SignedMsgEncodeInfo;    /* ���������, ����������� ����������� ���������*/
    BOOL                        bReleaseContext;
    DWORD                       flags = 0;
//--------------------------------------------------------------------
//  ������ ��������� ������.

    printf("About to begin with the message %s.\n",pbContent);
    printf("The message length is %d bytes. \n", cbContent);

//--------------------------------------------------------------------
// �������� ���������� ��������� ������������.

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
// ��������� ��������� �� ���������� �������� � �������
// ������� GetRecipientCert. 

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
// ��������� ��������� �����
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
// ������������� ��������� �������������� ���������.

    HashAlgSize = sizeof(HashAlgorithm);

//--------------------------------------------------------------------
// ������������� ��������� � �����.

    memset(&HashAlgorithm, 0, HashAlgSize);

//--------------------------------------------------------------------
// ��������� ������������ ��������.

    HashAlgorithm.pszObjId = (LPSTR)szOID_CP_GOST_R3411;

/*--------------------------------------------------------------------*/
    /* �������������� ��������� CMSG_SIGNER_ENCODE_INFO*/


    memset(&SignerEncodeInfo, 0, sizeof(CMSG_SIGNER_ENCODE_INFO));
    SignerEncodeInfo.cbSize = sizeof(CMSG_SIGNER_ENCODE_INFO);
    SignerEncodeInfo.pCertInfo = pRecipientCert->pCertInfo;
    SignerEncodeInfo.hCryptProv = hCryptProv;
    SignerEncodeInfo.dwKeySpec = keytype;
    SignerEncodeInfo.HashAlgorithm = HashAlgorithm;
    SignerEncodeInfo.pvHashAuxInfo = NULL;

/*--------------------------------------------------------------------*/
/* �������� ������ ������������. ������ ������ �� ������.*/

    SignerEncodeInfoArray[0] = SignerEncodeInfo;

/*--------------------------------------------------------------------*/
/* �������������� ��������� CMSG_SIGNED_ENCODE_INFO*/

    SignerCertBlob.cbData = pRecipientCert->cbCertEncoded;
    SignerCertBlob.pbData = pRecipientCert->pbCertEncoded;

/*--------------------------------------------------------------------*/
/* �������������� ��������� ������ �������� CertBlob.*/

    SignerCertBlobArray[0] = SignerCertBlob;
    memset(&SignedMsgEncodeInfo, 0, sizeof(CMSG_SIGNED_ENCODE_INFO));
    SignedMsgEncodeInfo.cbSize = sizeof(CMSG_SIGNED_ENCODE_INFO);
    SignedMsgEncodeInfo.cSigners = 1;
    SignedMsgEncodeInfo.rgSigners = SignerEncodeInfoArray;
    SignedMsgEncodeInfo.cCertEncoded = 0;
    SignedMsgEncodeInfo.rgCertEncoded = NULL;
    SignedMsgEncodeInfo.rgCrlEncoded = NULL;

/*--------------------------------------------------------------------*/
/* �������� ���������� ���������*/
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
/* �������� � ��������� ������������� ������*/

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
/* ��������� ����� ������������ ���������*/
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
/* ����������� ������, ��������� �����*/

    pbEncodedBlob = (BYTE *)malloc(cbEncodedBlob);
    if (!pbEncodedBlob)
                HandleError("Memory allocation failed");

/*--------------------------------------------------------------------*/
/* ������ ����������� ���������*/

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

//������������ ������

    if(hMsg)
	CryptMsgClose(hMsg);
    if(hCryptProv && bReleaseContext)
	CryptReleaseContext(hCryptProv,0);

}//end of do_low_sign

void do_low_verify() {
    HCRYPTPROV          hCryptProv = 0;         /* ���������� ����������*/
    PCCERT_CONTEXT      pRecipientCert = NULL;       /* ����������, ������������ ��� �������� ���*/
    BYTE                *mem_tbs = pbEncodedBlob;
    size_t              mem_len = cbEncodedBlob;
    HCRYPTMSG           hMsg = 0;               /* ���������� ���������*/
    DWORD               cbDecoded;
    BYTE                *pbDecoded = NULL;
    PCERT_INFO          pSignerCertInfo = NULL;
    PCCERT_CONTEXT      pSignerCertContext = NULL;
    PCERT_INFO          pSignerCertificateInfo = NULL;
    HCERTSTORE          hStoreHandle = NULL;
    DWORD		flags=0;
    BOOL		bResult;	

    if(CryptAcquireContext(
	   &hCryptProv,         // ����� ������������� �����������.
	   NULL,// ������������ ��������� Test
	   NULL,                // ������������ ��������� �� ���������.
	   75,   // ���������� ��� ������������ � �������.
	   CRYPT_VERIFYCONTEXT))               // ������� ����� �� �����.
    {
        printf("A CSP has been acquired. \n");
    }
    else
    {
        HandleError("Cryptographic context could not be acquired.");
    }
//--------------------------------------------------------------------
// �������� ���������� ��������� ������������.

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
// ��������� ��������� �� ���������� �������� � �������
// ������� GetRecipientCert.

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
/* ������� ��������� ��� �������������*/

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
/* �������� � ��������� ����������� ������*/
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
/* ��������� ����� ����������� ������*/

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
/* ����������� ������*/
    pbDecoded = (BYTE *) malloc(cbDecoded);
    if (!pbDecoded)
	HandleError("Decode memory allocation failed");
/*--------------------------------------------------------------------*/
/* ������ ����������� ������*/

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
/* �������� ���*/
/* ������� ��������� ���������� CERT_INFO �� �����������.*/

/*--------------------------------------------------------------------*/
/* ���� ���������� ����� */
/* �������� ���������� � ������ � ���� ������������.*/
/* ��� ������� ������ ��� ����, ����� ����� ������� ���������� �������� */
/* CertGetSubjectCertificateFromStore, ������� ����� ������������, ����*/
/* ���������� ����������� ��������� � ����� ���������.*/
    if (pRecipientCert) {
	hStoreHandle = CertOpenStore(CERT_STORE_PROV_MEMORY, TYPE_DER, 0, CERT_STORE_CREATE_NEW_FLAG,NULL);
	if (!hStoreHandle)
	    HandleError("Cannot create temporary store in memory.");
	/* ������� ���������� � ����������*/
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
/* ������ ���������� ����������� � �����������*/

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
/* ��������� ��������� CERT_INFO ��������� ��� ���������*/
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
//������������ ������
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

//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� �� ������ � ����������� ���� 
//  ������ (stderr) � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.

void HandleError(const char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    if(!err) err = 1;
    exit(err);
} // ����� ���������� HandleError

//--------------------------------------------------------------------
// GetRecipientCert ����������� ����������� � ��������� � �������
// ������ ����������, ���������� ������ AT_EXCHANGE. ���� ����������
// ���������� ������, �� ������������ ��������� �� ����.  

PCCERT_CONTEXT GetRecipientCert(HCERTSTORE hCertStore) 
//-------------------------------------------------------------------- 
// ��������������� ��������: 
// hCertStore, ���������� ���������, � ������� ����� ����������� �����. 
{ 
//-------------------------------------------------------------------- 
// ���������� � ������������� ��������� ����������. 

    PCCERT_CONTEXT pCertContext = NULL; 
    BOOL fMore = TRUE; 
    DWORD dwSize = 0; 
    CRYPT_KEY_PROV_INFO* pKeyInfo = NULL; 
    DWORD PropId = CERT_KEY_PROV_INFO_PROP_ID; 

//-------------------------------------------------------------------- 
// ����� ������������ � ��������� �� ��� ���, ���� �� ����� ��������� 
// ����� ���������, ��� ���������� � ������ AT_KEYEXCHANGE �� ����� ������. 

    do { 
//------------------------------------------------------------- 
// ��� �������� � ���� ���� ���������� ������ ����� ������� 
// ��������� ����� AT_KEYEXCHANGE. �� ������ �������, ������ 
// ������ ���� �����, �������������� ����� ����� ������������� 
// ����� ��������. 

        pCertContext = CertFindCertificateInStore( 
	    hCertStore, // ���������� ���������, � ������� ����� ����������� �����. 
	    TYPE_DER,   // ��� ������������. � ���� ������ �� ������������. 
	    0,          // dwFindFlags. ����������� �������� ������. 
	                // � ���� ������ �� ������������. 
	    CERT_FIND_PROPERTY, 
                        // ��� ������. ������ ��� ������, ������� ����� 
                        // �����������. � ���� ������, ����� ������������,  
                        // ������� ������������ ����������� ��������. 
	    &PropId,    // pvFindPara. ������ ������������ ��������
                        // ������, � ������ ������ �������������  
                        // ������������ ��������. 
	    pCertContext); 
                        // pCertContext ����� NULL ��� ������� ������  
                        // �������. ���� ������� ���������� � �����,
                        // ����� ������� �� ������ pCertContext ����������
                        // ����������, ������������ ���������� �������.

	if (!pCertContext) 
	    break;

//------------------------------------------------------------- 
// ����������� ����� ������� CertGetCertificateContextProperty  
// ��� ��������� ������������� ������� ���������. 

        if(!(CertGetCertificateContextProperty( 
             pCertContext, 
             CERT_KEY_PROV_INFO_PROP_ID, 
             NULL, &dwSize))) 
        {
             HandleError("Error getting key property."); 
        } 

//-------------------------------------------------------------- 
// ������������� ������ ��� ������������ ���������. 

        if(pKeyInfo) 
            free(pKeyInfo); 

        pKeyInfo = (CRYPT_KEY_PROV_INFO*)malloc(dwSize);

        if(!pKeyInfo) 
        { 
             HandleError("Error allocating memory for pKeyInfo."); 
        }

//-------------------------------------------------------------- 
// ��������� ��������� ���������� � �����. 

        if(!(CertGetCertificateContextProperty( 
           pCertContext, 
           CERT_KEY_PROV_INFO_PROP_ID, 
           pKeyInfo, 
           &dwSize))) 
        { 
            HandleError("The second call to the function failed."); 
        } 

//------------------------------------------- 
// �������� ����� dwKeySpec �� ����������� ���� � ���� ����������.

        if(pKeyInfo->dwKeySpec == AT_KEYEXCHANGE && pKeyInfo->dwProvType == PROV_GOST_2001_DH) 
        { 
            fMore = FALSE; 
        } 
    } while(fMore && pCertContext);

    if(pKeyInfo) 
          free(pKeyInfo); 
    return (pCertContext); 
} // ����� ����������� GetRecipientCert 

//END-OF-FILE
