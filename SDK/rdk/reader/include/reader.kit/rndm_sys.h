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
 * \version $Revision: 66366 $
 * \date $Date: 2011-02-15 19:51:24 +0400 (Tue, 15 Feb 2011) $
 * \author $Author: cav $
 * \brief ��������� � ���������� ���.
 */

#if !defined( _RNDM_RNDM_SYS_H )
#define _RNDM_RNDM_SYS_H

#include "reader.kit/sup_sys.h"
#include "reader/rndm.h" 

#define RANDOM_FUN_TYPE 0x100
#define RANDOM_FUN_RNDM 0x2100
#define RANDOM_FUN_HWND 0x2101

typedef struct TRandomInfoType_
{
    size_t size_of;
    TRndmType type;
} TRandomInfoType;

typedef struct TRandomInfoRndm_
{
    size_t size_of;
    TSupSysInfoUntyped info;
} TRandomInfoRndm;

#if defined( _WIN32 )
typedef struct TRandomInfoParentHwnd_
{
    size_t size_of;
    HWND hwnd;
} TRandomInfoParentHwnd;
#endif

#endif /* !defined( _RNDM_RNDM_SYS_H ) */
