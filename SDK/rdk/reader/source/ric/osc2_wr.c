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

TSupErr oscar2_file_write( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoWrite *inf = (TReaderInfoWrite*)info;
    TSupErr code;
    TRICContext *ctx = (TRICContext*)context;
    TSCardInfoAPDU apdu = { 0, 0xD6, 0, 0, {0,0}, {0,0}, 0, 0 };
    size_t length;
    int full = 0;
    size_t from_new;
    size_t offset = 0;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoWrite );
    SUPSYS_PRE( ctx->file );
    SUPSYS_PRE( inf->from <= 0xffff );
    SUPSYS_PRE( inf->info.length <= 0xffff );

    length = ctx->block_len_write;
    if( length > inf->info.length )
	length = inf->info.length;
    if( inf->from >= ctx->file_length )
	return RDR_ERR_FULL;
    if( inf->from + length > ctx->file_length )
    {
	full = 1;
	length = ctx->file_length - inf->from;
    }

    code = oscar2_file_offset(ctx->file, &offset, ctx->connect);
    if(code)
	return code;
    from_new = inf->from + offset;

    if(length > (ctx->block_len_write - (from_new % ctx->block_len_write))) 
	length = ctx->block_len_write - (from_new % ctx->block_len_write);

    apdu.p1 = (unsigned char)( from_new >> 8 );
    apdu.p2 = (unsigned char)( from_new & 0xff );
    apdu.input.length = length;
    apdu.input.info = inf->info.info;
    apdu.output.length = 0;
    apdu.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu );
    if( code )
	return code;

    // Если данным блоком писать не может (места нет), то ошибка
    if(apdu.sw1 == 0x62 && apdu.sw2 == 0xf3)
	return RDR_ERR_FULL;

    if( apdu.sw1 != 0x90 || apdu.sw2 != 0 )
    {
	if( apdu.sw1 == 0x6b && apdu.sw2 == 0 )
	    return RDR_ERR_FULL;
	return ric_error( apdu.sw1, apdu.sw2 );
    }
    inf->info.length -= length;
    inf->size_of = sizeof( TReaderInfoWrite );
    if( full )
	return RDR_ERR_FULL;
    if( inf->info.length )
	return RDR_ERR_BLOCK;
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2_wr.c 38642 2007-01-23 13:48:56Z dim $ */
