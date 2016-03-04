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
 * ������� ����������� ��������� �����������.
 * ������� ������ ��������� ������������ �������� �� ��������� �����������.
 ++++*/
TSupErr fat12_disconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TReaderInfoDisconnectCarrier *inf = (TReaderInfoDisconnectCarrier*)info;

    SUPSYS_PRE_INFO( info, TReaderInfoDisconnectCarrier );
    inf->size_of = sizeof( TReaderInfoDisconnectCarrier );

    if( context == NULL )
	return SUP_ERR_NO;
    if( ctx->volume_label != NULL )
    {
	free( ctx->volume_label );
	ctx->volume_label = NULL;
    }
    ctx->volume_serial = (unsigned long)-1;

    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12lout.c 11621 2003-09-04 11:26:15Z pooh $ +*/
