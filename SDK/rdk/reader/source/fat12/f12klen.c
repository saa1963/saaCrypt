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
 * $Id: f12klen.c 75486 2012-03-04 12:45:49Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции чтения / записи ключевой информации с ключевой дискеты.
 * Функция получения длины файла.
 *
 * Функция: fat12_length
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*++++
 * Функция получения длины файла.
 ++++*/
TSupErr fat12_length(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoLength *inf = (TReaderInfoLength*)info;
    TFat12Context *ctx = (TFat12Context*)context;
#ifndef UNIX
    BY_HANDLE_FILE_INFORMATION finfo;
#endif /* UNIX */

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoLength );

    inf->size_of = sizeof( TReaderInfoLength );
#ifdef UNIX
    if( fseek( ctx->handle, 0, SEEK_END ) )
	return fat12_os_error();
    inf->length = (size_t)ftell( ctx->handle );
    if( inf->length == (size_t)-1 )
	return fat12_os_error();
#else /* UNIX */
    if( !GetFileInformationByHandle( ctx->handle, &finfo ) )
	return fat12_os_error();
    inf->length = finfo.nFileSizeLow;
    if( finfo.nFileSizeHigh )
	inf->length = (size_t)-1;
#endif /* UNIX */
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12klen.c 75486 2012-03-04 12:45:49Z dim $ +*/
