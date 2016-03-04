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
 * \file $RCSfile$
 * \version $Revision: 76279 $
 * \date $Date: 2012-03-28 12:25:07 +0400 (Wed, 28 Mar 2012) $
 * \author $Author: sagafyin $
 * \brief ����������, ������� ���������� ������� ������ ��� �������������� 
 *  ������ �� ���� �������. 
 */

#if !defined( _SUPPORT_SUP_SYSE_H )
#define _SUPPORT_SUP_SYSE_H

#include "reader/support.h"

#define SUPSYSE_FLAG_BLOCK_GENERAL 0
#define SUPSYSE_FLAG_GENERAL_ADD_LIBRARY 0
#define SUPSYSE_FLAG_GENERAL_VERSION_1 1
#define SUPSYSE_FLAG_GENERAL_SUID 1
#define SUPSYSE_FLAG_GENERAL_VERSION_2 2

#define SUPSYSE_NICKNAME_LENGTH 0x40

#define SUPSYSE_REGISTER_FLAG_FAILED 0x1
#define SUPSYSE_REGISTER_FLAG_DISABLED 0x2
#define SUPSYSE_REGISTER_FLAG_INSTALL_ADD 0x4
#define SUPSYSE_REGISTER_FLAG_DISCONNECT 0x8
#define SUPSYSE_REGISTER_FLAG_NOCONNECT 0x10
#define SUPSYSE_REGISTER_FLAG_REREAD 0x20
#define SUPSYSE_REGISTER_FLAG_NOTIFY 0x40
#define SUPSYSE_REGISTER_FLAG_NO_REGISTER 0x80
#define SUPSYSE_REGISTER_FLAG_PNPGROUP 0x100
#define SUPSYSE_REGISTER_FLAG_ALL ( SUPSYSE_REGISTER_FLAG_FAILED \
    | SUPSYSE_REGISTER_FLAG_DISABLED \
    | SUPSYSE_REGISTER_FLAG_INSTALL_ADD \
    | SUPSYSE_REGISTER_FLAG_DISCONNECT \
    | SUPSYSE_REGISTER_FLAG_NOCONNECT \
    | SUPSYSE_REGISTER_FLAG_PNPGROUP )

#define SUPSYSE_STATE_UNKNOWN 0
#define SUPSYSE_STATE_PRE_INSTALLED 1
#define SUPSYSE_STATE_NINSTALL_ADD 2
#define SUPSYSE_STATE_NLOADED 3
#define SUPSYSE_STATE_INSTALLED 4
#define SUPSYSE_STATE_INSTALLED_ADD 5
#define SUPSYSE_STATE_LOADED 6
#define SUPSYSE_STATE_REGISTERED 7
#define SUPSYSE_STATE_CONNECTED 8

/* ����� ������ ������� ����������� */
#define SUPSYSE_MEDIA_ICON_EMPTY 0
/* ����� ������ ����������� �� ����������� ��������� */
#define SUPSYSE_MEDIA_ICON_INSERTED 1
/* ����� ������ ����������� �� ����������� ���������, � ������� */
#define SUPSYSE_MEDIA_ICON_ERROR 2
/* ����� ������ ����������� �� ����������� ���������, � ����������� ���������� */
#define SUPSYSE_MEDIA_ICON_UNKNOWN 3
/* ����� ������ ����������� �� ����������� ���������, ������������ */
#define SUPSYSE_MEDIA_ICON_BLOCKED 4

typedef TCHAR TSupSysENickname[SUPSYSE_NICKNAME_LENGTH + 1];
struct TSupSysEFlags_;
typedef struct TSupSysEFlags_ TSupSysEFlags;
struct TSupSysEContext_;
typedef struct TSupSysEContext_ TSupSysEContext;
struct TSupSysEConnectContext_;
typedef struct TSupSysEConnectContext_ TSupSysEConnectContext;
struct TSupSysEList_;
typedef struct TSupSysEList_ TSupSysEList;
struct TSupSysEFind_;
typedef struct TSupSysEFind_ TSupSysEFind;
struct TSupSysEHandle_;
typedef struct TSupSysEHandle_ TSupSysEHandle;
struct TSupSysEGroupEnum_;
typedef struct TSupSysEGroupEnum_ TSupSysEGroupEnum;

typedef unsigned TSupSysERegisterFlags;

