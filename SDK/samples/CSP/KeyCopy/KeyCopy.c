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
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// В данном примере реализовано копирование закрытого ключа AT_KEYEXCHANGE.
// Копирование ключа AT_SIGNATURE производится аналогично. Необходимо также
// произвести копирование соответствующих этим ключам сертификатов.
// Для корректной работы этой программы копируемые ключи должны быть
// созданы как exportable. Копирование ключа производится из контейнера 
// с именем "Source" (который, соответственно, должен быть создан) в 
// создаваемый контейнер с именем "Destination" (который, если он имеется, 
// до начала работы программы должен быть удален).
//--------------------------------------------------------------------

static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProvResponder = 0;     // Дескриптор CSP получателя 
static HCRYPTPROV hProvSender = 0;	   // Дескриптор CSP отправителя
static HCRYPTKEY hSenderKey = 0;	   // Дескриптор копирумого ключа AT_EXCHANGE
static HCRYPTKEY hResponderKey = 0;       // Дескриптор полученного ключа AT_EXCHANGE
static HCRYPTKEY hSenderEphemKey = 0;     // Дескриптор эфемерного ключа для отправителя
static HCRYPTKEY hResponderEphemKey = 0;  // Дескриптор эфемерного ключа для получателя
static HCRYPTKEY hSenderAgreeKey = 0;	   // Дескриптор ключа согласования отправителя
static HCRYPTKEY hResponderAgreeKey = 0;  // Дескриптор ключа согласования получателя
static BYTE *pbKeyBlob = NULL;		   // Указатель на ключевой BLOB

