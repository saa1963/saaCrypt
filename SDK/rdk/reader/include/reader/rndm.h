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
 * \version $Revision: 78493 $
 * \date $Date: 2012-06-07 14:09:43 +0400 (Thu, 07 Jun 2012) $
 * \author $Author: dim $
 * \brief ��������� � ���.
 */

#if !defined( _RNDM_RNDM_H )
#define _RNDM_RNDM_H

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif /* HAVE_LIMITS_H */
#include "reader/sup_syse.h"
#include "reader/std_decl.h"

/* **** **** define **** **** */
#if !defined( _RNDM_DECL )
#if defined( RNDM_IMPORTS )
#define _RNDM_DECL _STDIMPL_DECL
#else /* defined( RNDM_IMPORTS ) */
#define _RNDM_DECL
#endif /* !defined( RNDM_IMPORTS ) */
#endif /* !defined( _RNDM_DECL ) */

#define rndm_version supsys_version
#define rndm_nickname_a supsys_nickname_a
#define rndm_nickname_w supsys_nickname_w
#define rndm_nickname supsys_nickname
#define rndm_name_a supsys_name_a
#define rndm_name_w supsys_name_w
#define rndm_name supsys_name
#define rndm_detail_a supsys_detail_a
#define rndm_detail_w supsys_detail_w
#define rndm_detail supsys_detail
#define rndm_producer_a supsys_producer_a
#define rndm_producer_w supsys_producer_w
#define rndm_producer supsys_producer
#define rndm_producer_version_a supsys_producer_version_a
#define rndm_producer_version_w supsys_producer_version_w
#define rndm_producer_version supsys_producer_version
#define rndm_company_a supsys_company_a
#define rndm_company_w supsys_company_w
#define rndm_company supsys_company
#define rndm_icon supsys_icon
#define rndm_find_next supsys_find_next
#define rndm_find_close supsys_find_close
#define rndm_context_free supsys_context_free
#define rndm_context_dup supsys_context_dup
//#define rndm_flags supsys_flags
#define rndm_state supsys_state
#define TRndmContext TSupSysEContext
#define TRndmNickname TSupSysENickname
#define TRndmList TSupSysEList
#define TRndmFind TSupSysEFind
#define TRndmHandle TSupSysEHandle
#define TRndmRegisterFlags TSupSysERegisterFlags
#define RNDM_NICKNAME_LENGTH SUPSYSE_NICKNAME_LENGTH
#define RNDM_REGISTER_FLAG_FAILED SUPSYSE_REGISTER_FLAG_FAILED
#define RNDM_REGISTER_FLAG_DISABLED SUPSYSE_REGISTER_FLAG_DISABLED
#define RNDM_REGISTER_FLAG_INSTALL_ADD SUPSYSE_REGISTER_FLAG_INSTALL_ADD

/*! \ingroup rndm_error
 * \brief ������. ��� �� ������. 
 *
 * ��������� ���������� �������: ��� �� ������. 
 */
#define RNDM_ERR_NOT_FOUND 0x4000

/*+ ������ ������ 1.0 �� ��������������. +*/
#define RNDM_ERR_VERSION_1 0x4001

/*+ ������ ��� ������ �� �������������� ��� ������ ��������� �� �����. +*/
#define RNDM_ERR_INVALID 0x4E00

/*+ ����������� ������������� ������ � ������� ������� ���������.
    �������� ����������� ������. +*/
#define RNDM_ERR_IS( err ) \
    ( ( (err) >= RNDM_ERR_NOT_FOUND ) && ( (err) <= RNDM_ERR_VERSION_1 ) )

/*! \ingroup rndm_type 
 * \brief ��� �� �������� ����.
 */
#define RNDM_TYPE_WINDOWLESS 0x1

/*! \ingroup rndm_type 
 * \brief ����������� ���.
 */
#define RNDM_TYPE_REMOVABLE 0x2

/*! \ingroup rndm_type 
 * \brief ����������������� ����� ���
 */
#define RNDM_TYPE_CERTIFIED 0x4

/*! \ingroup rndm_type
 * \brief ���������� ���.
 */
#define RNDM_TYPE_HARD 0x8

/*! \ingroup rndm_type
 * \brief ����� �����
 */
#define RNDM_TYPE_FAPSI_GAMMA 0x10

/*! \ingroup rndm_type 
 * \brief ����� ������ ������ ���. 
 */
#define RNDM_TYPE_ANY_MASK 0 

/*! \ingroup rndm_type 
 * \brief ����� ������ ��� ���������������� ������ �������� ������. 
 */
#define RNDM_TYPE_EXCLUSIVE_MASK ULONG_MAX

/*! \ingroup rndm_type 
 * \brief ��������� ��������� ����� ������ ���
 */
#define RNDM_TYPE_ANY_VALUE 0

/* **** **** typedef **** **** */

/*! \ingroup rndm_type 
 * \brief ����� ����� ���. 
 */
typedef unsigned long TRndmType;

/* ������� ����������� ���. */
typedef unsigned TRndmLevel;

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* ������� ��������� ���������� �����. */
_RNDM_DECL TSupErr rndm( TRndmContext *context, size_t length, 
    void *info ); 

/* ������� ��������� ���� ���. */
_RNDM_DECL TSupErr rndm_type_get( const TRndmContext *context, 
    TRndmType *type ); 

#if !defined UNIX
/* ����� ������. */
_RNDM_DECL TSupErr rndm_error( const TRndmContext *context,
    TSupErr code, size_t *length, TCHAR *text );
#endif /* !UNIX */

/* ������� �������� ������������ ���. */
_RNDM_DECL TSupErr rndm_find_open( 
    TRndmList *list, TRndmFind **context, TRndmType mask,
    TRndmType value );

/* ������� ��������� ��� �� �����. */
_RNDM_DECL TSupErr rndm_find_mask( TRndmList *list, 
    TRndmContext **context, TRndmType mask, TRndmType value );

/* ������� ��������� ������ ���. */
_RNDM_DECL TSupErr rndm_level_set( const TRndmContext *ctx,
    TRndmLevel level );

/* ������� ��������� ������ ���. */
_RNDM_DECL TSupErr rndm_level_get( const TRndmContext *ctx,
    TRndmLevel *level );

#if defined( _WIN32 )
_RNDM_DECL TSupErr rndm_set_hwnd( const TRndmContext *ctx,
    HWND hwnd );
#endif

/* ������� ����������� ���. */
_RNDM_DECL TSupErr rndm_register_start( TRndmList **list );

#ifndef NO_SUP_PROPERTIES
_RNDM_DECL TSupErr rndm_wnd_configure( HWND hwnd ); 
#endif /* !NO_SUP_PROPERTIES */

/* ��������� ����������. */
TSupErr rndm_load_library( void );
/* �������� ����������. */
void rndm_unload_library( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _RNDM_RNDM_H ) */