typedef unsigned TSupSysEState;

#if defined( DUMMY_STRUCT )
typedef int TDummy;

struct TSupSysEFlags_
{
    TDummy dummy;
};

struct TSupSysEContext_
{
    TDummy dummy;
};

struct TSupSysEConnectContext_
{
    TDummy dummy;
} TSupSysEConnectContext;

struct TSupSysEList_
{
    TDummy dummy;
};

struct TSupSysEFind_
{
    TDummy dummy;
};

struct TSupSysEHandle_
{
    TDummy dummy;
};

struct TSupSysEGroupEnum_
{
    TDummy dummy;
};
#endif /* defined( DUMMY_STRUCT ) */

struct TSupSysFindInfo_;
typedef struct TSupSysFindInfo_ TSupSysFindInfo;

typedef int (*TSupSysFindValidFun)( const TSupSysFindInfo *add_info,
    const TSupSysEContext *item );
typedef void (*TSupSysFindFreeFun)( TSupSysFindInfo *find_ctx );

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* ������ ��������� ���������� ��������. */
_SUPPORT_DECL TSupErr supsys_version( 
    const TSupSysEContext *context, unsigned int version[4] );

/* �������� ��������. */
_SUPPORT_DECL TSupErr supsys_nickname( 
    const TSupSysEContext *context, TSupSysENickname nickname );

