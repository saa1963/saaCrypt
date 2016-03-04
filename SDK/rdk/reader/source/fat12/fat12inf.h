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

#if !defined( _READER_FAT12_FAT12INF_H )
#define _READER_FAT12_FAT12INF_H /*+ Запрещение двойного включения. +*/

#include "reader/rdr.h" /*+ Project (READER) include.
    Внешний интерфейс работы со считывателями. +*/
#include "reader.kit/reader.h" /*+ Project (READER/RDR) include.
    Определения необходимые для создания библиотеки считывателя. +*/

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/*+ Номера поддерживаемых info-функций. +*/
extern const TSupSysInfoFun FAT12_INFO_SUPPORT[];

extern const size_t FAT12_INFO_SUPPORT_LEN;

#if defined( _WIN32 )
#   ifndef UNDER_CE
typedef struct TFat12ConnectEnum_
{
    TCHAR *drive_strings;
    TCHAR *cur;
} TFat12ConnectEnum;
#   else
typedef struct TFat12ConnectEnum_
{
    HANDLE hSearch;         // Search handle returned by FindFirstFile
    WIN32_FIND_DATA  fd;    // Data structure describes the file found
} TFat12ConnectEnum;
#   endif

#if 0
// TODO: clean out
typedef struct Fat12EnumReader_
{
    HANDLE hFind;
    TCHAR *volume_name;
} Fat12EnumReader;
#endif

#elif defined UNIX
  typedef struct TFat12ConnectEnumElem_ TFat12ConnectEnumElem;
  struct TFat12ConnectEnumElem_
  {
    TCHAR *connect_str;
    TFat12ConnectEnumElem *next;
  };
  
  typedef struct TFat12ConnectEnum_
  {
    TFat12ConnectEnumElem *drive_strings;
    TFat12ConnectEnumElem *cur;
  } TFat12ConnectEnum;
#endif /* _WIN32 */
  
TSupErr fat12_info_system_flag(
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_group_info_system_flag(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_group_enum_open(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_group_enum_next(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_group_enum_close(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_nickname(
   TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_name(
   TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_register( 
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_unregister( 
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_context_dup( 
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_context_free( 
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_info_same_media(
    TSupSysContext *context, TSupSysInfo* );

TSupErr fat12_info_instance(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_connect( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_disconnect( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция открытия файла. */
TSupErr fat12_open( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция записи в файл. */
TSupErr fat12_write( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция чтения из файла. */
TSupErr fat12_read( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция закрытия файла. */
TSupErr fat12_close( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция получения длины файла. */
TSupErr fat12_length(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция удаления файла. */
TSupErr fat12_unlink( 
    TSupSysContext *context, TSupSysInfo *info );

/* Функция установления соединения с носителем. */
TSupErr fat12_connect_carrier( 
    TSupSysContext *context, TSupSysInfo* );

/* Функция уничтожения контекста считывателя. */
TSupErr fat12_disconnect_carrier(
    TSupSysContext *context, TSupSysInfo* );

#if !defined UNIX
/* Функция получения уникального номера. */
TSupErr fat12_unique_get(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция установки уникального номера. */
TSupErr fat12_unique_set(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_aux_call(
    TSupSysContext *context, TSupSysInfo *info );
#endif /* !UNIX */

TSupErr fat12_restore_context( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_folder_enum_open( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_folder_enum_next( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_folder_enum_close( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_locked_folder_enum_open( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_locked_folder_enum_next( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_info_locked_folder_enum_close( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_folder_open( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_folder_close( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_folder_clear( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_lock( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_unlock( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr fat12_chsize( 
    TSupSysContext *context, TSupSysInfo *info );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_FAT12_FAT12INF_H ) */
