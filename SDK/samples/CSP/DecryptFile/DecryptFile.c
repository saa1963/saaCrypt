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
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Данный пример демонстрирует расшифрование файла на сессионном ключе.

// Для корректной работы этого примера передаваемые ему входные данные
// должны являться результатом работы примера EncryptFile.
// Зашифрованный сессионный ключ содержится в файле "session_EncryptedKey.bin",
// синхропосылка для согласования ключей - в "session_SV.bin",
// MAC зашифрованного сессионного ключа - в "session_MacKey.bin".
// Для его расшифрования необходим открытый ключ отправителя, содержащийся
// в файле "Sender.pub" (данный файл создается при выполнении примера 
// ExportInFile). 
// Для корректной работы этой программы необходимо получить зашифрованный
// файл с именем "encrypt.bin". Для правильного расшифрования файла
// необходим вектор инициализации, который хранится в файле "vector.bin".
// Расшифрование производится в файл "destination.txt".
// Следует сделить за тем, чтобы контейнеры "Sender" и "Responder", 
// используемые в программах ExportInFile, EncryptFile и DecryptFile на момент
// выполнения этих программ существовали.
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

#define BLOCK_LENGTH 4096

static void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile);
static void CleanUp(void);
static void HandleError(char *s);

static HCRYPTPROV hProv = 0;		// Дескриптор CSP
static HCRYPTKEY hKey = 0;		// Дескриптор закрытого ключа 
static HCRYPTKEY hSessionKey = 0;	// Дескриптор сессионного ключа
static HCRYPTKEY hAgreeKey = 0;        // Дескриптор ключа согласования

static FILE *Encrypt=NULL;		// Зашифрованный файл
static FILE *certf=NULL;		// Файл, в котором хранится сертификат
static FILE *publicf=NULL;		// Файл, в котором хранится открытый ключ
static FILE *session_SV=NULL;// Файл для хранения сессионного ключа (вектор инициализации)
static FILE *session_EncryptedKey=NULL;// Файл для хранения сессионного ключа (зашифрованный ключ)
static FILE *session_MacKey=NULL;	// Файл для хранения сессионного ключа (имита)
static FILE *vectorf=NULL;		// Файл для хранения вектора инициализации
static FILE *destination=NULL;		// Расшифрованный файл

