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

TSupErr fat12_info_instance(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoInstance *inf = (TSupSysInfoInstance*)info;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoInstance );
    inf->instance = FAT12_RESOURCE;
    inf->size_of = sizeof( TSupSysInfoInstance );
    return SUP_ERR_NO;
}
/*+ end of file: $Id: f12iinst.c 4025 2001-10-03 10:40:40Z cav $ +*/
