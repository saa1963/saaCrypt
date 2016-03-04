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

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). +*/

/*++++
 * ������� ��������� ����������� ������.
 ++++*/
TSupErr fat12_unique_get(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoUniqueGet *inf = (TReaderInfoUniqueGet*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoUniqueGet );

    inf->size_of = sizeof( TReaderInfoUniqueGet );
    if( inf->unique.text == NULL || inf->unique.length == 0 )
	return fat12_unique_form( ctx->volume_serial, ctx->volume_label,
	    &inf->unique.length, NULL );
    return fat12_unique_form( ctx->volume_serial, ctx->volume_label,
	&inf->unique.length, inf->unique.text );
}
/* end of file: $Id: f12uget.c 44148 2008-01-10 14:22:33Z dim $ */
