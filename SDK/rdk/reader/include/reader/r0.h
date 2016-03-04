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
 * \version $Revision: 41056 $
 * \date $Date: 2007-05-30 00:41:40 +0400 (Wed, 30 May 2007) $
 * \author $Author: dim $
 *
 * \brief ������� �������� ������.
 *
 * ������: ������ ��������� �����: 0 ������������������(RNDM0).
 *
 * ������� ��������� ���.
 *
 * �������: rndm0_rndm_get_handle
 */

#if !defined( _READER_RNDM_RNDM0_R0_EXP_H )
#define _READER_RNDM_RNDM0_R0_EXP_H 

#include "reader/std_decl.h"
#include "reader/rndm.h" 

#if !defined( _RNDM0_DECL )
#if defined( RNDM0_IMPORTS )
#define _RNDM0_DECL _STDIMPL_DECL
#else /* defined( RNDM0_IMPORTS ) */
#define _RNDM0_DECL
#endif /* !defined( RNDM0_IMPORTS ) */
#endif /* !defined( _RNDM0_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* ��������� ����� ����� � ���������� �����������. */
_RNDM0_DECL const TRndmHandle* rndm0_rndm_get_handle( void );

/* ��������� ����� ����� � ���������� �����������. */
_RNDM0_DECL const TRndmHandle* fapsi0_rndm_get_handle( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_RNDM_RNDM0_R0_EXP_H ) */
/* end of file:$Id: r0.h 41056 2007-05-29 20:41:40Z dim $ */
