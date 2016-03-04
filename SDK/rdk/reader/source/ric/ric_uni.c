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

/* ������� ��������� ����������� ������. */
TSupErr ric_unique_get(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoUniqueGet *inf = (TReaderInfoUniqueGet*)info;
    TRICContext *ctx = (TRICContext*)context;
    TCHAR *buffer;
    TCHAR full[MAX_UNIQUE_LENGTH + 1];
    TSupErr code;
    unsigned char unique_num[4];

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoUniqueGet );

    inf->size_of = sizeof( TReaderInfoUniqueGet );
    if( inf->unique.text == NULL || inf->unique.length == 0 )
    {
	inf->unique.length = ctx->unique_length;
	return SUP_ERR_NO;
    }
    buffer = inf->unique.text;
    code = ric_unique_num( ctx, unique_num );
    if( code )
	return code;
    ctx->unique_form( unique_num, full );
    _tcsncpy( buffer, full, inf->unique.length );
    buffer[inf->unique.length] = 0;
    inf->unique.length = ctx->unique_length;
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_uni.c 41567 2007-07-11 14:28:24Z dim $ */
