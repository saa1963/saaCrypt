/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

/*!
 * \file $RCSfile$
 * \version $Revision: 38642 $
 * \date $Date: 2007-01-23 17:48:56 +0400 (Tue, 23 Jan 2007) $
 * \author $Author: dim $
 * \brief ������� ������ �����.
 */

#include "ric_prj.h"
#include "limits.h" 

/*!
 * \brief ������� ������ �����.
 */
TSupErr ric_select_file( 
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

    search.class_tag = ctx->class_tag;
    *length = 0xffff;
    data[0] = (unsigned char)(( file_id + ctx->application ) >> 8 );
    data[1] = (unsigned char)(( file_id + ctx->application ) &0xff );
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
    return SUP_ERR_NO;
}
/* end of file: $Id: ricsf.c 38642 2007-01-23 13:48:56Z dim $ */
