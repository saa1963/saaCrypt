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

TSupErr trust_restore_context( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
#if 0 // TODO: �����������
    TReaderInfoRestoreContext *inf = (TReaderInfoRestoreContext*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;
#endif

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoRestoreContext );

#if 0 // TODO: �����������
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
#endif
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_rest.c 54220 2009-05-02 02:33:10Z lse $ */
