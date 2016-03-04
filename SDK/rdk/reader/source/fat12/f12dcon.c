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

#include "fat12prj.h"

/*!
 * \ingroup fat12_interface
 * \brief ������ ����� � ����������.
 * \return \link fat12_error ��� ������\endlink
 */
TSupErr fat12_disconnect( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TSupSysInfoDisconnect *inf = (TSupSysInfoDisconnect*)info;

    SUPSYS_PRE_INFO( info, TSupSysInfoDisconnect );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );

    inf->size_of = sizeof( TSupSysInfoDisconnect );
    if( ctx->path == NULL )
	return SUP_ERR_NO;
    free( ctx->path ); ctx->path = NULL;

    return SUP_ERR_NO;
}
