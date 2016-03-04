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
TSupErr ric_unique_num( 
    const TRICContext *ctx, 
    unsigned char unique_num[4] )
{
    TSCardInfoAPDU apdu = { 0, 0xCA, 01, 00, {0,0}, {0,0}, 0, 0 };
    TSupErr code;

    apdu.input.length = 0;
    apdu.input.info = NULL;
    apdu.output.length = 4;
    apdu.output.info = unique_num;

    code = ctx->apdu( ctx->reader, &apdu );
    if( code )
	return code;
    if( apdu.sw1 != 0x90 )
	return ric_error( apdu.sw1, apdu.sw2 );
    if( apdu.output.length != 4 )
	return RDR_ERR_INVALID_MEDIA;
    return SUP_ERR_NO;
}
/* end of file: $Id: ricuni.c 38642 2007-01-23 13:48:56Z dim $ */
