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

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Пример кода для закодирования и раскодирования сообщения.
//--------------------------------------------------------------------

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
static void HandleError(char *s);

// Для устранения конфликта с определением CMSG_DATA в системном заголовочном файле socket.h на
// Unix-системах, тип сообщения CMSG_DATA переименован в CMSG_CAPILITE_DATA. Если socket.h не
// используется, то можно определить CMSG_DATA как:
// 
// #define CMSG_DATA CMSG_CAPILITE_DATA
//
#ifdef UNIX
#define CMSG_DATA CMSG_CAPILITE_DATA
#endif // UNIX

int main(void)
{
//-------------------------------------------------------------
// Объявление и инициализация переменных. Они получают указатель на  
// содержимое сообщения. Эта программа создает содержимое сообщения 
// и получает указатель на него. В большинстве случаев, содержимое  
// сообщения уже где-то существует и указатель на сообщение устанавливается
// приложением. 

HCRYPTMSG hMsg;
BYTE* pbContent;     // Байтовый указатель на сообщение
DWORD cbContent;     // Длина сообщения
DWORD cbEncodedBlob;
BYTE *pbEncodedBlob;

//--------------------------------------------------------------------
//  Следующие ниже переменные используются только в фазе раскодирования.

DWORD cbDecoded;
BYTE *pbDecoded;

//--------------------------------------------------------------------
//  Начало выполнения. Отображение исходного сообщения. 

pbContent = (BYTE*) "Security is our only business";
cbContent = (DWORD)(strlen((char *)pbContent)+1);

printf("The original message => %s\n",pbContent);  

//--------------------------------------------------------------------
// Открытие сообщения для закодирования.

hMsg = CryptMsgOpenToEncode(
          MY_ENCODING_TYPE,        // Тип закодированного сообщения
          0,                       // Флаги
          CMSG_DATA,               // Тип сообщения
          NULL,                    // Указатель на структуру
          NULL,                    // Внутренний контекст ID объекта
          NULL);                   // Потоковая информация (не используется)

if(hMsg)
{
    printf("The message to be encoded has been opened. \n");
}
else
{
     HandleError("OpenToEncode failed");
}
//--------------------------------------------------------------------
// Обновление сообщения с данными.

if(CryptMsgUpdate(
        hMsg,         // Дескриптор сообщения
        pbContent,    // Указатель на содержимое
        cbContent,    // Размер содержимого
        TRUE))        // Последний вызов
{
     printf("Content has been added to the encoded message. \n");
}
else
{
      HandleError("MsgUpdate failed");
}
//--------------------------------------------------------------------
// Получение размера BLOBа закодированного сообщения.

if(CryptMsgGetParam(
       hMsg,			// Дескриптор сообщения
       CMSG_CONTENT_PARAM,	// Тип параметров
       0,			// Индекс
       NULL,			// Указатель на BLOB
       &cbEncodedBlob)) {	// Размер BLOBа
    if(cbEncodedBlob) {
	printf("The length of the data has been calculated. \n");
    } else {
	HandleError("Getting cbEncodedBlob length failed");
    }
} else {
    HandleError("Getting cbEncodedBlob length failed");
}
//--------------------------------------------------------------------
// Распределени памяти под закодированный BLOB.

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
// Получение результирующего сообщения.

if(CryptMsgGetParam(
               hMsg,                      // Дескриптор сообщения
               CMSG_CONTENT_PARAM,        // Тип параметров
               0,                         // Индекс
               pbEncodedBlob,             // Указатель на BLOB
               &cbEncodedBlob))           // Размер BLOBа
{
    printf("Message encoded successfully. \n");
}
else
{
      HandleError("MsgGetParam failed");
}
//--------------------------------------------------------------------
// pbEncodedBlob сейчас указывает на закодированное, подписанное содержимое.

//--------------------------------------------------------------------
// Закрытие сообщения.

if(hMsg)
    CryptMsgClose(hMsg);

//--------------------------------------------------------------------
// Нижеследующий код осуществляет раскодирование сообщения. 
// Этот код может быть включен здесь или может быть использован в 
// автономной программе, если сообщение, подлежащее раскодированию,  
// и его длина заданы. 
// BLOB закодированного сообщения и его длина могут быть прочитаны 
// с дискового файла или могут быть получены из e-mail сообщения или  
// из других входных данных.

//--------------------------------------------------------------------
// Открытие сообщения для раскодирования.

hMsg = CryptMsgOpenToDecode(
               MY_ENCODING_TYPE,      // тип закодированного сообщения.
               0,                     // Флаги.
               0,                     // Поиск данных сообщения.
               0,                     // Криптографический провайдер.
               NULL,                  // Информация издателя.
               NULL);                 // потоковая информация.

if(hMsg) 
{
     printf("The message to decode is open. \n");
}
else
{
    HandleError("OpenToDecode failed");
}
//--------------------------------------------------------------------
// Обновление сообщения с закодированным BLOBом.
// И pbEncodedBlob, закодированные данные, 
// и cbEncodedBlob, длина закодированных данных,
// должны быть доступны. 

printf("\nThe length of the encoded message is %d.\n\n",cbEncodedBlob);

if(CryptMsgUpdate(
    hMsg,                 // Дескриптор сообщения
    pbEncodedBlob,        // Указатель на закодированный BLOB
    cbEncodedBlob,        // Размер закодированного BLOBа
    TRUE))                // Последний вызов
{
      printf("The encoded BLOB has been added to the message. \n");
}
else
{
    HandleError("Decode MsgUpdate failed");
}
//--------------------------------------------------------------------
// Получение размера содержимого.

if(CryptMsgGetParam(
                  hMsg,                  // Дескриптор сообщения
                  CMSG_CONTENT_PARAM,    // Тип параметров
                  0,                     // Индекс
                  NULL,                  // Адрес возвращаемой 
                                         // информации
                  &cbDecoded))           // Размер возвращаемой
                                         // информации
{
    printf("The decoded message size is %d. \n", cbDecoded);
}
else
{
    HandleError("Decode CMSG_CONTENT_PARAM failed");
}
//--------------------------------------------------------------------
// Распределение памяти.

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
// Получение указателя на содержимое.

if(CryptMsgGetParam(
                  hMsg,                  // Дескриптор сообщения
                  CMSG_CONTENT_PARAM,    // Тип параметров
                  0,                     // Индекс
                  pbDecoded,             // Адрес возвращаемой 
                                         // информации
                  &cbDecoded))           // Размер возвращаемой 
                                         // информации
{
     printf("The message is %s.\n",(LPSTR)pbDecoded);
}
else
{
     HandleError("Decode CMSG_CONTENT_PARAM #2 failed");
}
//--------------------------------------------------------------------
// Освобождение.

if(pbEncodedBlob)
    free(pbEncodedBlob);
if(pbDecoded)
    free(pbDecoded);
if(hMsg)
    CryptMsgClose(hMsg);

printf("This program ran to completion without error. \n");
return 0;
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
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE
