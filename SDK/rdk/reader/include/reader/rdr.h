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
 * \version $Revision: 79078 $
 * \date $Date: 2012-06-28 18:42:26 +0400 (Thu, 28 Jun 2012) $
 * \author $Author: borodin $
 * \brief ��������� ������������.
 */

#if !defined( _READER_RDR_H )
#define _READER_RDR_H

#include "reader/sup_syse.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif /* HAVE_LIMITS_H */

#include "reader/std_decl.h"
#include "reader/support.h"

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable: 4100 4115 4201 4214 )

#include <windows.h>

#pragma warning( pop )
#endif /* defined( _MFC_VER ) */

/* **** **** defines **** **** */

#if !defined( _RDR_DECL )
#if defined( RDR_IMPORTS )
#define _RDR_DECL _STDIMPL_DECL
#else /* defined( RDR_IMPORTS ) */
#define _RDR_DECL
#endif /* !defined( RDR_IMPORTS ) */
#endif /* !defined( _RDR_DECL ) */

#if defined( _WIN32 ) && !defined (_M_ARM)
#define rdr_connect_try( rdr, code ) __try { code = supsys_connect( rdr ); if( code ) __leave; 
#define rdr_connect_finally( rdr, code ) } __finally { if( !code ) supsys_disconnect( rdr ); }
#define rdr_connect_return( rdr ) 
#define rdr_lock_try( rdr, code ) __try { code = rdr_lock( rdr ); if( code ) __leave; 
#define rdr_lock_finally( rdr, code ) } __finally { if( !code ) rdr_unlock( rdr ); }
#define rdr_lock_return( rdr ) 
#define rdr_login_try( rdr, passwd, code, retries ) __try { code = rdr_login( rdr, passwd, retries ); if( code ) __leave; 
#define rdr_prime_section( rdr, passwd, code, retries ) __try { code = rdr_prime( rdr, passwd, retries ); if( code ) __leave; 
#define rdr_login_finally( rdr, code ) } __finally { if( !code ) rdr_logout( rdr ); }
#define rdr_crear_finally( rdr, code ) rdr_folder_clear( rdr ); } __except( rdr_logout( context ), EXCEPTION_CONTINUE_SEARCH ) {}
#define rdr_login_return( rdr ) 
#else
#define rdr_connect_try( rdr, code ) code = supsys_connect( rdr ); if( !code ) {
#define rdr_connect_finally( rdr, code ) supsys_disconnect( rdr ); }
#define rdr_connect_return( rdr ) supsys_disconnect( rdr )
#define rdr_lock_try( rdr, code ) code = rdr_lock( rdr ); if( !code ) {
#define rdr_lock_finally( rdr, code ) rdr_unlock( rdr ); } 
#define rdr_lock_return( rdr ) rdr_unlock( rdr )
#define rdr_login_try( rdr, passwd, code, retries ) code = rdr_login( rdr, passwd, retries ); if( !code ) {
#define rdr_prime_section( rdr, passwd, code, retries ) code = rdr_prime( rdr, passwd, retries ); if( !code ) {
#define rdr_crear_finally( rdr, code ) rdr_folder_clear( rdr ); }
#define rdr_login_finally( rdr, code ) rdr_logout( rdr ); }
#define rdr_login_return( rdr ) rdr_logout( rdr )
#endif

/* ��������� ��������� �����������. */
#define RDR_STATE_CARRIER 11
#define RDR_STATE_PRIME 12
#define RDR_STATE_FOLDER 13
#define RDR_STATE_LOGEDIN 14
#define RDR_STATE_FILE 15

/* ���������� ��� login. */

#define RDR_LOGIN_INFO_NO 0
#define RDR_LOGIN_INFO_PASSWD 1
#define RDR_LOGIN_INFO_PASSWD_MASTER 2
#define RDR_LOGIN_INFO_PRIME_PASSWD 3
#define RDR_LOGIN_INFO_PUBLIC 4
#define RDR_LOGIN_INFO_UNKNOWN 0xffff

/* ���� ������. */
#define RDR_ERR_NO_CARRIER 0x3000
#define RDR_ERR_INVALID_MEDIA 0x3001
#define RDR_ERR_ANOTHER_MEDIA 0x3002
#define RDR_ERR_MEDIA_CHANGED 0x3003
#define RDR_ERR_WRITE_PROTECT 0x3004
#define RDR_ERR_NEED_PASSWD 0x3005
#define RDR_ERR_INVALID_PASSWD 0x3006
#define RDR_ERR_INVALID_PASSWD_FORMAT 0x3007
#define RDR_ERR_PASSWD_LOCKED 0x3008
#define RDR_ERR_FULL 0x3009
#define RDR_ERR_FILE_NOT_FOUND 0x300A
#define RDR_ERR_FILE_EXIST 0x300B
#define RDR_ERR_FILE_DISTORT 0x300C
#define RDR_ERR_PRIME 0x300D
#define RDR_ERR_FORMAT 0x300E
#define RDR_ERR_VERSION_1 0x300F

