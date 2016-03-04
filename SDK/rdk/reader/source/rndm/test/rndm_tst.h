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

/*!
 * \file $RCSfile$
 * \version $Revision: 36803 $
 * \date $Date: 2006-10-25 16:55:21 +0400 (Wed, 25 Oct 2006) $
 * \author $Author: cav $
 *
 * \brief ������ include ���� ����� ���.
 *
 * ������: ��������� � �������� ��������� ����� (RNDM).
 *
 * ������ include ���� ��� ������� (RNDM/TEST).
 */

#if !defined( _RNDM_RNDM_PRJ_H )
#define _RNDM_RNDM_PRJ_H 

#include "reader.kit/gen_prj.h"

#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"
#include "reader.kit/sup_lst.h"

/* Project (RNDM) */
#include "reader/rndm.h" /* Project (RNDM) include.
    ��������� � ���. */

#include "reader.kit/sup_sys.h"
#include "reader.kit/test/sstst.h"

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_LOG CONS
#include "reader/dprint.h"

#endif /* !defined( _RNDM_RNDM_PRJ_H ) */
/* end of file:$Id: rndm_tst.h 36803 2006-10-25 12:55:21Z cav $ */
