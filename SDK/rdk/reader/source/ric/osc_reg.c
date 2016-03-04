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

#include "ric_prj.h"

TSupErr oscar_register( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TRICContext *ctx;
    TSupSysInfoRegister *inf = (TSupSysInfoRegister*)info;
    SUPSYS_PRE_INFO( info, TSupSysInfoRegister );

    UNUSED( context );
    ctx = malloc( sizeof( TRICContext ) );
    if( ctx == NULL )
	return SUP_ERR_MEMORY;
    ctx->apdu = NULL;
    ctx->reader = NULL;
    ctx->status = RIC_STATUS_START;
    ctx->file = 0;
    ctx->file_length = 0;
    ctx->connect = 0;
    ctx->unique_length = OSCAR_UNIQUE_LENGTH;
    ctx->unique_form = oscar_unique_form;
    ctx->change_pin = oscar_change_pin;
    ctx->block_len_read = OSCAR_BLOCK_READ;
    ctx->block_len_write = OSCAR_BLOCK_WRITE;
    ctx->class_tag = 0xf;
    ctx->application = 0;
    memset( ctx->pin, 0, 8 );
    memset( ctx->unique, 0, 4 );
    inf->context = ctx;
    support_load_library();
    return SUP_ERR_NO;
}
/* end of file: $Id: osc_reg.c 68807 2011-06-27 07:16:13Z dim $ */
