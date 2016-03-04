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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief Функция смены pin.
 */

#include "ric_prj.h"

/*!
 * \brief Функция смены pin.
 */
TSupErr trust_change_pin(
    const TRICContext *ctx, 
    unsigned char pin[8] )
{
    TSupErr code;
    TSCardInfoAPDU apdu_sel = { 0x00, 0xa4, 0x00, 0x0c };
    TSCardInfoAPDU apdu_change = { 0x00, 0x24, 0x01, 0x00 };
    unsigned char data[2] = { 0, 1 };

    /* Select pin file. */
    apdu_sel.input.length = 2;
    apdu_sel.input.info = data;
    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_sel );
    if( code )
	return code;
    if( apdu_sel.sw1 != 0x90 || apdu_sel.sw2 != 0x00 )
	return RDR_ERR_INVALID_MEDIA;
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
    /* Если был select file, то вернуть его. */
    if( ctx->file )
    {
	size_t length;
	ric_select_file( ctx, ctx->file, &length );
    }
    return SUP_ERR_NO;
}
/* end of file:$Id: mag_chp.c 54220 2009-05-02 02:33:10Z lse $ */
