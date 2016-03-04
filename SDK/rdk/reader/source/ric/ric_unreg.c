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

#include "ric_prj.h"

TSupErr ric_unregister( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoUnregister *inf = (TSupSysInfoUnregister*)info;
    SUPSYS_PRE_INFO( info, TSupSysInfoUnregister );

    if( inf->unload )
	support_unload_library();
    if( context == NULL )
	return SUP_ERR_NO;
    free( context );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_unreg.c 68807 2011-06-27 07:16:13Z dim $ */
