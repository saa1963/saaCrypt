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
 * $Id: f12kwri.c 44147 2008-01-10 14:18:38Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции чтения / записи ключевой информации с ключевой дискеты.
 * Функция записи в файл.
 *
 * Функция: fat12_write
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/
#include "limits.h" 
/*++++ 
 * Функция записи в файл. 
 ++++*/
TSupErr fat12_write( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoWrite *inf = (TReaderInfoWrite*)info;
    TFat12Context *ctx = (TFat12Context*)context;

#ifdef UNIX
    size_t loc_writen;
#else /* UNIX */
    DWORD loc_writen = 0;
    void *buffer = (void*)inf->info.info;
#endif /* UNIX */

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoWrite );
    SUPSYS_PRE( inf->from <= LONG_MAX );
    SUPSYS_PRE( inf->info.length <= LONG_MAX );

    SUPSYS_PRE_READ_PTRS( inf->info.info, inf->info.length );

    inf->size_of = sizeof( TReaderInfoWrite );
#ifdef UNIX
    if( ctx->handle == NULL )
#else
    if( ctx->handle == INVALID_HANDLE_VALUE )
#endif
	return RDR_ERR_MEDIA_CHANGED;
#ifdef UNIX
    if( fseek( ctx->handle, inf->from, SEEK_SET ) )
	return fat12_os_error();
    loc_writen = fwrite( inf->info.info, 1, inf->info.length, ctx->handle );
    if( loc_writen != inf->info.length )
	return fat12_os_error();
#else /* UNIX */
    if( SetFilePointer( ctx->handle, (LONG)inf->from, NULL, 
	FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	return fat12_os_error();
    if( !WriteFile( ctx->handle, buffer, (LONG)inf->info.length, &loc_writen, 
	    NULL ) )
	return fat12_os_error();
#endif /* UNIX */
    inf->info.length -= loc_writen;
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kwri.c 44147 2008-01-10 14:18:38Z dim $ +*/
