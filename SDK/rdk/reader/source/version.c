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

/****
 * $Id: version.c 61198 2010-04-10 08:41:58Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ������� ��������� ���������� � �������� �������.
 * ��������� ������ ������.
 *
 * ���������: FAT12_VERSION
 *
 * �������: fat12_info_version
 *
 * ������ ����������� �� define: 
 *
 * �����������: 
 *
 * ������������ �������������: SUPPORT_ASSERT
 *
 ****/
/*+ Project (READER/SUPPORT) +*/
#include "reader.kit/gen_prj.h"
#ifndef UNIX
#include <windows.h>
#endif
#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"
#include "reader.kit/rdr_ver.h"


/* ------------------------------------------ */
/*+ ������� ����� ������. +*/
static const TSupSysInfoVersion _VERSION = 
{
 sizeof(TSupSysInfoVersion),{3,0,0,0}
};


/*++++
 * ��������� ������ ������.
 ++++*/
TSupErr info_versionsupport(
    TSupSysContext *context, /*+ (io) ������ ������������ ���������� �� ������ ������ 
	������������ ���������� (� ���� �� �� ����������, �� ������ ����������� 
	����������). +*/
    TSupSysInfo *info ) /*+ (io) ������������� ����������. +*/
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� ������� ������� ����� ������
 * SUP_ERR_PARAM, ��� ����������� ���������� ����������.
 +*/
{
    UNUSED( context );
    if(info == NULL) return SUP_ERR_PARAM;
    if((*(TSupSysInfoVersion*)info).size_of < sizeof( TSupSysInfoVersion ) ) return SUP_ERR_PARAM;

    *(TSupSysInfoVersion*)info = _VERSION;
    return SUP_ERR_NO;
}
/*+ end of file: $Id: version.c 61198 2010-04-10 08:41:58Z dim $ +*/
