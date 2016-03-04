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

TSupErr ric_login( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoLogin *inf = (TReaderInfoLogin*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;
    TCHAR *passwd;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoLogin );

    passwd = inf->passwd.text;
    memset( ctx->pin, 0x31, 8 );
    SUPSYS_PRE( inf->passwd.length <= 8 );
    if( passwd )
	_2asciincpy( (char*)ctx->pin, passwd, (int)_tcslen( passwd ) );
    code = ric_verify_pin( ctx, ctx->pin, &( inf->retries ) );
    if( code ) 
    {
	memset( ctx->pin, 0x31, 8 );
	return code;
    }
    ctx->status = RIC_STATUS_PIN_VERIFIED;
    inf->size_of = sizeof( TReaderInfoLogin );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_log.c 41567 2007-07-11 14:28:24Z dim $ */