int main(void)
{
    BYTE  pbKeyBlob[100];	// Указатель на ключевой BLOB 
    DWORD dwBlobLen = 100;	// Длина ключевого BLOBа
    BYTE *pbKeyBlobSimple = NULL;  // Указатель на сессионный ключевой BLOB 
    DWORD dwBlobLenSimple;	// Длина сессионного ключевого BLOBа
    BYTE pbIV[100];		// Вектор инициализации сессионного ключа
    DWORD dwIV = 0;		// Длина вектора инициализации
    BYTE pbContent[BLOCK_LENGTH];	// Указатель на содержимое зашифрованного файла
    DWORD cbContent = 0;	// Длина содержимого
    ALG_ID ke_alg = CALG_PRO_EXPORT; // PRO_EXPORT алгоритм ключа согласования
    CRYPT_SIMPLEBLOB_HEADER tSimpleBlobHeaderStandart;
    BYTE bEncryptionParamSetStandart[] = {0x30, 0x09, 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x1f, 0x01}; //неменяемая часть блоба

    tSimpleBlobHeaderStandart.BlobHeader.aiKeyAlg = CALG_G28147; 
    tSimpleBlobHeaderStandart.BlobHeader.bType = SIMPLEBLOB;
    tSimpleBlobHeaderStandart.BlobHeader.bVersion = BLOB_VERSION;
    tSimpleBlobHeaderStandart.BlobHeader.reserved = 0;
    tSimpleBlobHeaderStandart.EncryptKeyAlgId = CALG_G28147;
    tSimpleBlobHeaderStandart.Magic = G28147_MAGIC;    

    // Открытие файла, в котором хранится зашифрованный файл.
    //if(!fopen_s( &Encrypt, "encrypt.bin", "r+b" ))
    if(!(Encrypt = fopen("encrypt.bin", "r+b" )))
	HandleError( "Problem opening the file 'encrypt.bin'\n" );
    printf( "The file 'encrypt.bin' was opened\n" );

    // Открытие файла, в который производится запись сессионного ключа.
    if(!(session_SV = fopen("session_SV.bin", "r+b")))
	HandleError( "Problem opening the file 'session_SV.bin'\n" );
    printf( "The file 'session_SV.bin' was opened\n" );

    // Открытие файла, в который производится запись сессионного ключа.
    if(!(session_EncryptedKey = fopen("session_EncryptedKey.bin", "r+b")))
	HandleError( "Problem opening the file 'session_EncryptedKey.bin'\n" );
    printf( "The file 'session_EncryptedKey.bin' was opened\n" );

    // Открытие файла, в который производится запись сессионного ключа.
    if(!(session_MacKey = fopen("session_MacKey.bin", "r+b")))
	HandleError( "Problem opening the file 'session_MacKey.bin'\n" );
    printf( "The file 'session_MacKey.bin' was opened\n" );

    // Открытие файла, в котором хранится вектор инициализации.
    //if(!fopen_s(&vectorf, "vector.bin", "r+b" ))
    if(!(vectorf = fopen("vector.bin", "r+b")))
	HandleError( "Problem opening the file 'vector.bin'\n" );
    printf( "The file 'vector.bin' was opened\n" );

    // Открытие файла, в который будет записан расшифрованный файл.
    //if(!fopen_s(&destination, "destination.txt", "w+b" ))
    if(!(destination = fopen("destination.txt", "w+b" )))
	HandleError( "Problem opening the file 'destination.txt'\n" );
    printf( "The file 'destination.txt' was opened\n" );

    // Чтение  вектора инициализации из файла.
    dwIV = (DWORD)fread(pbIV, 1, 100, vectorf);
    if(!dwIV)
	HandleError( "The IV can not be reading from the 'vector.bin'\n" );
    printf( "The IV was read from the 'vector.bin'\n" );

    // Получение дескриптора контейнера получателя с именем "Responder", 
    // находящегося в рамках провайдера. 
    if(!CryptAcquireContext(
	&hProv, 
	"Responder",
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	HandleError("Error during CryptAcquireContext");
    }
    printf("The key container \"Responder\" has been acquired. \n");

    pbKeyBlobSimple = malloc((sizeof(CRYPT_SIMPLEBLOB_HEADER)+SEANCE_VECTOR_LEN+G28147_KEYLEN+EXPORT_IMIT_SIZE) + sizeof(bEncryptionParamSetStandart));
    if(!pbKeyBlobSimple) 
	HandleError("Out of memory. \n");

    memcpy(&((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->tSimpleBlobHeader, &tSimpleBlobHeaderStandart, sizeof(CRYPT_SIMPLEBLOB_HEADER)); //копируем неменяемый хедер в блобе

    // Чтение  сесcионного ключа из файла.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bSV, 1, SEANCE_VECTOR_LEN, session_SV);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_SV.bin'\n" );
    printf( "The session key was read from the 'session_SV.bin'\n" );

    // Чтение  сесcионного ключа из файла.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptedKey, 1, G28147_KEYLEN, session_EncryptedKey);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_EncryptedKey.bin'\n" );
    printf( "The session key was read from the 'session_EncryptedKey.bin'\n" );

    // Чтение  сесcионного ключа из файла.
    dwBlobLenSimple = (DWORD)fread(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bMacKey, 1, EXPORT_IMIT_SIZE, session_MacKey);
    if(!dwBlobLenSimple)
	HandleError( "The session key can not be reading from the 'session_MacKey.bin'\n" );
    printf( "The session key was read from the 'session_MacKey.bin'\n" );

    memcpy(((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptionParamSet, bEncryptionParamSetStandart, 11); //копируем неменяемую ASN структуру

    LoadPublicKey(hProv, pbKeyBlob, &dwBlobLen, "Sender.cer", "Sender.pub");

    // Получение дескриптора закрытого ключа получателя.
    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {
	printf("The private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    // Получение ключа согласования импортом открытого ключа отправителя 
    // на закрытом ключе получателя.
    if(CryptImportKey(
	hProv, 
	pbKeyBlob, 
	dwBlobLen, 
	hKey, 
	0, 
	&hAgreeKey))
    {
	printf("The sender public key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey public key.");
    }

    // Установление PRO_EXPORT алгоритма ключа согласования
    if(CryptSetKeyParam(
        hAgreeKey,
        KP_ALGID,
        (LPBYTE)&ke_alg,
        0))
    {
	printf("PRO_EXPORT agree key algorithm has been set. \n");
    }
    else
    {
	HandleError("Error during CryptSetKeyParam agree key.");
    }

    // Получение сессионного ключа импортом зашифрованного сессионного ключа 
    // на ключе Agree.
    if(CryptImportKey(
	hProv, 
	pbKeyBlobSimple, 
	71, 
	hAgreeKey, 
	0, 
	&hSessionKey))
    {
	printf("The session key has been imported. \n");
    }
    else
    {
	HandleError("Error during CryptImportKey session key.");
    }

    // Установка вектора инициализации - без него первые 8 байт 
    // расшифруются неправильно.
    if(!CryptSetKeyParam(
	hSessionKey, 
	KP_IV, 	
	pbIV, 
	0))
    {
	HandleError("Error during CryptSetKeyParam.");
    }
    printf( "CryptSetKeyParam succeeded. \n");

    // Чтение  зашифрованного файла блоками по 4 КБ. Расшифрование прочитанного
    // блока и запись его в файл "destination.txt".
    do 
    {
	cbContent = (DWORD)fread(pbContent, 1, BLOCK_LENGTH, Encrypt);
	if(cbContent)
	{
	    BOOL bFinal = feof(Encrypt);
	    // Расшифрование прочитанного блока на сессионном ключе.
	    if(CryptDecrypt(
		hSessionKey,
		0,
		bFinal,
		0,
		pbContent,
		&cbContent))
	    {
		printf( "Decryption succeeded. \n");
		// Запись расшифрованного блока в файл.
		if(fwrite(
		    pbContent,
		    1,
		    cbContent,
		    destination))
		{
		    printf( "The decrypted content was written to the 'destination.txt'\n" );
		}
		else
		{
		    HandleError( "The decrypted content can not be written to the 'destination.txt'\n" );
		}  
	    }
	    else
	    {
		HandleError("Decryption failed.");
	    }
	}
	else
	{
	    HandleError( "Problem reading the file 'encrypt.bin'\n" );
	}
    }
    while(!feof(Encrypt));

    CleanUp();

    printf("The program ran to completion without error. \n");
    
    free(pbKeyBlobSimple);
    return 0;
}// Конец main

// Загрузка PUBLICKEYBLOB из сертификата, если он есть, (предпочтительно) или
// из блоба, записанного в файл.
void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile)
{
    // Открытие файла, в котором содержится открытый ключ получателя.
    //if(fopen_s(&certf, szCertFile, "r+b" ))
    if((certf = fopen(szCertFile, "rb"))) {
	DWORD cbCert = 2000;
	BYTE  pbCert[2000];
	PCCERT_CONTEXT pCertContext = NULL;
	HCRYPTKEY hPubKey;
	printf( "The file '%s' was opened\n", szCertFile );

	cbCert = (DWORD)fread(pbCert, 1, cbCert, certf);
	if(!cbCert)
	    HandleError( "Failed to read certificate\n" );
	printf( "Certificate was read from the '%s'\n", szCertFile );

	pCertContext = CertCreateCertificateContext (
	    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pbCert, cbCert);
	if (!pCertContext) {
	    HandleError( "CertCreateCertificateContext" );
	}

	// Импортируем открытый ключ
	if (CryptImportPublicKeyInfoEx(
	    hProv, 
	    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
	    &(pCertContext->pCertInfo->SubjectPublicKeyInfo),
	    0,
	    0,
	    NULL,
	    &hPubKey))
	{
	    printf("Public key imported from cert file\n");
	} else {
	    CertFreeCertificateContext(pCertContext);
	    HandleError( "CryptImportPublicKeyInfoEx" );
	}
	CertFreeCertificateContext(pCertContext);

	// Экспортируем его в BLOB
	if (CryptExportKey(
	    hPubKey,
	    0,
	    PUBLICKEYBLOB,
	    0,
	    pbBlob,
	    pcbBlob))
	{
	    printf("Public key exported to blob\n");
	}
	else
	{
	    HandleError( "CryptExportKey" );
	}
    } else {
	// Открытие файла, в котором содержится открытый ключ получателя.
	//if(!fopen_s(&publicf, szKeyFile, "r+b" ))
	if(!(publicf = fopen(szKeyFile, "rb")))
	    HandleError( "Problem opening the public key blob file\n" );
	printf( "The file '%s' was opened\n", szKeyFile );

	*pcbBlob = (DWORD)fread(pbBlob, 1, *pcbBlob, publicf);
	if(!*pcbBlob)
	    HandleError( "Failed to read key blob file\n" );
	printf( "Key blob was read from the '%s'\n", szKeyFile );
    }
}

void CleanUp(void)
{
    if(Encrypt)
        fclose (Encrypt);
    if(session_SV)
	fclose (session_SV);
    if(session_EncryptedKey)
	fclose (session_EncryptedKey);
    if(session_MacKey)
	fclose (session_MacKey);
    if(vectorf)    
        fclose (vectorf);
    if(destination)    
        fclose (destination);
    if(certf)
	fclose (certf);
    if(publicf)
	fclose (publicf);

    // Уничтожение дескриптора закрытого ключа.
    if(hKey)
	CryptDestroyKey(hKey);

    // Уничтожение дескриптора сессионного ключа.
    if(hSessionKey)
	CryptDestroyKey(hSessionKey);

    // Уничтожение дескриптора ключа согласования.
    if(hAgreeKey)
	CryptDestroyKey(hAgreeKey);

    // Освобождение дескриптора провайдера.
    if(hProv) 
	CryptReleaseContext(hProv, 0);
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