#define RDR_ERR_BLOCK 0x30FC
#define RDR_ERR_RETRY_ATTACH 0x30FD
#define RDR_ERR_RETRY_DUP 0x30FE
#define RDR_ERR_RETRY_FREE 0x30FF
#define RDR_ERR_INVALID 0x3E00

/* ����� �����������. */
#define RDR_FLAG_BLOCK_RDR 0x3000
#define RDR_FLAG_RDR_LONG_NAMES 0
#define RDR_FLAG_RDR_REMOVABLE 1
#define RDR_FLAG_RDR_UNIQUE 2
#define RDR_FLAG_RDR_FOLDERS 3
#define RDR_FLAG_RDR_FOLDER_ROOT 4
#define RDR_FLAG_RDR_VERSION_1 (RDR_FLAG_RDR_FOLDER_ROOT+1)
#define RDR_FLAG_RDR_PROTECTED 5
#define RDR_FLAG_RDR_VERSION_2 (RDR_FLAG_RDR_PROTECTED+1)
#define RDR_FLAG_RDR_FKC_SUPPORTED 6
#define RDR_FLAG_RDR_VERSION_3 (RDR_FLAG_RDR_FKC_SUPPORTED+1)
#define RDR_FLAG_RDR_HASH_SUPPORTED 7
#define RDR_FLAG_RDR_VERSION_4 (RDR_FLAG_RDR_HASH_SUPPORTED+1)


#define RDR_FOLDER_ENUM_FLAG_NEW 0x1

#define RDR_SAME_WRITE_AVAIL 0x1

/* �������� �����������. **** +*/
typedef int TRdrFileNumber;

/*! \ingroup rdr_ext_chl_struct
 * \brief ������������� ��������� � ������� ��������������
 * \req_rdr
 */
typedef unsigned int TRdrLoginInfoType;
typedef TSupErr (*TRdrRetryProc)( void *info, TSupErr code );
typedef struct TRdrRetryContextDup_
{
    size_t size_of;
    TSupSysEContext *dest_context;
    void *src_info;
    void *dest_info;
} TRdrRetryContextDup;

typedef struct TRdrRetryContextAttach_
{
    size_t size_of;
    TSupSysEContext *context;
    void *info;
} TRdrRetryContextAttach;

struct TRdrFolderEnumContext_;

typedef struct TRdrFolderEnumContext_ TRdrFolderEnumContext;

#if defined( DUMMY_STRUCT )
struct TRdrFolderEnumContext_
{
    TDummy dummy;
}
#endif
/* **** **** �������. **** **** */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** ������� ������ �� ������� ������������. **** */

/* ������� ��������������� �������� � ������. */
_RDR_DECL TSupErr rdr_register_start( 
    TSupSysEList **list );

_RDR_DECL TSupErr rdr_media_register_start( 
    TSupSysEList **list );

#if !defined UNIX
/* ������� ��������� ����������� �� ���������. ASCII ������. */
_RDR_DECL TSupErr rdr_context_default_get(
    const TSupSysEList *list, TSupSysEContext **context );

/* ������� ��������� ����������� �� ���������. ASCII ������. */
_RDR_DECL TSupErr rdr_context_default_set(
    const TSupSysEContext *context );

#endif /* !UNIX */

#if defined( _WIN32 ) 
/* ������� ��������� ����������� ������������ */
_RDR_DECL TSupErr rdr_set_sec_descr ( 
				     TSupSysEContext *context, 
				     SECURITY_INFORMATION sInfo,
				     PSECURITY_DESCRIPTOR pSecDescr,
				     HRESULT * out_error );

_RDR_DECL TSupErr rdr_get_sec_descr ( 
				     TSupSysEContext *context,
				     SECURITY_INFORMATION requestInfo,
				     PSECURITY_DESCRIPTOR pSecDescr,
				     DWORD * dbSecDescrLen,
				     HRESULT * out_error );
#endif /*_WIN32 */

/* **** �������������� �������. **** */

/* ������� ��������� ���� ��������. ASCII ������. */
_RDR_DECL TSupErr rdr_info_carrier_type(
    const TSupSysEContext *context, TSupSysENickname ); 

