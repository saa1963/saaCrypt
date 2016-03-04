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
// В данном примере производится зашифрование файла на открытом ключе
// получателя.

// Для корректной работы данного примера предварительно необходимо 
// записать открытый ключ получателя в файл "Responder.pub" при помощи
// примера ExportInFile.
// Также предварительно необходимо создать файл, подлежаший зашифрованию, 
// с именем "source.txt".
// Следует сделить за тем, чтобы контейнеры "Sender" и "Responder", 
// используемые в программах ExportInFile, EncryptFile и DecryptFile на момент
// выполнения этих программ существовали.
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

#define BLOCK_LENGTH 4096

static void LoadPublicKey(HCRYPTPROV hProv, BYTE *pbBlob, DWORD *pcbBlob, char *szCertFile, char *szKeyFile);
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// Дескриптор CSP 
static HCRYPTKEY hKey = 0;		// Дескриптор закрытого ключа 
static HCRYPTKEY hSessionKey = 0;	// Дескриптор сессионного ключа
static HCRYPTKEY hAgreeKey = 0;        // Дескриптор ключа согласования

static FILE *certf=NULL;		// Файл, в котором хранится сертификат
static FILE *publicf=NULL;		// Файл, в котором хранится открытый ключ
static FILE *source=NULL;		// Исходный файл
static FILE *Encrypt=NULL;		// Зашифрованный файл
static FILE *session_SV=NULL;// Файл для хранения сессионного ключа (вектор инициализации)
static FILE *session_EncryptedKey=NULL;// Файл для хранения сессионного ключа (зашифрованный ключ)
static FILE *session_MacKey=NULL;	// Файл для хранения сессионного ключа (имита)
static FILE *vectorf=NULL;		// Файл для хранения вектора инициализации

static BYTE *pbKeyBlobSimple = NULL;   // Указатель на сессионный ключевой BLOB 
static BYTE *pbIV = NULL;		// Вектор инициализации сессионного ключа

