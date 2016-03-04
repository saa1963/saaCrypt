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

TSupErr oscar2_restore_context( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoRestoreContext *inf = (TReaderInfoRestoreContext*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoRestoreContext );

    if( ctx->status & RIC_STATUS_PIN_VERIFIED )
    {
	code = ric_verify_pin( ctx, ctx->pin, NULL );
	if( code )
	    return code;
    }
    if( ctx->status & RIC_STATUS_FILE_SELECTED )
    {
	code = oscar2_select_file( ctx, ctx->file, &( ctx->file_length ) );
	if( code )
	    return code;
    }
    inf->size_of = sizeof( TReaderInfoRestoreContext );
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2_rest.c 10431 2003-06-09 13:15:36Z fduch $ */