int  main (void)
{
    //--------------------------------------------------------------------
    //  Объявление и инициализация переменных. 

    DWORD dwProvType = PROV_GOST_2001_DH;   // Тип провайдера по умолчанию
    DWORD data_len = 0;			    // Длина буфера
    BYTE *oid = NULL;			    // Буфер для хранения параметров
    DWORD dwBlobLen = 0;		    // Длина ключевого BLOBа
    DWORD cAlg = CALG_PRO_EXPORT;


    //--------------------------------------------------------------------
    // Получение дескриптора контейнера, в который осуществляется копирование 
    // ключа (получатель). 

    if(CryptAcquireContext(
	&hProvResponder, 
#ifdef _WIN32
	"Destination", 
#else
	"\\\\.\\HDIMAGE\\Destination",
#endif
	NULL, 
	dwProvType, 
	CRYPT_NEWKEYSET)) 
    {
	printf("The responder key container has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // Получение дескриптора контейнера, из которого осуществляется копирование 
    // ключа (отправитель). 


    if(CryptAcquireContext(
	&hProvSender, 
	"Source", 
	NULL, 
	dwProvType, 
	0)) 
    {
	printf("The sender key container has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // Копирование параметров контейнера-отправителя в контейнер-получатель
    // для параметра PP_HASHOID.

    //--------------------------------------------------------------------
    // Определение размера буфера и распределение памяти.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_HASHOID, 
	NULL, 
	&data_len, 
	0))
    {
	printf("Size of the buffer determined. \n");
    }
    else
    {
	HandleError("Error computing buffer length.");
    }

    oid = (BYTE *)malloc( data_len );
    if( !oid )
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // Копирование параметров контейнера-отправителя в буфер.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_HASHOID, 
	oid, 
	&data_len, 
	0))
    {
	printf("Parameters have been written to the buffer. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // Установка параметров, находящихся в буфере, получателем.

    if(CryptSetProvParam(
	hProvResponder, 
	PP_HASHOID, 
	oid, 
	0 ))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    //  Освобождение памяти, используемой буфером.
    free( oid );

    // Обнуление длины буфера для повторного использования.
    data_len = 0;

    //--------------------------------------------------------------------
    // Копирование параметров контейнера-отправителя в контейнер-получатель
    // для параметра PP_CIPHEROID.

    //--------------------------------------------------------------------
    // Определение размера буфера и распределение памяти.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_CIPHEROID, 
	NULL, 
	&data_len, 
	0))
    {
	printf("Size of the buffer determined. \n");
    }
    else
    {
	HandleError("Error computing buffer length.");
    }

    oid = (BYTE *)malloc( data_len );
    if( !oid )
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // Копирование параметров контейнера-отправителя в буфер.

    if(CryptGetProvParam( 
	hProvSender, 
	PP_CIPHEROID, 
	oid, 
	&data_len, 
	0))
    {
	printf("Parameters have been written to the buffer. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // Установка параметров, находящихся в буфере, получателем.

    if(CryptSetProvParam(
	hProvResponder, 
	PP_CIPHEROID, 
	oid, 
	0 ))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	free( oid );
	HandleError("Error during CryptSetProvParam.");
    }
    free( oid );

    //--------------------------------------------------------------------
    // Получение дескриптора закрытого ключа, который будет скопирован.
    if(CryptGetUserKey( 
	hProvSender, 
	AT_KEYEXCHANGE, 
	&hSenderKey )) 
    {
	printf("The sender private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }		

    //--------------------------------------------------------------------
    // Генерация эфемерного ключа в контейнере-отправителе.

    if(CryptGenKey(
	hProvSender, 
	CALG_DH_EL_EPHEM, 
	CRYPT_EXPORTABLE, 
	&hSenderEphemKey)) 
    {   
	printf("Ephemeral sender key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // Генерация эфемерного ключа в контейнере-получателе.

    if(CryptGenKey(
	hProvResponder, 
	CALG_DH_EL_EPHEM, 
	CRYPT_EXPORTABLE | CRYPT_PREGEN,
	&hResponderEphemKey))
    {   
	printf("Ephemeral responder key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }


    //--------------------------------------------------------------------
    // Согласование параметра KP_DHOID, вырабатываемого в алгоритме 
    // Диффи-Хелмана.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа и распределение памяти.

    if(CryptGetKeyParam( 
	hSenderEphemKey, 
	KP_DHOID, 
	NULL, 
	&dwBlobLen, 
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob)
	HandleError("Out of memory. \n");

    //--------------------------------------------------------------------
    // Копирование параметров ключа в BLOB.

    if(CryptGetKeyParam( 
	hSenderEphemKey, 
	KP_DHOID, 
	pbKeyBlob, 
	&dwBlobLen, 
	0))
    {
	printf("Parameters have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // Копирование параметров в hResponderEphemKey.

    if(CryptSetKeyParam(
	hResponderEphemKey, 
	KP_DHOID, 
	pbKeyBlob, 
	0))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    //--------------------------------------------------------------------
    //  Освобождение памяти, используемой BLOBом.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;


    //--------------------------------------------------------------------
    // Генерация ключа

    if(CryptSetKeyParam(
	hResponderEphemKey, 
	KP_X, 
	NULL, 
	0))
    {   
	printf("X parameter is generated. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam.");
    }

    //--------------------------------------------------------------------
    // Получение ключа согласования в контейнере - получателе.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа эфемерного ключа и распределение памяти.

    if(CryptExportKey(
	hSenderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the sender ephemeral key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // Экспортирование эфемерного ключа отправителя в BLOB.
    if(CryptExportKey(
	hSenderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // Получение ключа согласования получателя импортом эфемерного ключа отправителя 
    // на эфемерном ключе получателя.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlob, 
	dwBlobLen, 
	hResponderEphemKey, 
	0, 
	&hResponderAgreeKey))
    {
	printf("The sender ephemeral key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey ephemeral key.");
    }

    //--------------------------------------------------------------------
    //  Освобождение памяти, используемой BLOBом.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // Получение ключа согласования в контейнере - отправителе.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа эфемерного ключа и распределение памяти.

    if(CryptExportKey(
	hResponderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the responder ephemeral key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // Экспортирование эфемерного ключа получателя в BLOB.
    if(CryptExportKey(
	hResponderEphemKey,
	0, 
	PUBLICKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // Получение ключа согласования получателя импортом эфемерного ключа получателя 
    // на эфемерном ключе отправителя.

    if(CryptImportKey(
	hProvSender, 
	pbKeyBlob, 
	dwBlobLen, 
	hSenderEphemKey, 
	0, 
	&hSenderAgreeKey))
    {
	printf("The responder ephemeral key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey ephemeral key.");
    }

    //--------------------------------------------------------------------
    //  Освобождение памяти, используемой BLOBом.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // Конвертирование ключа согласования в сессионный в контейнере - отправителе.

    if(CryptSetKeyParam(
	hSenderAgreeKey,
	KP_ALGID, 
	(BYTE*)&cAlg,
	0 ))
    {
	printf("The sender agree key has been converted. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }

    //--------------------------------------------------------------------
    // Конвертирование ключа согласования в сессионный в контейнере - получателе.

    if(CryptSetKeyParam(
	hResponderAgreeKey,
	KP_ALGID, 
	(BYTE*)&cAlg,
	0 ))
    {
	printf("The responder agree key has been converted. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }


    //--------------------------------------------------------------------
    // Экспорт копируемого пользовательского ключа  на сессионном.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа ключа и распределение памяти.

    if(CryptExportKey(
	hSenderKey,
	hSenderAgreeKey, 
	PRIVATEKEYBLOB,
	0, 
	NULL,
	&dwBlobLen ))
    {
	printf("Size of the BLOB for the sender key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    if(!pbKeyBlob) 
	HandleError("Out of memory. \n");

    // Экспорт копируемого пользовательского ключа  на сессионном.
    if(CryptExportKey(
	hSenderKey,
	hSenderAgreeKey, 
	PRIVATEKEYBLOB,
	0, 
	pbKeyBlob,
	&dwBlobLen ))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    //--------------------------------------------------------------------
    // Получение копируемого ключа импортом пользовательского ключа на ключе
    // согласования.

    if(CryptImportKey(
	hProvResponder, 
	pbKeyBlob, 
	dwBlobLen, 
	hResponderAgreeKey, 
	0, 
	&hResponderKey))
    {
	printf("The private key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey private key.");
    }

    //--------------------------------------------------------------------
    //  Освобождение памяти, используемой BLOBом.
    free(pbKeyBlob);
    pbKeyBlob = NULL; 
    dwBlobLen = 0;

    //--------------------------------------------------------------------
    // Копирование параметров ключа hSenderKey в параметры ключа hResponderKey
    // для KP_CERTIFICATE.

    //--------------------------------------------------------------------
    // Определение размера BLOBа и распределение памяти.

    if(CryptGetKeyParam( 
	hSenderKey, 
	KP_CERTIFICATE, 
	NULL, 
	&dwBlobLen, 
	0))
    {
	printf("Size of the BLOB determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);

    if(pbKeyBlob)
    {
	printf("Memory has been allocated for the BLOB. \n");
    }
    else
    {
	HandleError("Out of memory. \n");
    }

    //--------------------------------------------------------------------
    // Копирование параметров ключа в BLOB.

    if(CryptGetKeyParam( 
	hSenderKey, 
	KP_CERTIFICATE, 
	pbKeyBlob, 
	&dwBlobLen, 
	0))
    {
	printf("Parameters have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptGetProvParam.");
    }

    //--------------------------------------------------------------------
    // Копирование параметров в hResponderKey.

    if(CryptSetKeyParam(
	hResponderKey, 
	KP_CERTIFICATE, 
	pbKeyBlob, 
	0))
    {
	printf("The parameters have been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetProvParam.");
    }

    CleanUp();
    return 0;
}

void CleanUp(void)
{
    free(pbKeyBlob);

    // Освобождение дескриптора провайдера.
    if(hProvResponder) 
	CryptReleaseContext(hProvResponder, 0);
    if(hProvSender) 
	CryptReleaseContext(hProvSender, 0);

    // Уничтожение дескрипторов ключей.
    if(hSenderKey)
	CryptDestroyKey(hSenderKey);

    if(hResponderKey)
	CryptDestroyKey(hResponderKey);

    if(hSenderEphemKey)
	CryptDestroyKey(hSenderEphemKey);

    if(hResponderEphemKey)
	CryptDestroyKey(hResponderEphemKey);

    if(hSenderAgreeKey)
	CryptDestroyKey(hSenderAgreeKey);

    if(hResponderAgreeKey)
	CryptDestroyKey(hResponderAgreeKey);
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
    CleanUp();
    if(!err) err = 1;
    exit(err);
}

//END-OF-FILE

