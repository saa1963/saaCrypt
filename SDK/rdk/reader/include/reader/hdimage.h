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

#if !defined( _READER_HDIMAGE_HDIMGEXP_H )
#define _READER_HDIMAGE_HDIMGEXP_H

#include "reader/std_decl.h"
#include "reader/rdr.h"

#if !defined( _HDIMAGE_DECL )
#if defined( HDIMAGE_IMPORTS )
#define _HDIMAGE_DECL _STDIMPL_DECL
#else /* defined( HDIMAGE_IMPORTS ) */
#define _HDIMAGE_DECL
#endif /* !defined( HDIMAGE_IMPORTS ) */
#endif /* !defined( _HDIMAGE_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** �������. **** */

_HDIMAGE_DECL const TSupSysEHandle* hdimage_reader_get_handle(
    void );

_HDIMAGE_DECL const TSupSysEHandle* hsm_reader_get_handle(
    void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_HDIMAGE_HDIMGEXP_H ) */
/* end of file:$Id: hdimage.h 42536 2007-09-26 13:50:44Z chudov $ */
