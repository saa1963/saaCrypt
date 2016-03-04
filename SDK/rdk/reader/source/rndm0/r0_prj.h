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
 * $Id: r0_prj.h 75332 2012-03-01 10:03:55Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ��������� � �������� ��������� ����� (RNDM).
 *
 * ������ ��������� �����: ���������� ������������������ (RNDM0).
 *
 * ������ include ���� ��� ������� (READER/RNDM/RNDM0).
 *
 * ������ ��� ���������� ������� (READER/RNDM/RNDM0).
 *
 * ������ ����������� �� define: 
 *
 * �����������: _WIN32
 ****/

#if !defined _READER_RNDM_RNDM0_R0_PRJ_H
#define _READER_RNDM_RNDM0_R0_PRJ_H 

#include "reader.kit/gen_prj.h"

#include "reader/rndm.h" /* Project (READER/RNDM) include.
    ��������� � ���. */
#include "reader.kit/rndm_sys.h" /* Project (READER/RNDM) include.
    ����������� ����������� ��� �������� ���. */

/* Project (READER/RNDM/RNDM0) */
#if defined( _USRDLL )
#define _RNDM0_DECL _STDEXP_DECL
#endif /* defined( _USRDLL ) */

#include "reader/r0.h" /* Project (READER/RNDM/RNDM0) include.
    ��������� � ��� 0. */
#if !defined _WIN32
#include "rndm0_rc.h"
#endif /* _WIN32 */

#if defined( _RESOURCE ) || !defined ( SUPPORT_RESOURCE_STD )
extern HINSTANCE RNDM0_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

#endif /* !_READER_RNDM_RNDM0_R0_PRJ_H */
