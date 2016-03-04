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

#if !defined( _SUPPORT_SSTEST_SSTSTPRJ_H )
#define _SUPPORT_SSTEST_SSTSTPRJ_H

#define _SSTEST_DECL _STDEXP_DECL

#include "reader.kit/gen_prj.h"

#if defined( SUPPORT_RESOURCE_STD )
extern TSupResourceInstance SSTEST_RESOURCE;
#else
extern HINSTANCE SSTEST_RESOURCE;
#endif

#include "reader/support.h"
#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"
#include "reader.kit/sup_lst.h"
#include "reader.kit/test/sstst.h"

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_CALL NORM
#define DB_LOG CONS
#define DB_UNUSUAL EXCL
#include "reader/dprint.h"
extern TSupportDbContext *support_context_io;

#endif /* !defined( _SUPPORT_SSTEST_SSTSTPRJ_H ) */
/* end of file:$Id: sststprj.h 41056 2007-05-29 20:41:40Z dim $ */
