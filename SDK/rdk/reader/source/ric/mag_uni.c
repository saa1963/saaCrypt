/*
 * Copyright(C) 2008 ������ ���
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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief ������� ��������� ����������� ������ �����.
 */

#include "ric_prj.h"

/*!
 * \brief ������� ��������� ����������� ������ �����.
 * \param reader [in] �������� �����������
 * \param card [in] �������� �����
 * \param unique_num [out] ���������� ����� �����
 * \return ��� ��������
 * \retval SUP_ERR_NO ���������� �������
 * \retval RDR_ERR_INVALID_MEDIA ����� ������� ����������� ������
 * \retval "������ �����������" \ref scard_error
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

    // ���� ��� select file, �� ������� ���.
    if( ctx->file )
    {
	size_t length;
	trust_select_file( ctx, ctx->file, &length );
    }
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_uni.c 54220 2009-05-02 02:33:10Z lse $ */
