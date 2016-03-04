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
 * \version $Revision: 59642 $
 * \date $Date: 2010-01-21 13:31:01 +0400 (Thu, 21 Jan 2010) $
 * \author $Author: maxdm $
 * \brief Функция проверки pin-кода.
 */

#include "ric_prj.h"

/*!
 * \brief Функция проверки pin-кода.
 */
TSupErr ric_verify_pin(
    const TRICContext *ctx, 
    unsigned char pin[8],
    int *retries )
{
    TSupErr code;
    TSCardInfoAPDU apdu_sel = { 0, 0xA4, 0x00, 0x00, {0,0}, {0,0}, 0, 0 };
    TSCardInfoAPDU apdu_verify = { 0, 0x20, 0x00, 0x00, {0,0}, {0,0}, 0, 0 };
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
    /* verify pin */
    apdu_verify.input.length = 8;
    apdu_verify.input.info = pin;
    apdu_verify.output.length = 0;
    apdu_verify.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_verify );
    if( code )
	return code;
    if( apdu_verify.sw1 == 0x63 && ( apdu_verify.sw2 & 0xf0 ) == 0xc0 )
    {
	if( apdu_verify.sw2 & 0xf )
	{
	    if( retries ) *retries = apdu_verify.sw2 & 0xf;
	    return RDR_ERR_INVALID_PASSWD;
	}
	else
	{
	    if( retries ) *retries = 0;
	    return RDR_ERR_PASSWD_LOCKED;
	}
    }
    if( apdu_verify.sw1 == 0x69 && apdu_verify.sw2 == 0x83 )
	return RDR_ERR_PASSWD_LOCKED;
    if( apdu_verify.sw1 != 0x90 || apdu_verify.sw2 != 0 )
	return RDR_ERR_INVALID_MEDIA;
    return SUP_ERR_NO;
}
/* end of file:$Id: ric_vpin.c 59642 2010-01-21 09:31:01Z maxdm $ */
