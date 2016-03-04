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

TSupErr ric_passwd_change( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoPasswdChange *inf = (TReaderInfoPasswdChange*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;
    TCHAR *passwd;
    char new_pin[8];

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoPasswdChange );

    passwd = inf->new_passwd.text;
    memset( new_pin, 0x31, 8 );
    SUPSYS_PRE( inf->new_passwd.length <= 8 );
    if( passwd )
	_2asciincpy( new_pin, passwd, (int)_tcslen( passwd ) );
    code = ctx->change_pin( ctx, (unsigned char*)new_pin );
    if( code ) 
    {
	memset( new_pin, 0x31, 8 );
	return code;
    }
    memcpy( ctx->pin, new_pin, 8 );
    ctx->status = RIC_STATUS_PIN_VERIFIED;
    inf->size_of = sizeof( TReaderInfoPasswdChange );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_pch.c 41567 2007-07-11 14:28:24Z dim $ */
