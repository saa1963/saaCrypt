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
#   include <stdlib.h>
#   include <string.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>

// Начало примера (не следует удалять данный комментарий, он используется 
// для автоматической сборки документации)
//--------------------------------------------------------------------
// Данный пример демонстрирует экспортирование открытого ключа в файл.
// Эта программа используется для экспортирования как открытого ключа 
// отправителя, так и открытого ключа получателя. 
// Она необходима для работы таких программ, как зашифрование и 
// расшифрование содержимого файла.
// В PUBLICKEYBLOB не является кроссплатформенной структурой и не защищает
// ключи, поэтому рекомендуется использовать сертификаты для обмена открытыми
// ключами. Сертификат также записывается в файл, если он есть в контейнере.
// Для корректной работы этой программы необходимо создать контейнер
// с именем, передаваемым параметром из командной строки.
// Контейнер может быть создан при помощи примера CreatingKeyContainer
// Замечание: под win32 рекомендуется использовать _s аналоги CRT функций.
//--------------------------------------------------------------------

static void WriteBlobToFile(FILE *file, char *szFileName, BYTE *pbData, DWORD cbData);
static void HandleError(char *s);
static void CleanUp(void);

static HCRYPTPROV hProv = 0;		// Дескриптор CSP 
static HCRYPTKEY hKey = 0;		// Дескриптор открытого ключа получателя 
static FILE *Public = NULL;		// Файл для хранения открытого ключа
static BYTE *pbKeyBlob = NULL;		// Указатель на ключевой BLOB 
static FILE *Cert = NULL;		// Файл для хранения сертификата
static BYTE *pbCertBlob = NULL;	// Указатель BLOB сертификата
static char *szFileName = NULL;        // Имя файла для экспорта

int main(int argc, char *argv[])
{
    DWORD cbKeyBlob;		// Длина ключевого BLOBа 
    DWORD cbCertBlob;		// Длина BLOBа сертификата
    char  *szCont = argv[1];	// Короткое имя контейнера

    if (argc < 2 || strchr(szCont, '\\'))
        HandleError("Short container name required.");

    // Получение дескриптора контейнера с именем szCont, 
    // находящегося в рамках провайдера. 
    if(CryptAcquireContext(
	&hProv, 
	szCont, 
	NULL, 
	PROV_GOST_2001_DH, 
	0)) 
    {
	printf("The key container \"%s\" has been acquired.\n", szCont);
    }
    else
    {
        HandleError("Error during CryptAcquireContext.");
    }

    //--------------------------------------------------------------------
    // Получение дескриптора открытого ключа.

    if(CryptGetUserKey(
	hProv, 
	AT_KEYEXCHANGE, 
	&hKey)) 
    {
	printf("The public key has been acquired. \n");
    }
    else
    {
	HandleError("Error during CryptGetUserKey public key.");
    }

    // Запись открытого ключа в ключевой BLOB.
    // Определение размера BLOBа открытого ключа и распределение памяти.
    if(CryptExportKey(
	hKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	NULL,
	&cbKeyBlob)) 
    {
	printf("Size of the BLOB for the public key determined. \n");
    }
    else
    {
	HandleError("Error computing BLOB length.");
    }

    pbKeyBlob = (BYTE*)malloc(cbKeyBlob);
    if(!pbKeyBlob) 
    {
	HandleError("Out of memory. \n");
    }

    // Экспортирование открытого ключа в BLOB открытого ключа.
    if(CryptExportKey(
	hKey, 
	0, 
	PUBLICKEYBLOB, 
	0, 
	pbKeyBlob, 
	&cbKeyBlob))
    {
	printf("Contents have been written to the BLOB. \n");
    }
    else
    {
	HandleError("Error during CryptExportKey.");
    }

    szFileName = malloc((strlen(szCont) + 5) * sizeof(char));
    if(!szFileName) 
    {
	HandleError("Out of memory. \n");
    }
    strcpy(szFileName, szCont);
    strcat(szFileName, ".pub");
    WriteBlobToFile(Public, szFileName, pbKeyBlob, cbKeyBlob);

    // Получение сертификата ключа из контейнера (если он есть)
    if(CryptGetKeyParam(
	hKey,
	KP_CERTIFICATE,
	NULL,
	&cbCertBlob,
	0))
    {
	pbCertBlob = malloc(cbCertBlob);
	if(!pbCertBlob) 
	{
	    HandleError("Out of memory. \n");
	}
	if(CryptGetKeyParam(
	    hKey,
	    KP_CERTIFICATE,
	    pbCertBlob,
	    &cbCertBlob,
	    0))
	{
	    printf("Got certificate from container.\n");
	}
	else
	{
	    HandleError("Error during CryptGetKeyParam.");
	}
	strcpy(szFileName, szCont);
	strcat(szFileName, ".cer");
	WriteBlobToFile(Cert, szFileName, pbCertBlob, cbCertBlob);
    }
    else
    {
	printf("No certificate in container.\n");
    }

    CleanUp();

    printf("The program ran to completion without error. \n");
    return 0;
}

void WriteBlobToFile(FILE *file, char *szFileName, BYTE *pbData, DWORD cbData)
{
    // Открытие файла на запись в него BLOB-а
    //if(!fopen_s(&file, szFileName, "r+b" ))
    if((file = fopen(szFileName, "wb")))
    {
	printf("The file '%s' was opened\n", szFileName);
    }
    else
    {
	HandleError("Problem opening the file\n");
    }

    // Запись BLOB-а в файл
    if(fwrite(pbData, 1, cbData, file))
    {
	printf("The public key was written to the '%s'\n", szFileName);
    }
    else
    {
	HandleError("The public key can not be written to file.\n");
    }
}

void CleanUp(void)
{
    if(Public)
        fclose (Public);

    if(Cert)
	fclose (Cert);

    // Освобождение памяти, используемой ключевым BLOBом.
    free(pbKeyBlob);

    // Освобождение памяти, используемой BLOBом сертификата.
    free(pbCertBlob);

    // Освобождени памяти под имя файла
    free(szFileName);

    // Уничтожение дескриптора открытого ключа.
    if(hKey)
	CryptDestroyKey(hKey);

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


