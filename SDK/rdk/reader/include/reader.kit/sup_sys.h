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
 * \version $Revision: 76279 $
 * \date $Date: 2012-03-28 12:25:07 +0400 (Wed, 28 Mar 2012) $
 * \author $Author: sagafyin $
 * \brief Объявления необходимые для написания поддержки элемента
 *  перечисления.
 */

#if !defined( _SUPPORT_SUP_SYS_H )
#define _SUPPORT_SUP_SYS_H

#include "reader/support.h"
#include "reader/sup_syse.h"

/*-------DBPrint----------------------------*/
#define SUPSYS_FTEXT(x) SUPPORT_DBFTEXT_(support_context_io, DTEXT( x ) )
#define SUPSYS_PRINTSTR(level,param) CONCAT(SUPPORT_DB_QPRINT_,level)(support_context_io,N_##level,param)
#define SUPSYS_PRINTHEX(level,buffer,length) CONCAT(SUPPORT_DB_QPRINT_HEX_,level)(support_context_io,N_##level,DTEXT(__FILE__), __LINE__, SFUNC,NULL,buffer,length)

typedef enum NSupSysTypeO_ { 
    N_SUPSYS_TYPEO_ERROR = 1,
    N_SUPSYS_TYPEO_TRACE = 2,
    N_SUPSYS_TYPEO_TEST = 4
} NSupSysTypeO;

/* Настройка на вывод */
#define SUPSYS_TYPEO_ERROR CONS
#define SUPSYS_TYPEO_TRACE CONS
#define SUPSYS_TYPEO_TEST CONS

#ifdef DEBUG
#define SUPSYS_PRE_ACTION SUP_STOP
#else
#define SUPSYS_PRE_ACTION return SUP_ERR_PARAM
#endif

#define SUPSYS_PRE(x) { if( !(x) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_INFO(x,t) SUPSYS_PRE_WRITE_PTRS(x,sizeof(t)); \
    SUPSYS_PRE( ( *(size_t*)info ) >= (sizeof(t)) )
#define SUPSYS_PRE_CONTEXT(x,t) SUPSYS_PRE_WRITE_PTRS(x,sizeof(t))
#define SUPSYS_PRE_CCONTEXT(x,t) SUPSYS_PRE_READ_PTRS(x,sizeof(t))
#define SUPSYS_PRE_READ_PTRS(x,s) { if( !IS_READ_PTRS(x,s) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_WRITE_PTRS(x,s) { if( !IS_WRITE_PTRS(x,s) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_READ_PTR(x) { if( !IS_READ_PTR(x) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_WRITE_PTR(x) { if( !IS_WRITE_PTR(x) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STRING_PTRS(x,s) { if( !IS_STRING_PTRS(x,s) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STRING_PTR(x) { if( !IS_STRING_PTR(x) ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_FUN(x) { if( !IS_FUN_PTR(x) ) SUPSYS_PRE_ACTION; }

#ifdef DEBUG
#define SUPSYS_PRE_STATE_LT(x,y) { if( supsys_get_state(x) >= y ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STATE_LE(x,y) { if( supsys_get_state(x) > y ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STATE_EQ(x,y) { if( supsys_get_state(x) != y ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STATE_GE(x,y) { if( supsys_get_state(x) < y ) SUPSYS_PRE_ACTION; }
#define SUPSYS_PRE_STATE_GT(x,y) { if( supsys_get_state(x) <= y ) SUPSYS_PRE_ACTION; }
#else /* DEBUG */
#define SUPSYS_PRE_STATE_LT(x,y)
#define SUPSYS_PRE_STATE_LE(x,y)
#define SUPSYS_PRE_STATE_EQ(x,y)
#define SUPSYS_PRE_STATE_GE(x,y)
#define SUPSYS_PRE_STATE_GT(x,y)
#endif /* DEBUG */

