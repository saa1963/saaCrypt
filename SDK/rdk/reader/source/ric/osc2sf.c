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
 * \brief Функция выбора файла.
 */

#include "ric_prj.h"
#include "limits.h" 

/* Реальные номера файлов на карте. */
static const unsigned char OSCAR2_FILE_COMMON[2] = { 3, 6 };

/*!
 * \brief Функция выбора файла.
 */
TSupErr oscar2_select_file( 
    const TRICContext *ctx,
    unsigned char file_id, 
    size_t *length )
{
    TSCardInfoAPDU apdu_sel = { 0, 0xA4, 00, 0x04, {0,0}, {0,0}, 0, 0 };
    TSCardInfoAPDU apdu_resp = { 0, 0xC0, 00, 0x00, {0,0}, {0,0}, 0, 0 };
    TSCardInfoBerTLVSearch search = { 0x3, 0x2, 0x80, {0,0}, {0,0} };
    unsigned char data[2];
    unsigned char simple[2];
    TSupErr code;
    unsigned char file_id_new = 0;

    search.class_tag = ctx->class_tag;
    *length = 0xffff;
    
    if( file_id > 6 || file_id == 0 )
	return SUP_ERR_PARAM;
    file_id_new = OSCAR2_FILE_COMMON[OSCAR2_FILE_INDEX[file_id - 1]];

    data[0] = (unsigned char)(( file_id_new + ctx->application ) >> 8 );
    data[1] = (unsigned char)(( file_id_new + ctx->application ) &0xff );
    apdu_sel.input.length = 2;
    apdu_sel.input.info = data;
    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_sel );
    if( code )
	return code;
    if( apdu_sel.sw1 == 0x6a && apdu_sel.sw2 == 0x82 )
	return RDR_ERR_FILE_NOT_FOUND;
    if( apdu_sel.sw1 != 0x61 || apdu_sel.sw2 == 0 )
	return RDR_ERR_INVALID_MEDIA;
    apdu_resp.output.info = malloc( apdu_sel.sw2 );
    if( apdu_resp.output.info == NULL )
	return SUP_ERR_MEMORY;
    apdu_resp.output.length = apdu_sel.sw2;
    apdu_resp.input.length = 0;
    apdu_resp.input.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_resp );
    if( code )
    {
	free( apdu_resp.output.info );
	return code;
    }
    if( apdu_resp.sw1 != 0x90 || apdu_resp.sw2 != 0x00 )
    {
	free( apdu_resp.output.info );
	return RDR_ERR_INVALID_MEDIA;
    }
    search.input.length = apdu_resp.output.length;
    search.input.info = apdu_resp.output.info;
    search.output.length = 2;
    search.output.info = simple;
    code = ctx->ber_tlv_search( &search );
    free( search.input.info );
    if( code )
	return RDR_ERR_INVALID_MEDIA;
    if( search.output.length > 2 || !search.output.length )
	return RDR_ERR_INVALID_MEDIA;
    *length = *search.output.info;
    if( search.output.length == 2 )
	*length = ( *length << 8 ) + search.output.info[1];

    code = oscar2_file_length(file_id, length, ctx->connect);
    if(code)
	return code;

    return SUP_ERR_NO;
}
/* end of file: $Id: osc2sf.c 38642 2007-01-23 13:48:56Z dim $ */
