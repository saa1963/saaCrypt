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
 * $Id: f12uset.c 54220 2009-05-02 02:33:10Z lse $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции поддержки уникального идентификатора носителя.
 * Функция записи уникального идентификатора носителя.
 *
 * Функция: fat12_unique_set
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

#define LENGTH 8 + 1 + 11
/* Функция установки уникального номера. */
TSupErr fat12_unique_set(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TCHAR volume_name[11 + 1]; /* Метка тома. */
    TCHAR cur_unique[LENGTH + 1];
    size_t i = 0;
    TReaderInfoUniqueSet *inf = (TReaderInfoUniqueSet*)info;
    TFat12Context *ctx = (TFat12Context*)context;
    TCHAR *buffer;
    TCHAR *new_label;
    TCHAR *ptr;
    size_t length = LENGTH;
    TSupErr code;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoUniqueGet );
    SUPSYS_PRE_STRING_PTRS( inf->unique.text, inf->unique.length );

    inf->size_of = sizeof( TReaderInfoUniqueGet );
    buffer = inf->unique.text;
    if( ctx->volume_serial != 0 || *ctx->volume_label )
	return SUP_ERR_NO;
    fat12_unique_form( ctx->volume_serial, ctx->volume_label,
	&length, cur_unique );
    if( !_tcsnicmp( buffer, cur_unique, 8 ) )
    {
	if( _tcslen( buffer ) > 8 )
	    ptr = buffer + 8 + 1;
	else
	    ptr = buffer + 8;
    }
    else
	ptr = buffer;

    for( ; i < 11 && ptr[i] ; i++ ) volume_name[i] = fat12_convert( ptr[i] );
    volume_name[i] = 0;
	
    new_label = malloc( ( _tcslen( volume_name ) + 1 ) * sizeof( TCHAR ) );
    if( new_label == NULL )
    {
	return SUP_ERR_MEMORY;
    }
    _tcscpy( new_label, volume_name );
    code = fat12_set_volume_label(ctx, new_label);
    if (code){
	free( new_label );
	return code; 
    }
    free( ctx->volume_label );
    ctx->volume_label = new_label;

    return SUP_ERR_NO;
}