/* **** ������� ������ � ���������. **** */
_RDR_DECL TSupErr rdr_connect_carrier( 
    TSupSysEContext *context );

_RDR_DECL TSupErr rdr_format_carrier( 
    TSupSysEContext *context );

_RDR_DECL TSupErr rdrint_connect_carrier( 
    TSupSysEContext *context, int reader_type );

_RDR_DECL TSupErr rdrint_format_carrier( 
    TSupSysEContext *context, int reader_type );

_RDR_DECL TSupErr rdr_disconnect_carrier( 
    TSupSysEContext *context );

/* **** ������� ������ � ������������. **** */
TSupErr rdr_local_machine(
    TSupSysEContext *context, int local_machine );

/* **** ������� ������ � �������. **** */

/* ������� ��������� ������������� ������� ������. */
_RDR_DECL TSupErr rdr_passwd_length(
    const TSupSysEContext *context, TRdrLoginInfoType *type, 
    size_t *max_length, size_t *min_length );

/* ������� ��������� ������� ������. */
_RDR_DECL TSupErr rdr_passwd_term(
    const TSupSysEContext *context, size_t *length, TCHAR *term );

/* ������� ����� ������. */
_RDR_DECL TSupErr rdr_passwd_change(
    TSupSysEContext *context, const TCHAR *new_passwd );

/* ������� ������� ������. */
_RDR_DECL TSupErr rdr_passwd_clear(
    TSupSysEContext *context, const TCHAR *master_passwd );

/* ������� ��������� �������� ����� ����� ������. */
_RDR_DECL TSupErr rdr_passwd_phrase(
    TSupSysEContext *context, size_t *length, TCHAR *phrase );

/* ������� ��������� ������ �� ���������. */
_RDR_DECL TSupErr rdr_passwd_default_get( 
    const TSupSysEContext *context, 
    size_t *passwd_length, TCHAR *passwd );

/* ������� ��������� ������ �� ���������. */
_RDR_DECL TSupErr rdr_passwd_default_set(
    const TSupSysEContext *context, const TCHAR *passwd );

/* ������� �������� ������ �������������� �� ���������. */
_RDR_DECL TSupErr rdr_passwd_default_clear( 
    const TSupSysEContext *context );

/* ������� �������� ���� ������� ������������� �� ���������. */
_RDR_DECL TSupErr rdr_passwd_default_clear_all( 
    int machine );

/* **** ������� � shortcut. **** */

/* ������� ��������� ����������� ������ �� shortcut. */
_RDR_DECL TSupErr rdr_shortcut_get( 
    int local_machine, const TCHAR *shortcut,
    size_t *unique_length, TCHAR *unique, 
    size_t *folder_length, TCHAR *folder,
    size_t *untreated_length, TCHAR *untreated,
    TSupSysENickname media_type );

/* ������� ��������� shortcut. */
_RDR_DECL TSupErr rdr_shortcut_set(
    const TSupSysEContext *context, const TCHAR *shortcut,
    const TCHAR *untreated );

/* ������� �������� shortcut. */
_RDR_DECL TSupErr rdr_shortcut_clear(
    int local_machine, const TCHAR *shortcut );

/* **** ������� ������ � ������. **** */

_RDR_DECL TSupErr rdr_folder_enum_open( 
    const TSupSysEContext *context, 
    TRdrFolderEnumContext **enum_ctx,
    size_t *folder_length,
    const TCHAR *prototype,
    unsigned flags );

_RDR_DECL TSupErr rdr_folder_enum_next(
    TRdrFolderEnumContext *enum_ctx,
    TCHAR *folder );

_RDR_DECL TSupErr rdr_folder_enum_close(
    TRdrFolderEnumContext *enum_ctx );

_RDR_DECL TSupErr rdr_folder_open(
    TSupSysEContext *context, 
    const TCHAR *mode,
    const TCHAR *folder );

_RDR_DECL TSupErr rdr_folder_close(
    TSupSysEContext *context );

_RDR_DECL TSupErr rdr_folder_clear(
    TSupSysEContext *context );

/* **** ������� ������ � �������������� ����������� ��� login. */

/* **** Login & Logout **** */

/* ������� login �� ��������. */
_RDR_DECL TSupErr rdr_login(
    TSupSysEContext *context, 
    const TCHAR *passwd,
    int *retries );

/* ������� ����� PUK. */
_RDR_DECL TSupErr rdr_puk_enter(
    TSupSysEContext *context, 
    const TCHAR *passwd,
    int *retries );

_RDR_DECL TSupErr rdr_prime(
    TSupSysEContext *context, 
    const TCHAR *passwd,
    int *retries );

/* ������� �� ��������� ������� � ������� ��������� � ������. */
_RDR_DECL TSupErr rdr_prime_login( TSupSysEContext *context );

/* ������� logout � ��������. */
_RDR_DECL TSupErr rdr_logout(
    TSupSysEContext *context );

/* **** Lock & Unlock **** */

_RDR_DECL TSupErr rdr_lock(
    TSupSysEContext *context );

_RDR_DECL TSupErr rdr_unlock(
    TSupSysEContext *context );

/* **** ������� ������ � �������. **** */

/* ������� �������� ����� �� ��������. */
_RDR_DECL TSupErr rdr_file_open(
    TSupSysEContext *context, const TCHAR *mode, const TCHAR *file_name,
    TRdrFileNumber filno );

/* ������� �������� ����� �� ��������. */
_RDR_DECL TSupErr rdr_file_close(
    TSupSysEContext *context );

/* ������� ������ ��������� �� ��������. */
_RDR_DECL TSupErr rdr_file_write(
    TSupSysEContext *context, size_t pos, size_t size, const void *buffer,
    size_t *writen );

/* ������� ������ ��������� � ��������. */
_RDR_DECL TSupErr rdr_file_read(
    TSupSysEContext *context, size_t pos, size_t size, void *buffer,
    size_t *rfread );

/* ������� ��������� ����� �����. */
_RDR_DECL TSupErr rdr_file_length(
    TSupSysEContext *context, size_t *size );

/* ������� ��������� ������������� ������� �����. */
_RDR_DECL TSupErr rdr_file_chsize(
    TSupSysEContext *context, size_t *size );

/* ������� �������� ����� �� ��������. */
_RDR_DECL TSupErr rdr_file_unlink(
    TSupSysEContext *context, const TCHAR *file_name, TRdrFileNumber rffileno );

/* **** ������� ������ ���������� ������� ����� ����� **** */
_RDR_DECL TSupErr rdr_clear_tries(
    TSupSysEContext *context, const TCHAR *passwd, int number );

/* **** ������� ������ � ���������� ������� ��������. **** */

/* ������� ��������� ����������� ������ ��������. */
_RDR_DECL TSupErr rdr_unique_get(
    const TSupSysEContext *context, size_t *length, TCHAR *unique );

/* ������� ��������� ����������� ������ ��������. */
_RDR_DECL TSupErr rdr_unique_set(
    TSupSysEContext *context, TCHAR *unique );

/* **** ������� ������. **** */

/* ������� ��������� ���� ������. ASCII ������. */
_RDR_DECL TSupErr rdr_error(
    const TSupSysEContext *context, TSupErr err, size_t *length, 
    TCHAR *text ); 

/* **** ������� ������a. **** */

/* ������� ��������� ���� �������� */
_RDR_DECL TSupErr rdr_carrier_type_get( 
    const TSupSysEContext *context, TSupSysENickname carrier_type, const TCHAR * path_to_item );

/* ������� �������� ���� �� �������� � ����������� */
_RDR_DECL TSupErr rdr_same_media(
    const TSupSysEContext *context, size_t *length, TCHAR *unique,
    unsigned flags );

/* ���������� ������� �������� ���� �� �������� � ����������� */
_RDR_DECL TSupErr rdrint_same_media(
    const TSupSysEContext *context, size_t *length, TCHAR *unique,
    unsigned flags, int reader_type );

/* ������� ��������� � �������� �������� ������� ������� ��������. */
_RDR_DECL TSupErr rdr_retry_proc_set( 
    TSupSysEContext *context, TRdrRetryProc proc, void *info );

/* ������� ��������� ���� ������, ������� ����� ��������� ��� ���������
    � �������� � ��� �� ���������. */
_RDR_DECL TSupErr rdr_cancel_consequences( 
    const TSupSysEContext *context );

/* �������������� ����������������� ����� ���������� �������� � cancel
    ������. */
// XXXX dim : static
//_RDR_DECL TSupErr rdr_cancel_notify(
//    const TSupSysEContext *context );

/* **** ������� ��������� / �������� ����������. **** */

/* ��������� ����������. */
TSupErr rdr_load_library( void );

/* �������� ���������� � ���������������. */
void rdr_unload_library( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_RDR_H_ALL ) */
/* end of file:Id: rdr.h,v 1.40 2001/01/22 09:42:09 cav Exp  */
