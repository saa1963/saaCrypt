/*
 * Copyright(C) 2007 ������ ���
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
 * \version $Revision: 42873 $
 * \date $Date: 2007-10-16 17:05:56 +0400 (Tue, 16 Oct 2007) $
 * \author $Author: raa $
 * \brief ������� ��������� �������� �����������.
 */

#include "fat12prj.h"

TSupErr fat12_info_nickname( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoNickname *inf = (TSupSysInfoNickname*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_INFO( info, TSupSysInfoNickname );
    SUPSYS_PRE_CCONTEXT( context, TFat12Context );

    _tcscpy ( inf->nickname, ctx->nickname );
    inf->size_of = sizeof( TSupSysInfoNickname );
    return SUP_ERR_NO;
}
