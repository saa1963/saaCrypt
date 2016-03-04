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
 * \brief Функция выбора файла.
 */

#include "ric_prj.h"
#include "limits.h" 

/*!
 * \brief Функция выбора файла.
 */
TSupErr trust_select_file( 
    const TRICContext *ctx,
    unsigned char file_id, 
    size_t *length )
{
    TSCardInfoAPDU apdu_sel = { 0x00, 0xa4, 0x00, 0x04 };
    TSCardInfoAPDU apdu_resp = { 0, 0xC0, 00, 0x00 };
    unsigned char data[2],data_out[256];
    TSupErr code;
    unsigned short id;

    *length = 0xffff;

    if( file_id >= UCHAR_MAX )
	return RDR_ERR_FILE_NOT_FOUND;
    if (file_id<7)
	id = (unsigned short) ( file_id + ctx->application );
    else
	id = (unsigned short) ( file_id + 0x100 );
    if (id<0x100) id+=0x100;

    data[0]=(unsigned char)(id>>8);
    data[1]=(unsigned char)(id&0x00ff);
	
    apdu_sel.input.length = 2;
    apdu_sel.input.info = data;
    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_sel );
    if( code )
	return code;
    if( apdu_sel.sw1 == 0x6a && apdu_sel.sw2 == 0x82 )
	return RDR_ERR_FILE_NOT_FOUND;
    if( apdu_sel.sw1 != 0x61 )
	return RDR_ERR_INVALID_MEDIA;

    apdu_resp.output.info = data_out;
    apdu_resp.output.length = apdu_sel.sw2;
    apdu_resp.input.length = 0;
    apdu_resp.input.info = NULL;
    code = ctx->apdu( ctx->reader, &apdu_resp );
    if( code )
    {
	return code;
    }
    if( apdu_resp.sw1 != 0x90 || apdu_resp.sw2 != 0x00 )
    {
	return RDR_ERR_INVALID_MEDIA;
    }

    switch (data_out[3]) {
    case 1:
	*length=(size_t)data_out[4];
	break;
    case 2:
	*length=(size_t)((data_out[4]<<8)+data_out[5]);
	break;
    default:
	return RDR_ERR_INVALID_MEDIA;
    }

    return SUP_ERR_NO;
}

/*!
 * \brief Функция выбора приложения.
 */
TSupErr trust_select_application( 
    const TRICContext *ctx )
{
    TSCardInfoAPDU apdu_sel = { 0x00, 0xa4, 0x04, 0x0c };
    TSupErr code;

//	Sleep(100);
//if (ric_select_file(ctx,1,&length)==SUP_ERR_NO) return SUP_ERR_NO;

    apdu_sel.input.info = (unsigned char *) "CPCSP";
    apdu_sel.input.length = 5; // без терминирующего '\0'

    apdu_sel.output.length = 0;
    apdu_sel.output.info = NULL;

    code = ctx->apdu( ctx->reader, &apdu_sel );

    if( code )
	return code;
    if( apdu_sel.sw1 == 0x6a && apdu_sel.sw2 == 0x82 )
	return RDR_ERR_FILE_NOT_FOUND;
    if( ( apdu_sel.sw1 != 0x90 || apdu_sel.sw2 != 0x00 ) && apdu_sel.sw1 != 0x61 )
 	return RDR_ERR_INVALID_MEDIA;
//	return RDR_ERR_MEDIA_CHANGED;
    return SUP_ERR_NO;
}

/* end of file: $Id: mag_sf.c 54220 2009-05-02 02:33:10Z lse $ */
