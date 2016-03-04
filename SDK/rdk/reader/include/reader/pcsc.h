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
 * \brief ������ ������� PC/SC � ������� �����������.
 */

#if !defined( _READER_PCSC_PCSC_H )
#define _READER_PCSC_PCSC_H

#include "reader/std_decl.h"
#include "reader/rdr.h"

#if !defined( _PCSC_DECL )
#if defined( PCSC_IMPORTS )
#define _PCSC_DECL _STDIMPL_DECL
#else /* defined( PCSC_IMPORTS ) */
#define _PCSC_DECL
#endif /* !defined( PCSC_IMPORTS ) */
#endif /* !defined( _PCSC_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** �������. **** */

/* ������� ������������ ����� ����� � ���������� �����������. */
_PCSC_DECL const TSupSysEHandle* pcsc_reader_get_handle( void );

/* ������� ������������ ����� ����� � ���������� �����������. */
_PCSC_DECL const TSupSysEHandle* pcsc_reader_group_handle( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_PCSC_PCSC_H ) */
/* end of file:$Id: pcsc.h 41056 2007-05-29 20:41:40Z dim $ */
