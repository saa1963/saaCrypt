/*
 * Copyright(C) 2008 Проект ИОК
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

/*!
 * \file $RCSfile$
 * \version $Revision: 68807 $
 * \date $Date: 2011-06-27 11:16:13 +0400 (Mon, 27 Jun 2011) $
 * \author $Author: dim $
 * \brief Регистрация Магистра (ОС Trust)
 */

#include "ric_prj.h"

TSupErr trust_register( 
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
    ctx->unique_length = TRUST_UNIQUE_LENGTH;
    ctx->unique_form = trust_unique_form;
    ctx->change_pin = trust_change_pin;
    ctx->block_len_read = TRUST_BLOCK_READ;
    ctx->block_len_write = TRUST_BLOCK_WRITE;
    ctx->class_tag = 0xf;
    ctx->application = 0x100;
    memset( ctx->pin, 0, 8 );
    memset( ctx->unique, 0, 4 );
    inf->context = ctx;
    /*TODO: ???? Програм-Парк отключал support_load_library() ????*/
    support_load_library();
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_reg.c 68807 2011-06-27 07:16:13Z dim $ */
