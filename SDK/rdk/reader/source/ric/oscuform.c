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

static const TCHAR UNIQUE_PREFIX[] = _TEXT( "OSCAR_%02X%02X%02X%02X" );

/* ������� ��������� ����������� ������. */
TSupErr oscar_unique_form( unsigned char unique_num[4], TCHAR *unique )
{
    _sntprintf( unique, MAX_UNIQUE_LENGTH + 1, UNIQUE_PREFIX, unique_num[0], unique_num[1], 
	unique_num[2], unique_num[3] );
    return SUP_ERR_NO;
}
/* end of file: $Id: oscuform.c 66111 2011-02-03 15:03:44Z maxdm $ */
