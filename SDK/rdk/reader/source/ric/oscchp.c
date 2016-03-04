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

/*!
 * \file $RCSfile$
 * \version $Revision: 38642 $
 * \date $Date: 2007-01-23 17:48:56 +0400 (Tue, 23 Jan 2007) $
 * \author $Author: dim $
 * \brief Функция смены pin.
 */

#include "ric_prj.h"

/*!
 * \brief Функция смены pin.
 */
TSupErr oscar_change_pin(
    const TRICContext *ctx, 
    unsigned char pin[8] )
{
    TSupErr code;
    TSCardInfoAPDU apdu_sel = { 0, 0xA4, 0x00, 0x00, {0,0}, {0,0}, 0, 0 };
    TSCardInfoAPDU apdu_change = { 0, 0x42, 0x00, 0x00, {0,0}, {0,0}, 0, 0 };
    unsigned char data[2] = { 0, 1 };

    /* Select pin file. */
    apdu_sel.input.length = 2;
    apdu_sel.input.info = data;
    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_sel );
    if( code )
	return code;
    if( ( apdu_sel.sw1 != 0x90 || apdu_sel.sw2 != 0x00 )
	&& apdu_sel.sw1 != 0x61 )
	return RDR_ERR_INVALID_MEDIA;
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
    /* change pin */
    apdu_change.input.length = 8;
    apdu_change.input.info = pin;
    apdu_change.output.length = 0;
    apdu_change.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_change );
    if( code )
	return code;
    if( apdu_change.sw1 != 0x90 || apdu_change.sw2 != 0 )
	return ric_error( apdu_change.sw1, apdu_change.sw2 );
#if 0
    /* Если был select file, то вернуть его. */
    if( ctx->file )
    {
	size_t length;
	ric_select_file( ctx, ctx->file, &length );
    }
#endif /* 0 */
    return SUP_ERR_NO;
}
/* end of file:$Id: oscchp.c 38642 2007-01-23 13:48:56Z dim $ */