#define SUPSYS_FUN_VERSION 0
#define SUPSYS_FUN_NICKNAME 0x1
#define SUPSYS_FUN_NAME 0x2
#define SUPSYS_FUN_COPYRIGHT 0x3
#define SUPSYS_FUN_ERROR 0x4
#define SUPSYS_FUN_INSTANCE 0x5
#define SUPSYS_FUN_CONTEXT_DUP 0x6
#define SUPSYS_FUN_CONTEXT_FREE 0x7
#define SUPSYS_FUN_IDS_BLOCK 0x8
#define SUPSYS_FUN_ITEM_ICON 0x9
#define SUPSYS_FUN_DETAIL_NAME 0xA
#define SUPSYS_FUN_COMPANY 0xB
#define SUPSYS_FUN_FLAGS 0xC
#define SUPSYS_FUN_CALL 0xD
#define SUPSYS_FUN_GROUP_ENUM_OPEN 0xE
#define SUPSYS_FUN_GROUP_ENUM_NEXT 0xF
#define SUPSYS_FUN_GROUP_ENUM_CLOSE 0x10
#define SUPSYS_FUN_GROUP_NICKNAME 0x11
#define SUPSYS_FUN_VERSIONSUPPORT 0x12

#define SUPSYS_FUN_REGISTER 0x1000
#define SUPSYS_FUN_UNREGISTER 0x1001
#define SUPSYS_FUN_PRODUCER 0x1002
#define SUPSYS_FUN_PRODUCER_VERSION 0x1003
#define SUPSYS_FUN_CONNECT_OPEN 0x1004
#define SUPSYS_FUN_CONNECT_NEXT 0x1005
#define SUPSYS_FUN_CONNECT_CLOSE 0x1006
#define SUPSYS_FUN_CONNECT_VALID 0x1007
#define SUPSYS_FUN_CONNECT 0x1008
#define SUPSYS_FUN_DISCONNECT 0x1009
#define SUPSYS_FUN_CONNECT_REMOVE 0x100A

#define SUPSYS_FUN_START ((TSupSysInfoFun)UINT_MAX)

#define SUPSYS_FLAG_BLOCK_CONNECT 1
#define SUPSYS_FLAG_CONNECT_MANY 0
#define SUPSYS_FLAG_CONNECT_COM 1
#define SUPSYS_FLAG_CONNECT_LPT 2
#define SUPSYS_FLAG_CONNECT_AUTO 3
#define SUPSYS_FLAG_CONNECT_VERSION_1 4
#define SUPSYS_FLAG_CONNECT_MULTY 4
#define SUPSYS_FLAG_CONNECT_VERSION_2 5
#define SUPSYS_FLAG_CONNECT_PNPGROUP 5
#define SUPSYS_FLAG_CONNECT_VERSION_3 6

#define SUPSYS_CONNECT_START 0
#define SUPSYS_CONNECT_AUTO 1
#define SUPSYS_CONNECT_COM1 0x10
#define SUPSYS_CONNECT_COM2 0x11
#define SUPSYS_CONNECT_COM3 0x12
#define SUPSYS_CONNECT_COM4 0x13
#define SUPSYS_CONNECT_COM_MAX SUPSYS_CONNECT_COM4
#define SUPSYS_CONNECT_LPT1 0x20
#define SUPSYS_CONNECT_LPT2 0x21
#define SUPSYS_CONNECT_LPT_MAX SUPSYS_CONNECT_LPT2
#define SUPSYS_CONNECT_USER 0x100
#define SUPSYS_CONNECT_MAX UINT_MAX

#define SUPSYS_ERR_USER_FIRST 0x1100
#define SUPSYS_ERR_USER_LAST 0x1fff

#define SUPSYS_IDS_BLOCK_BASE 0
#define SUPSYS_IDS_BLOCK_ERROR 1
#define SUPSYS_IDS_BLOCK_ICON 2
#define SUPSYS_IDS_BLOCK_ICON_EXTENDED 7

