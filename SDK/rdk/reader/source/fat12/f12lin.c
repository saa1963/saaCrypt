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
 * ������� ������������� �����������.
 ++++*/
TSupErr fat12_connect_carrier( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TReaderInfoConnectCarrier *inf = (TReaderInfoConnectCarrier*)info;
  TFat12Context *ctx = (TFat12Context*)context;
  
  SUPSYS_PRE_CONTEXT( context, TFat12Context );
  SUPSYS_PRE_CONTEXT( info, TReaderInfoConnectCarrier );

  inf->size_of = sizeof( TReaderInfoConnectCarrier );
  return fat12_volume_name( ctx, ctx->path, &ctx->volume_label, 
			    &ctx->volume_serial );
}
/* end of file: $Id: f12lin.c 44148 2008-01-10 14:22:33Z dim $ */
