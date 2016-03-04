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
