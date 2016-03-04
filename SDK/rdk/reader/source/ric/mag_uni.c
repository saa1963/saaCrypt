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
 * \brief Функция получения уникального номера карты.
 */

#include "ric_prj.h"

/*!
 * \brief Функция получения уникального номера карты.
 * \param reader [in] контекст считывателя
 * \param card [in] контекст карты
 * \param unique_num [out] уникальный номер карты
 * \return код возврата
 * \retval SUP_ERR_NO приложение выбрано
 * \retval RDR_ERR_INVALID_MEDIA карта вернула неожиданные данные
 * \retval "ошибки считывателя" \ref scard_error
 */
TSupErr trust_unique_num( 
    const TRICContext *ctx, 
    unsigned char unique_num[4] )
{
    TSupErr code;
    TSCardInfoAPDU apdu_sel = { 0x00, 0xa4, 0x00, 0x0c };
    TSCardInfoAPDU apdu_read = { 0x00, 0xb0, 0x00, 0x00 };
    unsigned char data[4];

    code = trust_select_application(ctx);
    if( code )
	return code;

    // Select s/n file.
    apdu_sel.input.length = 2;
    apdu_sel.input.info = (unsigned char *) "\x01\x00";
    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_sel );

    if( code )
	return code;
    if( apdu_sel.sw1 != 0x90 || apdu_sel.sw2 != 0x00 )
	return RDR_ERR_INVALID_MEDIA;

    apdu_read.input.length = 0;
    apdu_read.input.info = NULL;
    apdu_read.output.length = 4;
    apdu_read.output.info = data;
    code = ctx->apdu( ctx->reader, &apdu_read );

    if( code )
	return code;
    if( apdu_read.sw1 != 0x90 || apdu_read.sw2 != 0 )
	return RDR_ERR_INVALID_MEDIA;
    memcpy(unique_num,data,4);

    // Если был select file, то вернуть его.
    if( ctx->file )
    {
	size_t length;
	trust_select_file( ctx, ctx->file, &length );
    }
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_uni.c 54220 2009-05-02 02:33:10Z lse $ */
