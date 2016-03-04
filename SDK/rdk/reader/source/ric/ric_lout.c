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

TSupErr ric_logout( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoLogout *inf = (TReaderInfoLogout*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSCardInfoAPDU apdu_sel = { 0, 0xA4, 0x08, 0x04, {0,0}, {0,0}, 0, 0 };
    unsigned char data[4] = { 0x3f, 0x00, 0x0b, 0x01 };
    TSupErr code;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoLogout );

    apdu_sel.output.info = NULL;
    apdu_sel.output.length = 0;
    apdu_sel.input.length = 4;
    apdu_sel.input.info = data;

    code = ctx->apdu( ctx->reader, &apdu_sel );
    if( code == RDR_ERR_NO_CARRIER )
	return SUP_ERR_NO;
    if( code )
	return code;
    if( apdu_sel.sw1 == 0x61 )
    {
	TSCardInfoAPDU apdu_resp = { 0, 0xC0, 0x00, 0x00, {0,0}, {0,0}, 0, 0 };
	/* Дочитать. */
	apdu_resp.input.info = NULL;
	apdu_resp.input.length = 0;
	apdu_resp.output.length = apdu_sel.sw2;
	apdu_resp.output.info = malloc( apdu_sel.sw2 );
	if( apdu_resp.output.info == NULL )
	    return SUP_ERR_MEMORY;
	ctx->apdu( ctx->reader, &apdu_resp );
	free( apdu_resp.output.info );
    }
    ctx->status = RIC_STATUS_START;
    inf->size_of = sizeof( TReaderInfoLogout );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_lout.c 38642 2007-01-23 13:48:56Z dim $ */