/* ��� ��������. */
_SUPPORT_DECL TSupErr supsys_name( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

/* �������� ��� ��������, ��� ��� �� ���������������. */
_SUPPORT_DECL TSupErr supsys_register_name( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

/* Copyright ��������. */
_SUPPORT_DECL TSupErr supsys_copyright( 
    const TSupSysEContext *context, size_t *length, TCHAR *copyright );

/* ��������� ��� ��������. */
_SUPPORT_DECL TSupErr supsys_detail( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

/* ��� ������������� ����������. */
_SUPPORT_DECL TSupErr supsys_producer( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

/* ������ �� ������������� ����������. */
_SUPPORT_DECL TSupErr supsys_producer_version( 
    const TSupSysEContext *context, size_t *length, TCHAR *version );

/* �������� ������������� ��������� ����������. */
_SUPPORT_DECL TSupErr supsys_company( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

/* ������� ��������� ������ ��������. */
_SUPPORT_DECL TSupErr supsys_icon( 
    const TSupSysEContext *context, TSupIcon *icon );

/* ������� ��������� ������ ��������. */
_SUPPORT_DECL TSupErr supsys_icons( 
    const TSupSysEContext *context, TSupIcon *icons, int iconnum );

/* ������� �������� ������������ ��������. */
_SUPPORT_DECL TSupErr supsys_find_open( 
    const TSupSysEList *list, TSupSysEFind **context );

/* ������� �������� ������������ ��������. */
_SUPPORT_DECL TSupErr supsys_find_open_ext(
    const TSupSysEList *list, TSupSysEFind **context, TSupSysFindInfo *add_info,
    TSupSysFindValidFun valid_fun, TSupSysFindFreeFun free_fun );

/* ������� ��������� ��������� ��������. */
_SUPPORT_DECL TSupErr supsys_find_next( 
    TSupSysEFind *context, TSupSysEContext **ctx );

/* ������� ��������� ��������� �������� �� ��������. */
_SUPPORT_DECL TSupErr supsys_find_nickname(
    const TSupSysEList *list, TSupSysENickname nickname, 
    const TCHAR *ssconnect, TSupSysEContext **ctx );

/* ������� �������� ������������ ����������. */
_SUPPORT_DECL TSupErr supsys_connect_list_open(
    const TSupSysEContext *context, size_t *length, 
    TSupSysEConnectContext **list_context );

/* ������� ��������� ���������� ���� �����������. */
_SUPPORT_DECL TSupErr supsys_connect_list_next( 
    TSupSysEConnectContext *list_context, TCHAR *ssconnect );

/* ������� ����������� ������������ ����������. */
_SUPPORT_DECL TSupErr supsys_connect_list_close( 
    TSupSysEConnectContext *list_context );

/* ��������� �������� ����� ����������. */
_SUPPORT_DECL TSupErr supsys_connect_current( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );

#if !defined UNIX
/* �������� ��������� ���������� ����. */
_SUPPORT_DECL TSupErr supsys_connect_auto_sup( 
    const TSupSysEContext *context, size_t *length, TCHAR *name );
#endif /* !UNIX */

/* ���� �����. */
_SUPPORT_DECL TSupErr supsys_connect_auto( 
    const TSupSysEContext *context, size_t *length, TCHAR *name,
    size_t exclude_length, TCHAR **excludes );
    
/* �������� ������� ����������. */
_SUPPORT_DECL TSupErr supsys_connect_valid( 
    const TSupSysEContext *context, const TCHAR *ssconnect );

/* ����������. */
_SUPPORT_DECL TSupErr supsys_connect( 
    TSupSysEContext *context );

/* ����������. */
_SUPPORT_DECL TSupErr supsys_disconnect( 
    TSupSysEContext *context );

#if !defined UNIX
/* �������� ������������� ����������. */
_SUPPORT_DECL TSupErr supsys_connect_is_used( 
    const TSupSysEContext *context, const TCHAR *ssconnect );

/* �������� ������������� ����������� �����. */
_SUPPORT_DECL TSupErr supsys_parse_internal_name( 
    const TSupSysEList *list, const TCHAR *name, TSupSysEContext **context );
#endif /* !UNIX */

_SUPPORT_DECL TSupErr supsys_group_enum_open( 
    const TSupSysEContext *group, size_t *length, 
    TSupSysEGroupEnum **group_enum );

_SUPPORT_DECL TSupErr supsys_group_enum_close(
    TSupSysEGroupEnum *group_enum );

_SUPPORT_DECL TSupErr supsys_group_enum_next(
    TSupSysEGroupEnum *group_enum, TCHAR *item );

/* �������� ������. */
_SUPPORT_DECL TSupErr supsys_group_nickname( 
    const TSupSysEContext *context, TSupSysENickname nickname );

/* ������� �������� ������������ ��������. */
_SUPPORT_DECL TSupErr supsys_find_close( TSupSysEFind *context );

/* ������� �������� HANDLE �� �������. */
_SUPPORT_DECL TSupErr supsys_context_free( TSupSysEContext *ctx ); 

/* ������� ������������ ��������� ��������. */
_SUPPORT_DECL TSupErr supsys_context_dup( 
    const TSupSysEContext *src, TSupSysEContext **dest );

/* ���������� / ���������� ��������. */
_SUPPORT_DECL TSupErr supsys_enable( 
    TSupSysEContext *ctx, const TCHAR *ssconnect, const TCHAR *name,
    int enabling );

/* ������� ��������� ������ ��������. */
_SUPPORT_DECL TSupErr supsys_flags(
    const TSupSysEContext *context, size_t type, size_t *quant,
    unsigned char *flags );

/* ������� ��������� ����� ��������. */
_SUPPORT_DECL TSupErr supsys_flag( 
    const TSupSysEContext *context, size_t type, size_t num,
    int *flag );

/* ������� ��������� ������ ����������� ��������. */
_SUPPORT_DECL TSupSysEState supsys_get_state(
    const TSupSysEContext *context );

/* ������� ������ �������������� ���� ���������. */
_SUPPORT_DECL TSupErr supsys_unregister_all( TSupSysEList *list, 
    int unload );

/* ������� ����������� ���� ��������� ������. */
_SUPPORT_DECL TSupErr supsys_register_all_ext( 
    TSupSysEList *list, TSupSysERegisterFlags flags_mask, 
    TSupSysERegisterFlags flags_value );

/* ������� ����������� ���� ��������� ������. */
_SUPPORT_DECL TSupErr supsys_register_all( 
    TSupSysEList *list );

#if !defined UNIX
_SUPPORT_DECL TSupErr supsys_list_notificator( 
    const TSupSysEList *list, TSupRegNotificator **notificator );
#endif /* !UNIX */

_SUPPORT_DECL TSupErr supsys_connect_notificator( 
    const TSupSysEContext *context, TSupRegNotificator **notificator );

/* �������, ����������� �� supsys */
_SUPPORT_DECL TSupErr support_default_internal_register_name( 
    const TSupSysEContext *context, size_t *length, TCHAR *name, 
    const TCHAR *ssconnect );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _SUPPORT_SUP_SYSE_H ) */
/* end of file: $Id: sup_syse.h 76279 2012-03-28 08:25:07Z sagafyin $ */
