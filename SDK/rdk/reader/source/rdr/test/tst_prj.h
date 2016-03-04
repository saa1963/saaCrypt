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
 * $Id: tst_prj.h 75490 2012-03-04 13:04:37Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 * ��������� �� ���� ������������ (RDR).
 * ���������� ������������ (TEST).
 * ������ include ���� ��� ������� (READER/RDR/TEST).
 *
 * ������ ����������� �� define:
 *
 * �����������: Win32
 ****/

#if !defined( _READER_RDR_TEST_TST_PRJ_H )
#define _READER_RDR_TEST_TST_PRJ_H /*+ ���������� �������� ���������. +*/

#define _READER_TEST_DECL _STDEXP_DECL

#include "reader.kit/gen_prj.h"

/*+ Project (READER). +*/
#include "reader/rdr.h" /*+ Project (READER) include.
    ������� ��������� ������ �� �������������. +*/

/*+ Project (READER/sup_sys). +*/
#include "reader.kit/sup_sys.h"

/*+ Project (READER/RDR/TEST). +*/
#include "reader.kit/test/tst_rdr.h" /*+ Project (READER/RDR/TEST) include.
    ������� ������������ �����������. +*/

/*+ Project (READER/RDR/TEST) ���������� include �����. +*/
/*+ Project (READER/RDR/TEST) include.
    ��������� ���������. +*/
#include "reader/sup_syse.h"
#include "reader.kit/test/sstst.h"

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_CALL NORM
#define DB_LOG CONS
#define DB_UNUSUAL EXCL
#include "reader/dprint.h"

extern TSupportDbContext *support_context_io;

#endif /* !defined( _READER_RDR_TEST_TST_PRJ_H ) */
/*+ end of file:tst_prj.h +*/
