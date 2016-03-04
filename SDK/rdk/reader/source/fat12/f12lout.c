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
 * Функция уничтожения контекста считывателя.
 * Функция должна корректно отрабатывать закрытие не открытого считывателя.
 ++++*/
TSupErr fat12_disconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TReaderInfoDisconnectCarrier *inf = (TReaderInfoDisconnectCarrier*)info;

    SUPSYS_PRE_INFO( info, TReaderInfoDisconnectCarrier );
    inf->size_of = sizeof( TReaderInfoDisconnectCarrier );

    if( context == NULL )
	return SUP_ERR_NO;
    if( ctx->volume_label != NULL )
    {
	free( ctx->volume_label );
	ctx->volume_label = NULL;
    }
    ctx->volume_serial = (unsigned long)-1;

    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12lout.c 11621 2003-09-04 11:26:15Z pooh $ +*/
