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

TSupErr ric_file_read( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoRead *inf = (TReaderInfoRead*)info;
    TSupErr code;
    TRICContext *ctx = (TRICContext*)context;
    TSCardInfoAPDU apdu = { 0, 0xB0, 0, 0, {0,0}, {0,0}, 0, 0 };
    size_t length = 0;
    int full = 0;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoRead );
    SUPSYS_PRE( ctx->file );
    SUPSYS_PRE( inf->from <= 0x7fff );
    SUPSYS_PRE( inf->info.length <= 0xffff );

    length = ctx->block_len_read;
    if( length > inf->info.length )
	length = inf->info.length;
    if( inf->from >= ctx->file_length )
	return RDR_ERR_FULL;
    if( inf->from + length > ctx->file_length )
    {
	full = 1;
	length = ctx->file_length - inf->from;
    }
    apdu.p1 = (unsigned char)( inf->from >> 8 );
    apdu.p2 = (unsigned char)( inf->from & 0xff );
    apdu.input.length = 0;
    apdu.input.info = 0;
    apdu.output.length = length;
    apdu.output.info = inf->info.info;
    code = ctx->apdu( ctx->reader, &apdu );
    if( code )
	return code;
    if( apdu.sw1 != 0x90 || apdu.sw2 != 0 )
	return ric_error( apdu.sw1, apdu.sw2 );
    inf->info.length -= length;
    inf->size_of = sizeof( TReaderInfoRead );
    if( full )
	return RDR_ERR_FULL;
    if( inf->info.length )
	return RDR_ERR_BLOCK;
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_rd.c 54220 2009-05-02 02:33:10Z lse $ */