int main(void)
{
    BYTE  pbKeyBlob[100];	// Указатель на ключевой BLOB 
    DWORD dwBlobLen = 100;	// Длина ключевого BLOBа
    DWORD dwBlobLenSimple;      // Длина сессионного ключевого BLOBа

    BYTE pbContent[BLOCK_LENGTH];	// Указатель на содержимое исходного файла
    DWORD cbContent = 0;	// Длина содержимого
    DWORD dwIV = 0;		// Длина вектора инициализации
    DWORD bufLen = sizeof(pbContent);   // Длина буфера
    ALG_ID ke_alg = CALG_PRO_EXPORT; // PRO_EXPORT алгоритм ключа согласования

    // Открытие файла, который будет зашифрован.
    //if(!fopen_s(&source, "source.txt", "r+b" ))
    if(!(source = fopen("source.txt", "rb")))
	HandleError( "Problem opening the file 'source.txt'\n" );
    printf( "The file 'source.txt' was opened\n" );

    // Открытие файла, в который будет производится запись зашифрованного файла.
    //if(!fopen_s(&Encrypt, "encrypt.bin", "wb"))
    if(!(Encrypt = fopen("encrypt.bin", "wb")))
	HandleError( "Problem opening the file 'encrypt.bin'\n" );
    printf( "The file 'encrypt.bin' was opened\n" );

    // Открытие файла, в который производится запись синхропосылки.
    //if(!fopen_s(&session_SV, "session_SV.bin", "wb"))
    if(!(session_SV = fopen("session_SV.bin", "wb")))
	HandleError( "Problem opening the file 'session_SV.bin'\n" );
    printf( "The file 'session_SV.bin' was opened\n" );

    // Открытие файла, в который производится запись сессионного ключа.
    //if(!fopen_s(&session_EncryptedKey, "session_EncryptedKey.bin", "wb"))
    if(!(session_EncryptedKey = fopen("session_EncryptedKey.bin", "wb")))
	HandleError( "Problem opening the file 'session_EncryptedKey.bin'\n" );
    printf( "The file 'session_EncryptedKey.bin' was opened\n" );

    // Открытие файла, в который производится запись MAC сессионного ключа.
    //if(!fopen_s(&session_MacKey, "session_MacKey.bin", "wb" ))
    if(!(session_MacKey = fopen("session_MacKey.bin", "wb")))
	HandleError( "Problem opening the file 'session_MacKey.bin'\n" );
    printf( "The file 'session_MacKey.bin' was opened\n" );

    // Открытие файла, в который производится запись вектора инициализации.
    //if(!fopen_s(&vectorf, "vector.bin", "wb" ))
    if(!(vectorf = fopen("vector.bin", "wb")))
	HandleError( "Problem opening the file 'vector.bin'\n" );
    printf( "The file 'vector.bin' was opened\n" );

    // Получение дескриптора контейнера получателя с именем "Sender", 
    // находящегося в рамках провайдера. 
    if(CryptAcquireContext(
	&hProv, 
	"Sender", 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"Sender\" has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptAcquireContext.");
    }

    LoadPublicKey(hProv, pbKeyBlob, &dwBlobLen, "Responder.cer", "Responder.pub");

    // Получение дескриптора закрытого ключа отправителя.
    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {printf("The private key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey private key.");
    }

    // Получение ключа согласования импортом открытого ключа получателя
    // на закрытом ключе отправителя.

    if(CryptImportKey(
	hProv, 
	pbKeyBlob, 
	dwBlobLen, 
	hKey, 
	0, 
	&hAgreeKey))
    {
	printf("The responder public key has been imported. \n");
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

    // Генерация сессионного ключа.
    if(CryptGenKey(     
	hProv,      
	CALG_G28147,      
	CRYPT_EXPORTABLE, 
	&hSessionKey))
    {   
	printf("Original session key is created. \n");
    }
    else
    {
	HandleError("ERROR -- CryptGenKey.");
    }

    //--------------------------------------------------------------------
    // Зашифрование сессионного ключа.
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Определение размера BLOBа сессионного ключа и распределение памяти.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey, 
	SIMPLEBLOB, 
	0, 
	NULL,
	&dwBlobLenSimple)) 
    {
	printf("Size of the BLOB for the sender session key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlobSimple = (BYTE*)malloc(dwBlobLenSimple);

    if(!pbKeyBlobSimple) 
	HandleError("Out of memory. \n");

    // Зашифрование сессионного ключа на ключе Agree.

    if(CryptExportKey(
	hSessionKey, 
	hAgreeKey,
	SIMPLEBLOB, 
	0, 
	pbKeyBlobSimple, 
	&dwBlobLenSimple))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    // Определение размера вектора инициализации сессионного ключа. 
    if(CryptGetKeyParam(
	hSessionKey, 
	KP_IV, 
	NULL, 
	&dwIV, 
	0))
    {
	printf("Size of the IV for the session key determined. \n");
    }
    else
    {
	HandleError("Error computing IV length.");
    }

    pbIV = (BYTE*)malloc(dwIV);
    if (!pbIV)
	HandleError("Out of memory. \n");

    // Определение вектора инициализации сессионного ключа.
    if(CryptGetKeyParam(
	hSessionKey, 
	KP_IV, 
	pbIV, 
	&dwIV, 
	0))
    {
	printf( "CryptGetKeyParam succeeded. \n");
    }
    else
    {
	HandleError("Error during CryptGetKeyParam.");
    }


    //--------------------------------------------------------------------
    // Запись вектора инициализации в файл.

    if(fwrite(
	pbIV,
	1,
	dwIV,
	vectorf))
    {
	printf( "The IV was written to the 'vector.bin'\n" );
    }
    else
    {
	HandleError( "The IV can not be written to the 'vector.bin'\n" );
    }  

    //--------------------------------------------------------------------
    // Запись зашифрованного сессионного ключа в файл.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bSV,
	1,
	SEANCE_VECTOR_LEN,
	session_SV))
    {
	printf( "The session key was written to the 'session_SV.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session_SV.bin'\n" );
    }

    //--------------------------------------------------------------------
    // Запись зашифрованного сессионного ключа в файл.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bEncryptedKey,
	1,
	G28147_KEYLEN,
	session_EncryptedKey))
    {
	printf( "The session key was written to the 'session.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session.bin'\n" );
    }

    //--------------------------------------------------------------------
    // Запись зашифрованного сессионного ключа в файл.

    if(fwrite(
	((CRYPT_SIMPLEBLOB*)pbKeyBlobSimple)->bMacKey,
	1,
	EXPORT_IMIT_SIZE,
	session_MacKey))
    {
	printf( "The session key was written to the 'session.bin'\n" );
    }
    else
    {
	HandleError( "The session key can not be written to the 'session.bin'\n" );
    }

    //--------------------------------------------------------------------
    // Чтение  файла, который будет зашифрован блоками по 4 КБ. Зашифрование 
    // прочитанного блока и запись его в файл "encrypt.bin".
    //--------------------------------------------------------------------

    do 
    {
	cbContent = (DWORD)fread(pbContent, 1, BLOCK_LENGTH, source);
	if(cbContent)
	{
	    BOOL bFinal = feof(source);
	    // Зашифрование прочитанного блока на сессионном ключе.
	    if(CryptEncrypt(
		hSessionKey,
		0,
		bFinal,
		0,
		pbContent,
		&cbContent,
		bufLen))
	    {
		printf( "Encryption succeeded. \n");
		// Запись зашифрованного блока в файл.
		if(fwrite(
		    pbContent,
		    1,
		    cbContent,
		    Encrypt))
		{
		    printf( "The encrypted content was written to the 'encrypt.bin'\n" );
		}
		else
		{
		    HandleError( "The encrypted content can not be written to the 'encrypt.bin'\n" );
		}  
	    }
	    else
	    {
		HandleError("Encryption failed.");
	    }
	}
	else
	{
	    HandleError( "Problem reading the file 'source.txt'\n" );
	}
    }
    while (!feof(source));

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}

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
    if(source)
        fclose (source);
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

    if(pbKeyBlobSimple)
	free(pbKeyBlobSimple);
    if(pbIV)
	free(pbIV);
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