#define SUPSYS_IDS_BASE 0x1000
#define SUPSYS_IDS_NAME 0
#define SUPSYS_IDS_NICKNAME 1
#define SUPSYS_IDS_DETAIL_NAME 2
#define SUPSYS_IDS_PRODUCER 3
#define SUPSYS_IDS_GROUP_NICKNAME 4
#define SUPSYS_IDS_GROUP_NAME 5
#define SUPSYS_IDS_BASE_QUANT_V1 ( SUPSYS_IDS_GROUP_NICKNAME + 1 )
#define SUPSYS_IDS_BASE_QUANT_V2 ( SUPSYS_IDS_GROUP_NAME + 1 )

#define SUPSYS_IDS_ERROR_BASE 0x1100
#define SUPSYS_IDS_ERROR_QUANT_V1 0xEFF

#define SUPSYS_IDS_ICON_BASE 0x1000
#define SUPSYS_IDS_ICON_ITEM 0
#define SUPSYS_IDS_ICON_QUANT_V1 ( SUPSYS_IDS_ICON_ITEM + 1 )

#define SUPSYS_IDS_ICON_BASE_EXTENDED 0x1020
#define SUPSYS_IDS_ICON_ITEM_INSERTED 1
#define SUPSYS_IDS_ICON_ITEM_ERROR 2
#define SUPSYS_IDS_ICON_ITEM_UNKNOWN 3
#define SUPSYS_IDS_ICON_ITEM_BLOCKED 4
#define SUPSYS_IDS_ICON_EXTENDED_QUANT_V1 ( SUPSYS_IDS_ICON_ITEM_BLOCKED + 1 )
#define SUPSYS_IDS_ICONS_NUMBER (SUPSYS_IDS_ICON_EXTENDED_QUANT_V1)

typedef unsigned TSupSysConnectNum;
typedef int TSupSysInfoFun;
typedef void TSupSysInfo;
typedef int TSupSysDummy;
typedef void TSupSysContext;

typedef struct TSupSysInfoDummy_
{
    size_t size_of;
} TSupSysInfoDummy;

#ifndef TSUPSYSINFOUNTYPED
#define TSUPSYSINFOUNTYPED

typedef struct TSupSysInfoUntyped_
{
    size_t length;
    unsigned char *info;
} TSupSysInfoUntyped;

typedef struct TSupSysInfoText_
{
    size_t length;
    TCHAR *text;
} TSupSysInfoText;
#endif /*TSUPSYSINFOUNTYPED*/

typedef struct TSupSysInfoBitString_
{
    size_t bits;
    unsigned char *string;
} TSupSysInfoBitString;

typedef struct TSupSysInfoNickname_
{
    size_t size_of;
    TSupSysENickname nickname;
} TSupSysInfoNickname;


typedef struct TSupSysInfoVersion_
{
    size_t size_of;
    unsigned int version[4];
} TSupSysInfoVersion;

typedef struct TSupSysInfoItemName_
{
    size_t size_of;
    TSupSysInfoText name;
} TSupSysInfoItemName;

typedef struct TSupSysInfoCopyright_
{
    size_t size_of;
    TSupSysInfoText copyright;
} TSupSysInfoCopyright;

typedef struct TSupSysInfoError_
{
    size_t size_of;
    TSupErr code;
    TSupSysInfoText message;
} TSupSysInfoError;

typedef struct TSupSysInfoInstance_
{
    size_t size_of;
    TSupResourceInstance instance;
} TSupSysInfoInstance;

typedef struct TSupSysInfoContextDup_
{
    size_t size_of;
    TSupSysContext *dest_context;
} TSupSysInfoContextDup;

typedef struct TSupSysInfoDummy_ TSupSysInfoContextFree;

typedef struct TSupSysInfoIDSBaseItem_
{
    size_t block;
    int base;
    size_t quant;
} TSupSysInfoIDSBaseItem;

