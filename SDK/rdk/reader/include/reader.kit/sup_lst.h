/*
 * Copyright(C) 2000 Проект ИОК
 *
 * Этот файл содержит информацию, являющуюся
 * собственностью компании Крипто Про.
 *
 * Любая часть этого файла не может быть скопирована,
 * исправлена, переведена на другие языки,
 * локализована или модифицирована любым способом,
 * откомпилирована, передана по сети с или на
 * любую компьютерную систему без предварительного
 * заключения соглашения с компанией Крипто Про.
 */

/*
 * \file $RCSfile$
 * \version $Revision: 75293 $
 * \date $Date: 2012-02-29 15:15:34 +0400 (Wed, 29 Feb 2012) $
 * \author $Author: dim $
 * \brief Интерфейс разработчика списка
 */

#if !defined( _SUPPORT_SUP_LST_H )
#define _SUPPORT_SUP_LST_H

#include "reader.kit/sup_sys.h"

#define SUPSYS_IDS_BLOCK_ERROR_LIST 3
#define SUPSYS_IDS_BLOCK_WND 4
#define SUPSYS_IDS_BLOCK_WND_BMP 5
#define SUPSYS_IDS_BLOCK_GROUP 6

#define SUPSYS_IDS_ERROR_LIST_BASE 0x2000U
#define SUPSYS_IDS_ERROR_LIST_LAST 0x2FFF

#define SUPSYS_IDS_WND_BASE 0x3000U
#define SUPSYS_IDS_WND_PROP_GROUP 0
#define SUPSYS_IDS_WND_PROP_COMPANY_UNKNOWN 1
#define SUPSYS_IDS_WND_PROP_VERSION_UNKNOWN 2
#define SUPSYS_IDS_WND_PROP_STATUS_DISABLED 3
#define SUPSYS_IDS_WND_PROP_STATUS_INSTALL 4
#define SUPSYS_IDS_WND_PROP_STATUS_OK 5
#define SUPSYS_IDS_WND_PROP_PROD_GROUP 6
#define SUPSYS_IDS_WND_PROP_PROD_STATUS_OK 7
#define SUPSYS_IDS_WND_PROP_PROD_STATUS_FAILED 8
#define SUPSYS_IDS_WND_PROP_PROD_COMPANY_UNKNOWN 9
#define SUPSYS_IDS_WND_PROP_PROD_VERSION_UNKNOWN 10

#define SUPSYS_IDS_WND_ADD_WINDOW_TITLE 11
#define SUPSYS_IDS_WND_ADD_CAPTION 12
#define SUPSYS_IDS_WND_ADD_WELCOME_HEADER 13
#define SUPSYS_IDS_WND_ADD_WELCOME_TEXT 14
#define SUPSYS_IDS_WND_ADD_WELCOME_FOOTER 15
#define SUPSYS_IDS_WND_ADD_FINISH_HEADER 16
#define SUPSYS_IDS_WND_ADD_FINISH_TEXT 17
#define SUPSYS_IDS_WND_ADD_FINISH_FOOTER 18
#define SUPSYS_IDS_WND_ADD_SELECT_TITLE 19
#define SUPSYS_IDS_WND_ADD_SELECT_SUBTITLE 20
#define SUPSYS_IDS_WND_ADD_SELECT_HEADER 21
#define SUPSYS_IDS_WND_ADD_SELECT_TEXT 22
#define SUPSYS_IDS_WND_ADD_SELECT_FOOTER 23
#define SUPSYS_IDS_WND_ADD_NAME_TITLE 24
#define SUPSYS_IDS_WND_ADD_NAME_SUBTITLE 25
#define SUPSYS_IDS_WND_ADD_NAME_HEADER 26
#define SUPSYS_IDS_WND_ADD_NAME_TEXT 27
#define SUPSYS_IDS_WND_ADD_NAME_FOOTER 28
#define SUPSYS_IDS_WND_ADD_CONNECT_TITLE 29
#define SUPSYS_IDS_WND_ADD_CONNECT_SUBTITLE 30
#define SUPSYS_IDS_WND_ADD_CONNECT_HEADER 31
#define SUPSYS_IDS_WND_ADD_CONNECT_TEXT 32
#define SUPSYS_IDS_WND_ADD_CONNECT_FOOTER 33
#define SUPSYS_IDS_WND_ADD_NAME_FREE 34
#define SUPSYS_IDS_WND_ADD_NAME_USED 35
#define SUPSYS_IDS_WND_ADD_CONNECT_AUTO 36
#define SUPSYS_IDS_WND_CONFIGURE_CAPTION 37
#define SUPSYS_IDS_WND_CONFIGURE_DELETE_QUESTION 38
#define SUPSYS_IDS_WND_CONFIGURE_DELETE_CAPTION 39
#define SUPSYS_IDS_WND_CONFIGURE_HEADER 40
#define SUPSYS_IDS_WND_CONFIGURE_TEXT 41
#define SUPSYS_IDS_WND_ADD_ALL_PRODUCERS 42
#define SUPSYS_IDS_WND_CHOOSE_LOCATION_FOLDER 43
#define SUPSYS_IDS_WND_CHOOSE_LOCATION_WINDOW_TITLE 44
#define SUPSYS_IDS_WND_CHOOSE_LOCATION_TITLE 45
#define SUPSYS_IDS_WND_CHOOSE_LOCATION_SUBTITLE 46
#define SUPSYS_IDS_WND_CHOOSE_LOCATION_TEXT 47
#define SUPSYS_IDS_WND_ADD2_SELECT_TEXT 48
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_TITLE 49
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_SUBTITLE 50
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_HEADER 51
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_TEXT 52
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_WINDOW_TITLE 53
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_FOOTER 54
#define SUPSYS_IDS_WND_ADD2_WELCOME_HEADER 55
#define SUPSYS_IDS_WND_ADD2_WELCOME_TEXT 56
#define SUPSYS_IDS_WND_ADD2_WELCOME_FOOTER 57
#define SUPSYS_IDS_WND_ADD2_SELECT_NEW_ERROR 58
#define SUPSYS_IDS_WND_ADD_UNKNOWN_PRODUCER 59
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_TITLE 62
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_SUBTITLE 63
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_HEADER 64
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_FOOTER 65
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_TEXT 66
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_FINISH 67
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_FINISH_EMTY 68
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_FINISH_ABORT 69
#define SUPSYS_IDS_WND_ADD2_SELECT_PROGRESSBAR_FINISH_RECONFIG 70


#define SUPSYS_IDS_WND_LAST ( SUPSYS_IDS_WND_ADD_UNKNOWN_PRODUCER + 1 )

#define SUPSYS_BMP_WND_BASE 0x1000U
#define SUPSYS_BMP_WND_ADD_BITMAP_HEADER 0
#define SUPSYS_BMP_WND_ADD_BITMAP_WATERMASK 1
#define SUPSYS_BMP_WND_LAST ( SUPSYS_BMP_WND_ADD_BITMAP_WATERMASK + 1 )

#define SUPSYS_IDS_GROUP_BASE 0x4000
#define SUPSYS_IDS_GROUP_MSI_TITLE 0
#define SUPSYS_IDS_GROUP_PREFIX_NAME 1
#define SUPSYS_IDS_GROUP_PREFIX_GROUP 2
#define SUPSYS_IDS_GROUP_LAST 3

#define SUPSYS_LIST_ITEM_FLAG_DELETE 0x1
#define SUPSYS_LIST_ITEM_FLAG_NEW 0x2

#define SUPSYSL_FUN_BASE_PATH 0x400
#define SUPSYSL_FUN_MSI_TITLE 0x401
#define SUPSYSL_FUN_PREFIX_NAME 0x402
#define SUPSYSL_FUN_PREFIX_GROUP 0x403
#define SUPSYSL_FUN_ENABLE 0x404

#define SUPSYSL_FUN_REGISTER SUPSYS_FUN_REGISTER
#define SUPSYSL_FUN_UNREGISTER SUPSYS_FUN_UNREGISTER

#define SUPSYSL_FUN_ITEM_ALLOC 0x2000
#define SUPSYSL_FUN_ITEM_FREE 0x2001
#define SUPSYSL_FUN_ITEM_DUP 0x2002
#define SUPSYSL_FUN_ITEM_COMPARE 0x2003
#define SUPSYSL_FUN_ITEM_INFO_GET 0x2004
#define SUPSYSL_FUN_ITEM_INFO_FREE 0x2005
#define SUPSYSL_FUN_ITEM_DISCONNECT 0x2006
#define SUPSYSL_FUN_ITEM_INFO_VERSION 0x2007

typedef struct TSupSysListBasePath_
{
    size_t size_of;
    TSupSysInfoText base_path;
} TSupSysListBasePath;

typedef struct TSupSysListEnable_
{
    size_t size_of;
    int enabled;
    TSupSysEContext *item;
    TSupSysEList *list;
} TSupSysListEnable;

typedef TSupSysInfoRegister TSupSysInitList;
typedef TSupSysInfoUnregister TSupSysListDone;

typedef struct TSupSysListItemAlloc_
{
    size_t size_of;
    TSupSysEContext *item;
    TSupSysEContext *owner;
    TSupSysERegisterFlags flags_mask;
    TSupSysERegisterFlags flags_value;
    TSupSysERegisterFlags flags_add;
    const TCHAR *path_to_item;
    void *add_info;
} TSupSysListItemAlloc;

typedef struct TSupSysListItemFree_
{
    size_t size_of;
    TSupSysEContext *item;
} TSupSysListItemFree;

typedef struct TSupSysListItemDisconnect_
{
    size_t size_of;
    TSupSysEContext *item;
} TSupSysListItemDisconnect;

typedef struct TSupSysListItemDup_
{
    size_t size_of;
    const TSupSysEContext *src;
    TSupSysEContext *dest;
    TSupSysEContext *dest_owner;
} TSupSysListItemDup;

