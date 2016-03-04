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

#if !defined( _READER_HDIMAGE_HDIMGINF_H )
#define _READER_HDIMAGE_HDIMGINF_H

#include "reader/rdr.h" /* Project (READER) include.
    ������� ��������� ������ �� �������������. */
#include "reader.kit/reader.h" /* Project (READER/RDR) include.
    ����������� ����������� ��� �������� ���������� �����������. */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* ������� �������. */
/* ������� ��������� ��������� ������. */
TSupErr hdimage_info_system_flag(
    TSupSysContext *context, TSupSysInfo* );

/* ������� ��������������� �����������. */
TSupErr hdimage_register(
    TSupSysContext *context, TSupSysInfo *info );

/* ������� ��������� ����� local_machine. */
TSupErr hdimage_local_machine(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr hdimage_info_ids(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ehdimage_info_system_flag(
    TSupSysContext *context, TSupSysInfo* );

TSupErr ehdimage_info_ids(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ehdimage_setup_encryption(
    TSupSysContext *context, TSupSysInfo *info );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_HDIMAGE_HDINF_H ) */
/* end of file:$Id: hdimginf.h 42536 2007-09-26 13:50:44Z chudov $ */
