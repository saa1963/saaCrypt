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

TSupErr ric_file_close( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoClose *inf = (TReaderInfoClose*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoClose );

    ctx->status &= ~RIC_STATUS_FILE_SELECTED;
    ctx->file = 0;
    ctx->file_length = 0;
    inf->size_of = sizeof( TReaderInfoClose );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_clos.c 3583 2001-08-06 16:07:46Z cav $ */
