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
 * \version $Revision: 3583 $
 * \date $Date: 2001-08-06 20:07:46 +0400 (Mon, 06 Aug 2001) $
 * \author $Author: cav $
 * \brief ������������ ��������� �����.
 */

#include "ric_prj.h"

/*!
 * \brief ������������ ��������� �����.
 */
TSupErr ric_context_free( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoContextFree *inf = (TSupSysInfoContextFree*)info;
    TRICContext *ctx = (TRICContext*)context; 

    if( context == NULL )
	return SUP_ERR_NO;
    SUPSYS_PRE_INFO( info, TSupSysInfoContextFree );
    SUPSYS_PRE_CONTEXT( context, TRICContext );
    free( ctx );
    inf->size_of = sizeof( TSupSysInfoContextFree );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_free.c 3583 2001-08-06 16:07:46Z cav $ */
