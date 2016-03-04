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
 * \version $Revision: 70756 $
 * \date $Date: 2011-09-09 16:43:32 +0400 (Fri, 09 Sep 2011) $
 * \author $Author: maxdm $
 * \brief ������� ������������ � ���������.
 */

#include "ric_prj.h"

/*! 
 * \ingroup ric_fun
 * \brief ������� ������������ � ���������.
 */
TSupErr ric_disconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TRICContext *ctx = (TRICContext*)context;
    UNUSED(info);

    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );

    free(ctx->connect); ctx->connect = NULL;

    return SUP_ERR_NO;
}
/* end of file: $Id: ric_disc.c 70756 2011-09-09 12:43:32Z maxdm $ */
