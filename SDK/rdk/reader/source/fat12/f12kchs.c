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

/****
 * $Id: f12kchs.c 24518 2005-03-15 15:20:21Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции чтения / записи ключевой информации с ключевой дискеты.
 * Функция изменеия размера файла.
 *
 * Функция: fat12_write
 ****/

#include "fat12prj.h"
#if defined(UNIX)
#include <unistd.h>
#endif /* UNIX */

#ifdef UNIX
static TSupErr set_file_size(FILE *handle, size_t size)
{
  if(!ftruncate(fileno(handle),size)) return SUP_ERR_NO;
  /* Shit, ftruncate didn't work, we need manual intervention */
  if(fseek(handle, size, SEEK_SET)==-1) return fat12_os_error(); 

  return 0;
}
#else /* UNIX */
static TSupErr set_file_size(HANDLE handle, size_t size)
{
    SUPSYS_PRE( size <= MAXLONG );
    if( SetFilePointer( handle, (long)size, NULL, FILE_BEGIN ) 
	== INVALID_SET_FILE_POINTER )
	    return fat12_os_error();
    if( !SetEndOfFile( handle ) )
	    return fat12_os_error();

  return SUP_ERR_NO;
} 
#endif /* UNIX */


/*++++ 
 * Функция изменения размера файла. 
 ++++*/
TSupErr fat12_chsize( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoChSize *inf = (TReaderInfoChSize*)info;
    TFat12Context *ctx = (TFat12Context*)context;
    TSupErr code;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoChSize );

    inf->size_of = sizeof( TReaderInfoChSize );
#ifdef UNIX
    if( ctx->handle == NULL )
#else
    if( ctx->handle == INVALID_HANDLE_VALUE )
#endif
	return RDR_ERR_MEDIA_CHANGED;

    code = set_file_size(ctx->handle, inf->length);
    if( code ) return code;

    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kchs.c 24518 2005-03-15 15:20:21Z dim $ +*/
