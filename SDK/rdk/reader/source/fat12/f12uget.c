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

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*++++
 * Функция получения уникального номера.
 ++++*/
TSupErr fat12_unique_get(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoUniqueGet *inf = (TReaderInfoUniqueGet*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoUniqueGet );

    inf->size_of = sizeof( TReaderInfoUniqueGet );
    if( inf->unique.text == NULL || inf->unique.length == 0 )
	return fat12_unique_form( ctx->volume_serial, ctx->volume_label,
	    &inf->unique.length, NULL );
    return fat12_unique_form( ctx->volume_serial, ctx->volume_label,
	&inf->unique.length, inf->unique.text );
}
/* end of file: $Id: f12uget.c 44148 2008-01-10 14:22:33Z dim $ */
