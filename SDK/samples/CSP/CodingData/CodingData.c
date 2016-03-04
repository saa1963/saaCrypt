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
#ifdef HAVE_CONFIG_H
#include "myconfig.h"
#endif
#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#   include <wincryptex.h>
#else
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#   include <WinCryptEx.h>
#   include <string.h>
#   include <stdlib.h>
#endif

// ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
// ��� �������������� ������ ������������)
//--------------------------------------------------------------------
// ������ ���� ��� ������������� � �������������� ���������.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
static void HandleError(char *s);

// ��� ���������� ��������� � ������������ CMSG_DATA � ��������� ������������ ����� socket.h ��
// Unix-��������, ��� ��������� CMSG_DATA ������������ � CMSG_CAPILITE_DATA. ���� socket.h ��
// ������������, �� ����� ���������� CMSG_DATA ���:
// 
// #define CMSG_DATA CMSG_CAPILITE_DATA
//
#ifdef UNIX
#define CMSG_DATA CMSG_CAPILITE_DATA
#endif // UNIX

int main(void)
{
//-------------------------------------------------------------
// ���������� � ������������� ����������. ��� �������� ��������� ��  
// ���������� ���������. ��� ��������� ������� ���������� ��������� 
// � �������� ��������� �� ����. � ����������� �������, ����������  
// ��������� ��� ���-�� ���������� � ��������� �� ��������� ���������������
// �����������. 

HCRYPTMSG hMsg;
BYTE* pbContent;     // �������� ��������� �� ���������
DWORD cbContent;     // ����� ���������
DWORD cbEncodedBlob;
BYTE *pbEncodedBlob;

//--------------------------------------------------------------------
//  ��������� ���� ���������� ������������ ������ � ���� ��������������.

DWORD cbDecoded;
BYTE *pbDecoded;

//--------------------------------------------------------------------
//  ������ ����������. ����������� ��������� ���������. 

pbContent = (BYTE*) "Security is our only business";
cbContent = (DWORD)(strlen((char *)pbContent)+1);

printf("The original message => %s\n",pbContent);  

//--------------------------------------------------------------------
// �������� ��������� ��� �������������.

hMsg = CryptMsgOpenToEncode(
          MY_ENCODING_TYPE,        // ��� ��������������� ���������
          0,                       // �����
          CMSG_DATA,               // ��� ���������
          NULL,                    // ��������� �� ���������
          NULL,                    // ���������� �������� ID �������
          NULL);                   // ��������� ���������� (�� ������������)

if(hMsg)
{
    printf("The message to be encoded has been opened. \n");
}
else
{
     HandleError("OpenToEncode failed");
}
//--------------------------------------------------------------------
// ���������� ��������� � �������.

if(CryptMsgUpdate(
        hMsg,         // ���������� ���������
        pbContent,    // ��������� �� ����������
        cbContent,    // ������ �����������
        TRUE))        // ��������� �����
{
     printf("Content has been added to the encoded message. \n");
}
else
{
      HandleError("MsgUpdate failed");
}
//--------------------------------------------------------------------
// ��������� ������� BLOB� ��������������� ���������.

if(CryptMsgGetParam(
       hMsg,			// ���������� ���������
       CMSG_CONTENT_PARAM,	// ��� ����������
       0,			// ������
       NULL,			// ��������� �� BLOB
       &cbEncodedBlob)) {	// ������ BLOB�
    if(cbEncodedBlob) {
	printf("The length of the data has been calculated. \n");
    } else {
	HandleError("Getting cbEncodedBlob length failed");
    }
} else {
    HandleError("Getting cbEncodedBlob length failed");
}
//--------------------------------------------------------------------
// ������������ ������ ��� �������������� BLOB.

pbEncodedBlob = (BYTE *) malloc(cbEncodedBlob);

if(pbEncodedBlob)
{
   printf("Memory has been allocated for the signed message. \n");
}
else
{
   HandleError("Memory allocation failed");
}

//--------------------------------------------------------------------
// ��������� ��������������� ���������.

if(CryptMsgGetParam(
               hMsg,                      // ���������� ���������
               CMSG_CONTENT_PARAM,        // ��� ����������
               0,                         // ������
               pbEncodedBlob,             // ��������� �� BLOB
               &cbEncodedBlob))           // ������ BLOB�
{
    printf("Message encoded successfully. \n");
}
else
{
      HandleError("MsgGetParam failed");
}
//--------------------------------------------------------------------
// pbEncodedBlob ������ ��������� �� ��������������, ����������� ����������.

//--------------------------------------------------------------------
// �������� ���������.

if(hMsg)
    CryptMsgClose(hMsg);

//--------------------------------------------------------------------
// ������������� ��� ������������ �������������� ���������. 
// ���� ��� ����� ���� ������� ����� ��� ����� ���� ����������� � 
// ���������� ���������, ���� ���������, ���������� ��������������,  
// � ��� ����� ������. 
// BLOB ��������������� ��������� � ��� ����� ����� ���� ��������� 
// � ��������� ����� ��� ����� ���� �������� �� e-mail ��������� ���  
// �� ������ ������� ������.

//--------------------------------------------------------------------
// �������� ��������� ��� ��������������.

hMsg = CryptMsgOpenToDecode(
               MY_ENCODING_TYPE,      // ��� ��������������� ���������.
               0,                     // �����.
               0,                     // ����� ������ ���������.
               0,                     // ����������������� ���������.
               NULL,                  // ���������� ��������.
               NULL);                 // ��������� ����������.

if(hMsg) 
{
     printf("The message to decode is open. \n");
}
else
{
    HandleError("OpenToDecode failed");
}
//--------------------------------------------------------------------
// ���������� ��������� � �������������� BLOB��.
// � pbEncodedBlob, �������������� ������, 
// � cbEncodedBlob, ����� �������������� ������,
// ������ ���� ��������. 

printf("\nThe length of the encoded message is %d.\n\n",cbEncodedBlob);

if(CryptMsgUpdate(
    hMsg,                 // ���������� ���������
    pbEncodedBlob,        // ��������� �� �������������� BLOB
    cbEncodedBlob,        // ������ ��������������� BLOB�
    TRUE))                // ��������� �����
{
      printf("The encoded BLOB has been added to the message. \n");
}
else
{
    HandleError("Decode MsgUpdate failed");
}
//--------------------------------------------------------------------
// ��������� ������� �����������.

if(CryptMsgGetParam(
                  hMsg,                  // ���������� ���������
                  CMSG_CONTENT_PARAM,    // ��� ����������
                  0,                     // ������
                  NULL,                  // ����� ������������ 
                                         // ����������
                  &cbDecoded))           // ������ ������������
                                         // ����������
{
    printf("The decoded message size is %d. \n", cbDecoded);
}
else
{
    HandleError("Decode CMSG_CONTENT_PARAM failed");
}
//--------------------------------------------------------------------
// ������������� ������.

pbDecoded = (BYTE *) malloc(cbDecoded);

if(pbDecoded)
{
     printf("Memory has been allocated for the decoded message.\n");
}
else
{
    HandleError("Decoding memory allocation failed.");
}
//--------------------------------------------------------------------
// ��������� ��������� �� ����������.

if(CryptMsgGetParam(
                  hMsg,                  // ���������� ���������
                  CMSG_CONTENT_PARAM,    // ��� ����������
                  0,                     // ������
                  pbDecoded,             // ����� ������������ 
                                         // ����������
                  &cbDecoded))           // ������ ������������ 
                                         // ����������
{
     printf("The message is %s.\n",(LPSTR)pbDecoded);
}
else
{
     HandleError("Decode CMSG_CONTENT_PARAM #2 failed");
}
//--------------------------------------------------------------------
// ������������.

if(pbEncodedBlob)
    free(pbEncodedBlob);
if(pbDecoded)
    free(pbDecoded);
if(hMsg)
    CryptMsgClose(hMsg);

printf("This program ran to completion without error. \n");
return 0;
} 

// ����� ������� 
// (�� ������� ������� ������ �����������, �� ������������ 
//  ��� �������������� ������ ������������)

//--------------------------------------------------------------------
//  � ���� ������� ������������ ������� HandleError, ������� ���������
//  ������� ������, ��� ������ ��������� � ������ �� ���������. 
//  � ����������� ���������� ��� ������� ���������� ������ ��������, 
//  ������� ������� ����� ������ ��������� �� ������.

void HandleError(char *s)
{
    DWORD err = GetLastError();
    printf("Error number     : 0x%x\n", err);
    printf("Error description: %s\n", s);
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE
