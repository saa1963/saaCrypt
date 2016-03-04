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

/*
 * ���� ���� �������� ����� �������� ������� ��� ������� ����.
 * ��� ������� ����� �� ������ �� ����������, �� ��� �����.
 */

#include <tchar.h>
#include <stdlib.h>

#if !defined( _READER_TEST_TST_RDR_H )
#define _READER_TEST_TST_RDR_H

#include "reader/rdr.h" /*+ Project (READER/RDR) include.
    ������� ��������� ������ �� �������������. +*/

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

#if !defined( _READER_TEST_DECL )
#if defined( READER_TEST_IMPORTS )
#define _READER_TEST_DECL _STDIMPL_DECL
#else /* defined( READER_TEST_IMPORTS ) */
#define _READER_TEST_DECL
#endif /* !defined( READER_TEST_IMPORTS ) */
#endif /* !defined( _READER_TEST_DECL ) */

/* **** ������� ������������ ������� ������������. **** */

/* ������� ������������ �������� ���������. */
_READER_TEST_DECL TSupErr reader_test_list_load( void );

/* ������� ������������ ������� �� ������ ���������. */
_READER_TEST_DECL TSupErr reader_test_list_out( void );

/* ������� ������������ �������� ���������. */
_READER_TEST_DECL TSupErr reader_test_list_unload( void );

/* �������� ������� ������������ ��������� ���������� ���������. */
_READER_TEST_DECL TSupErr reader_test_list_all( void );

/* **** ������� ������������ �������������� �������. **** */

/* ��������� ���������� � ������. */
_READER_TEST_DECL TSupErr reader_test_info_passwd( 
    const TSupSysEContext *context );

/* ��������� ��������� ������. */
_READER_TEST_DECL TSupErr reader_test_info_system_flag(
    const TSupSysEContext *context );

/* ��� �������������� �����. */
_READER_TEST_DECL TSupErr reader_test_info( 
    const TSupSysEContext *context );

/* **** ������� ������������, ��������� ���������� �� ������������. **** */

/* ���������� � ���������. */
_READER_TEST_DECL TSupErr reader_test_connect_carrier( 
    TSupSysEContext *context );

/* ������������ �� ��������. */
_READER_TEST_DECL TSupErr reader_test_disconnect_carrier( 
    TSupSysEContext *context );

/* ��������� / ������ ����� local machine. */
_READER_TEST_DECL TSupErr reader_test_local_machine( 
    TSupSysEContext *context, int flag );

/* **** ������� ������������ ��������. **** */

/* ������� ������������ ����������� ������ ��������. */
_READER_TEST_DECL TSupErr reader_test_unique(
    TSupSysEContext *context );

/* **** ������� ������������ ����������. **** */

/* ������� ������������ ������� ������. */
_READER_TEST_DECL TSupErr reader_test_info_passwd_clear(
    TSupSysEContext *context,
    const TCHAR *master_passwd );

/* ���� � ����������. */
_READER_TEST_DECL TSupErr reader_test_prime( 
    TSupSysEContext *context,
    const TCHAR *passwd );

/* ���� � ����������. */
_READER_TEST_DECL TSupErr reader_test_login( 
    TSupSysEContext *context,
    const TCHAR *passwd );

/* ����� �� ����������. */
_READER_TEST_DECL TSupErr reader_test_logout( 
    TSupSysEContext *context );

/* ������������. */
_READER_TEST_DECL TSupErr reader_test_lock( 
    TSupSysEContext *context );

/* ���������������. */
_READER_TEST_DECL TSupErr reader_test_unlock( 
    TSupSysEContext *context );

/* ������� ������������ ��������� ������. */
_READER_TEST_DECL TSupErr reader_test_passwd_change(
    TSupSysEContext *context,
    const TCHAR *old_passwd,
    const TCHAR *new_passwd );

/* ������� ������������ �����. */
_READER_TEST_DECL TSupErr reader_test_folder_enum( 
    const TSupSysEContext *context );

/* ������� ������������ �������� �����. */
_READER_TEST_DECL TSupErr reader_test_folder_open( 
    TSupSysEContext *context,
    const TCHAR *folder );

/* ������� ������������ �������� �����. */
_READER_TEST_DECL TSupErr reader_test_folder_create( 
    TSupSysEContext *context,
    const TCHAR *folder );

/* ������� ������������ ������� �����. */
_READER_TEST_DECL TSupErr reader_test_folder_clear( 
    TSupSysEContext *context );

/* ������������ �������� �����. */
_READER_TEST_DECL TSupErr reader_test_folder_close( 
    TSupSysEContext *context );
/* **** ������� ������������ ������. **** */

/* ������� ������������ �������� ��������, ������, �������� �����. */
_READER_TEST_DECL TSupErr reader_test_file_create( 
    TSupSysEContext*, /*+ (i) Handle �����������. +*/
    const TCHAR*, /*+ (i) ��� ������������ �����. +*/
    TRdrFileNumber, /*+ (i) ����� ������������ �����. +*/
    size_t );

/* ������� ������������ �������� ��������, ������, �������� �����. */
_READER_TEST_DECL TSupErr reader_test_file_read( 
    TSupSysEContext*, /*+ (i) Handle �����������. +*/
    const TCHAR*, /*+ (i) ��� ������������ �����. +*/
    TRdrFileNumber, /*+ (i) ����� ������������ �����. +*/
    size_t length );

/* ������� ������������ �������� ��������� ����� �����. */
_READER_TEST_DECL TSupErr reader_test_file_length( 
    TSupSysEContext* ); /*+ (i) Handle �����������. +*/

/* ������� ������������ �������� ��������� ����������� �� ����� �����. */
_READER_TEST_DECL TSupErr reader_test_file_chsize( 
    TSupSysEContext*, /*+ (i) Handle �����������. +*/
    size_t test_length );

/* ������� ������������ �������� �������� �����. */
_READER_TEST_DECL TSupErr reader_test_file_unlink( 
    TSupSysEContext*, /*+ (i) Handle �����������. +*/
    const TCHAR*, /*+ (i) ��� ���������� �����. +*/
    TRdrFileNumber ); /*+ (i) ����� ���������� �����. +*/

/* ������� ������������ �������� ������ � �������. */
_READER_TEST_DECL TSupErr reader_test_file_all( 
    TSupSysEContext*,
    size_t length );

/*+ **** ������� ������������ ����� ������. **** +*/

/* ������� ������������ � ������ ��������� �� ������. */
_READER_TEST_DECL TSupErr test_error( 
    const TSupSysEContext *context,
    TSupErr );

/*+ **** �������� ��������� �������� ����������. **** +*/

_READER_TEST_DECL TSupErr reader_test_retry_function_set(
    TSupSysEContext *context ); /*+ (i) HANDLE �����������. +*/

_READER_TEST_DECL TSupErr reader_test_retry_wait_function_set(
    TSupSysEContext *context ); /*+ (i) HANDLE �����������. +*/

#if defined( SUPPORT_RESOURCE_STD )
extern TSupResourceInstance READER_TEST_RESOURCE;
#else
extern HINSTANCE READER_TEST_RESOURCE;
#endif

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_TEST_TST_RDR_H ) */
/*+ end of file: $Id: tst_rdr.h 68640 2011-06-17 20:29:25Z dim $ +*/
