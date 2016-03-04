/*
 * Copyright(C) 2000 Проект ИОК
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
 */

#if !defined( _READER_FAT12_FAT12INT_H )
#define _READER_FAT12_FAT12INT_H /*+ Запрещение двойного включения. +*/

#include <tchar.h> /*+ Windows or project (READER) windows include. 
    Универсальные функции работы со строками. +*/

#include "reader/rdr.h" /*+ Project (READER) include.
    Внешний интерфейс работы со считывателями. +*/
#include "reader.kit/reader.h" /*+ Project (READER) include. 
    Определения необходимые для создания библиотеки считывателя. +*/

#ifdef UNIX
#   define _POSIX_PTHREAD_SEMANTICS 1
#   include <dirent.h>
#endif /* UNIX */

#define DIR_NOTEXIST 0
#define DIR_EXIST 1
#define DIR_INVALID 2

/*++++
 * Функция преобразования символа уникального номера.
 ++++*/
static SUP_INLINE TCHAR fat12_convert( int c )
{
    if( c >= 'a' && c <= 'z' )
	return (TCHAR)c;
    if( c >= 'A' && c <= 'Z' )
	return (TCHAR)c;
    if( c >= '0' && c <= '9' )
	return (TCHAR)c;
    if( c == '-' )
	return (TCHAR)c;
    return (TCHAR)( ( (unsigned char)c ) % ( 'z' - 'a' + 1 ) + 'a' );
}

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

#if !defined( _RESOURCE )
/*+ Заменитель ресурса. +*/
extern TSupResourceInstance FAT12_RESOURCE;
#else
#define FAT12_RESOURCE FAT12_DLL_INSTANCE
extern HINSTANCE FAT12_DLL_INSTANCE;
#endif /* !defined( _RESOURCE ) */

#define FAT12_FLAG_HDIMAGE 0x1

/*+ Структура внутреннего контекста дискеты в формате fat12. +*/
typedef struct {
    TCHAR *path; /*+ Имя дисковода. 
	Для A: "A", для B: "B", для removable соответсвующее имя. +*/
#if defined _WIN32
    HANDLE handle;
    DWORD access; /*!< Способ доступа к файлу. */
#else
    FILE *handle;
#endif /* _WIN32 */
    unsigned long volume_serial;
    TCHAR *volume_label;
    TCHAR *file_name; /*!< Имя открытого файла. */
    TCHAR *folder;
    TCHAR *path_to_item;
    unsigned flags;
    int machine;
    TSupSysENickname nickname;
    TSupSysENickname connect;
#if defined(UNIX)
    TSupportMutex lock;
    size_t n_lock;
#endif /* UNIX */
} TFat12Context;

typedef struct TFat12FolderEnumNew_
{
    TCHAR *full_prefix;
    TCHAR prefix[10];
    size_t num;
} TFat12FolderEnumNew;

/*++++
 * Функция получения кода ошибки в формате драйвера по коду ошибки ОС.
 ++++*/
TSupErr fat12_os_error( void );

TSupErr fat12_volume_name( TFat12Context *ctx, const TCHAR *path, TCHAR **name, 
    unsigned long *serial );

TSupErr fat12_set_volume_label(TFat12Context *ctx, TCHAR *label);

TSupErr fat12_unique_form( unsigned long volume_serial, const TCHAR *vname,
    size_t *length, TCHAR *unique );

int fat12_is_dir_exist( const TCHAR *path );

TSupErr hdimage_path_default( 
    TCHAR **str, int machine );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_FAT12_FAT12INT_H ) */
/*+ end of file:$Id: fat12int.h 75486 2012-03-04 12:45:49Z dim $ +*/