typedef TSupIcon TSupSysIcon;

typedef struct TSupSysInfoIconItem_
{
    size_t size_of;
    TSupSysIcon icon;
} TSupSysInfoIconItem;

typedef struct TSupSysInfoIDSBase_
{
    size_t size_of;
    size_t blocks;
    const TSupSysInfoIDSBaseItem *items;
} TSupSysInfoIDSBase;

typedef struct TSupSysInfoDetailName_
{
    size_t size_of;
    TSupSysInfoText name;
} TSupSysInfoDetailName;

typedef struct TSupSysInfoProducer_
{
    size_t size_of;
    TSupSysInfoText producer;
} TSupSysInfoProducer;

typedef struct TSupSysInfoProducerVersion_
{
    size_t size_of;
    TSupSysInfoText version;
} TSupSysInfoProducerVersion;

typedef struct TSupSysInfoConnectNext_
{
    size_t size_of;
    const TSupSysEContext *context;
    TSupSysInfoText name;
    TSupSysConnectNum num;
    TSupSysInfoUntyped add_info;
} TSupSysInfoConnectNext;

typedef struct TSupSysInfoConnectNext_ TSupSysInfoConnectOpen;

typedef struct TSupSysInfoConnectNext_ TSupSysInfoConnectClose;

typedef struct TSupSysInfoConnect_
{
    size_t size_of;
    TSupSysInfoText name;
    TSupSysConnectNum num;
} TSupSysInfoConnect;

typedef struct TSupSysInfoDummy_ TSupSysInfoDisconnect;

typedef struct TSupSysInfoConnect_ TSupSysInfoConnectValid;

typedef struct TSupSysInfoConnect_ TSupSysInfoConnectRemove;

typedef struct TSupSysInfoCompany_
{
    size_t size_of;
    TSupSysInfoText company;
} TSupSysInfoCompany;

typedef struct TSupSysInfoFlags_
{
    size_t size_of;
    size_t type;
    TSupSysInfoBitString string;
} TSupSysInfoFlags;

typedef TSupErr ( *TSupSysFunction )( TSupSysContext *context, 
    TSupSysInfo *info );

typedef struct TSupSysInfoCall_
{
    size_t size_of;
    TSupSysInfoFun num;
    TSupSysFunction fun;
    TSupSysInfo *info;
} TSupSysInfoCall;

typedef struct TSupSysInfoGroupEnumOpen_
{
    size_t size_of;
    const TSupSysEContext *context;
    TSupSysInfoText name;
    TSupSysInfoUntyped add_info;
} TSupSysInfoGroupEnumOpen;

typedef struct TSupSysInfoGroupEnumOpen_ TSupSysInfoGroupEnumNext;

typedef struct TSupSysInfoGroupEnumOpen_ TSupSysInfoGroupEnumClose;

typedef TSupSysInfoNickname TSupSysInfoGroupNickname;

typedef struct TSupSysInfoRegister_
{
    size_t size_of;
    TSupSysContext *context;
    TSupSysInfoText in_group_name;
    TSupSysInfoText path_to_item;
} TSupSysInfoRegister;

typedef struct TSupSysInfoUnregister_
{
    size_t size_of;
    int unload;
} TSupSysInfoUnregister;

typedef struct TSupSysFunctionTableItem_
{
    TSupSysInfoFun num;
    TSupSysFunction function;
} TSupSysFunctionTableItem;

typedef struct TSupSysFunctionTable_
{
    size_t size_of;
    size_t quant;
    const TSupSysFunctionTableItem *table;
} TSupSysFunctionTable;

typedef const TSupSysFunctionTable* (*TSupSysExportFunction)( void );

#endif /* !defined( _SUPPORT_SUP_SYS_H ) */
/* end of file: $Id: sup_sys.h 76279 2012-03-28 08:25:07Z sagafyin $ */