typedef struct TSupSysListItemCompare_
{
    size_t size_of;
    const TSupSysEContext *item1;
    const TSupSysEContext *item2;
    int res;
} TSupSysListItemCompare;

typedef struct TSupSysListItemInfoGet_
{
    size_t size_of;
    TSupSysInfoText path;
    TSupSysInfoUntyped info;
} TSupSysListItemInfoGet;

typedef struct TSupSysListItemInfoFree_
{
    size_t size_of;
    TSupSysInfoUntyped info;
} TSupSysListItemInfoFree;

typedef TSupErr (*TSupSysListFun)( TSupSysEContext *list, TSupSysInfo *info );

typedef char TCFunName[40];

#if defined( _WIN32 )
typedef TSupErr (*TSupSysWndEnable)( TSupSysEContext *context, unsigned num );
typedef TSupErr (*TSupSysWndLevel)( const TSupSysEContext *context, unsigned *num,
    int get );
#endif

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* Функция получения порядкового номера заданной функции в таблице. */
_SUPPORT_DECL int supsys_function_n(
    const TSupSysEContext *context, TSupSysInfoFun fun );

/* Функция проверки поддерживаемости функции. */
_SUPPORT_DECL TSupErr supsys_fun(
    const TSupSysEContext *context, TSupSysInfoFun fun );

/* Функция вызова заданной функции элемента. */
_SUPPORT_DECL TSupErr supsys_call(
    const TSupSysEContext *context, TSupSysInfoFun fun,
    TSupSysInfo *info );

/* Функция получения instance элемента, только по элементу. */
_SUPPORT_DECL TSupErr supsys_get_instance(
    const TSupSysEContext *context, TSupResourceInstance *instance );

#if !defined UNIX
/* Функция получения instance списка. */
_SUPPORT_DECL TSupErr supsysl_get_instance(
    const TSupSysEList *context, TSupResourceInstance *instance );
#endif /* !UNIX */

/* Функция получения номера IDS элемента, по элементу или по списку. */
_SUPPORT_DECL TSupErr supsys_get_ids(
    const TSupSysEContext *context, size_t block,
    size_t item, size_t *real_num );

/* Функция получения номера IDS списка. */
_SUPPORT_DECL TSupErr supsysl_get_ids(
    const TSupSysEList *context, size_t block,
    size_t item, size_t *real_num );

/* Функция регистрации элемента. */
_SUPPORT_DECL TSupErr supsys_register( TSupSysEList *list,
    const TSupSysEHandle *handle, const TSupModuleInstance instance,
    TSupSysERegisterFlags flags_mask, TSupSysERegisterFlags flags_value,
    TSupSysERegisterFlags flags_add, void *add_info,
    const TCHAR *name, const TCHAR *connect, const TCHAR *item,
    const TCHAR *path_to_item, TSupSysEContext **out );

_SUPPORT_DECL TSupErr supsys_register_start(
    TSupSysEList **list,
    const TSupSysEHandle *handle );

/* Функция получения строки из ресурса элемента, только по элементу. */
_SUPPORT_DECL TSupErr supsys_get_string(
    const TSupSysEContext *context, size_t block, size_t num,
    size_t *length, TCHAR *string );

/* Функция получения строки из ресурса списка. */
_SUPPORT_DECL TSupErr supsysl_get_string(
    const TSupSysEList *list, size_t block, size_t num,
    size_t *length, TCHAR *string );

/* Функция получения иконки из ресурса элемента, только по элементу. */
_SUPPORT_DECL TSupErr supsys_get_icon(
    const TSupSysEContext *context, size_t block, size_t num,
    TSupIcon *icon );

/* Функция получения иконки из ресурса списка. */
_SUPPORT_DECL TSupErr supsysl_get_icon(
    const TSupSysEList *context, size_t block, size_t num,
    TSupIcon *icon );

/* Текст ошибки. */
_SUPPORT_DECL TSupErr supsys_error(
    const TSupSysEContext *context, TSupErr code, size_t *length,
    TCHAR *text, const TSupSysFunctionTable *global,
    TSupErr first_errcode, TSupErr last_errcode );

_SUPPORT_DECL TSupErr supsys_default_register_name(
    const TSupSysEContext *context, size_t *length,
    TCHAR *name, const TCHAR *connect );

_SUPPORT_DECL TSupErr supsys_default_internal_register_name(
    const TSupSysEContext *context, size_t *length,
    TCHAR *name, const TCHAR *connect );

#if defined _WIN32 && !defined CSP_LITE
_SUPPORT_DECL TSupErr supsys_wnd_configure( HWND hwnd,
    TSupSysEList *list, TSupSysWndEnable enabler, TSupSysWndLevel leveler );
#endif /* _WIN32 && !CSP_LITE */

_SUPPORT_DECL TSupSysEContext *supsys_owner_context(
    const TSupSysEContext *context );

_SUPPORT_DECL TSupErr supsys_set_state(
    const TSupSysEContext *context,
    TSupSysEState state );
#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _SUPPORT_SUP_LST_H ) */
/* end of file: $Id: sup_lst.h 75293 2012-02-29 11:15:34Z dim $ */
