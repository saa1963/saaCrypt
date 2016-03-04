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

#define GET_X_DIGIT(x) ((x)>='0'&&(x)<='9'?(x)-'0':((x)>='a'&&(x)<='f'?\
    (x)-'a'+10:(x)-'A'+10))

TSupErr oscar2_folder_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupErr code;
    TReaderInfoFolderOpen *inf = (TReaderInfoFolderOpen*)info;
    TRICContext *ctx = (TRICContext*)context;
    size_t length;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderOpen );

    if( inf->name.length )
    {
	if( inf->name.length != 4
	    || !_istxdigit( inf->name.text[0] )
	    || !_istxdigit( inf->name.text[1] )
	    || !_istxdigit( inf->name.text[2] )
	    || !_istxdigit( inf->name.text[3] ) )
	{
	    return SUP_ERR_PARAM;
	}
	ctx->application = (unsigned short)( GET_X_DIGIT( inf->name.text[0] ) << 12
	    | GET_X_DIGIT( inf->name.text[1] ) << 8
	    | GET_X_DIGIT( inf->name.text[2] ) << 4
	    | GET_X_DIGIT( inf->name.text[3] ) );
    }
    else
	ctx->application = RIC_APPLICATION;
    code = oscar2_select_file( ctx, 1, &length );
    if( code )
	return code;
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2_fop.c 12914 2003-11-25 08:08:36Z dim $ */
