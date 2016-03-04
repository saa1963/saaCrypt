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

/*!
 * \file $RCSfile$
 * \version $Revision: 79078 $
 * \date $Date: 2012-06-28 18:42:26 +0400 (Thu, 28 Jun 2012) $
 * \author $Author: borodin $
 * \brief Интерфейс ко всем считывателям (RDR).
 */

#if !defined( _READER_H )
#define _READER_H

#include <stdlib.h>
#include <tchar.h>

#include "reader/support.h" 
#include "reader/rdr.h" 
#include "reader.kit/sup_sys.h"
#include "reader.kit/sup_lst.h"

/* Номера функций работающих без предварительной регистрации. 0x3xx */
#define READER_FUN_PASSWD_TYPE 0x302
#define READER_FUN_PASSWD_LENGTH 0x303
#define READER_FUN_PASSWD_TERM 0x304
#define READER_FUN_CARRIER_TYPE 0x305
/* Номера функций требующих предварительной регистрации. 0x13xx */
#define READER_FUN_CANCEL_CONSEQUENCES 0x1300
#define READER_FUN_CANCEL_NOTIFY 0x1301
/* Номера функций требующих соединение со считывателем. 0x23xx */
#define READER_FUN_SAME_MEDIA 0x2300
#define READER_FUN_LOCK 0x2301
#define READER_FUN_UNLOCK 0x2302
#define READER_FUN_CONNECT_CARRIER 0x2303
#define READER_FUN_DISCONNECT_CARRIER 0x2304
#define READER_FUN_RESTORE_CONTEXT 0x2305
#define READER_FUN_LOCAL_MACHINE 0x2306
/* Номера функций требующих соединение с носителем 0x33xx */
#define READER_FUN_UNIQUE_GET 0x3300
#define READER_FUN_UNIQUE_SET 0x3301
#define READER_FUN_FOLDER_ENUM_OPEN 0x3302
#define READER_FUN_FOLDER_ENUM_NEXT 0x3303
#define READER_FUN_FOLDER_ENUM_CLOSE 0x3304
#define READER_FUN_FOLDER_OPEN 0x3305
#define READER_FUN_FOLDER_CLOSE 0x3306
#define READER_FUN_PRIME 0x3307
#define READER_FUN_PASSWD_PHRASE 0x3309
/* Номера функций доступных только в режиме выпуска. 0x43xx */
/* Номера функций требующих выбора папки. 0x53xx */
#define READER_FUN_PASSWD_CLEAR 0x5300
#define READER_FUN_LOGIN 0x5301
#define READER_FUN_LOGOUT 0x5302
#define READER_FUN_PRIME_LOGIN 0x5303
#define READER_FUN_SEC_DESCR_READ 0x5307 
#define READER_FUN_HASH_INIT 0x5308
#define READER_FUN_HASH_DATA 0x5309
#define READER_FUN_HASH_GETVAL 0x530A
#define READER_FUN_HASH_DESTROY 0x530B
#define READER_FUN_PUK_ENTER 0x530C
/* Номера функций требующих логина. 0x63xx */
#define READER_FUN_FOLDER_CLEAR 0x6300
#define READER_FUN_PASSWD_CHANGE 0x6301
#define READER_FUN_OPEN 0x6302
#define READER_FUN_CLOSE 0x6303
#define READER_FUN_UNLINK 0x6304
#define READER_FUN_SEC_DESCR_WRITE 0x6307
#define READER_FUN_CLEAR_TRIES 0x6308
/* Номера функций требующих открытие файла. 0x73xx */
#define READER_FUN_READ 0x7300
#define READER_FUN_WRITE 0x7301
#define READER_FUN_CHSIZE 0x7302
#define READER_FUN_LENGTH 0x7303

#define READER_IDS_BLOCK 0x400
#define READER_IDS_BASE 0x1300

/* dim 2011-июнь-17: значение 0 было, больше не используется */
#define READER_IDS_PASSWD_TERM 1
#define READER_IDS_CARRIER_TYPE 2
#define READER_IDS_QUANT_V1 ( READER_IDS_CARRIER_TYPE + 1 )

#define READER_PASSWD_DEF_SIZE 0x20
#define READER_PASSWD_DEF_PUBLIC_SIZE 0xff

#define READER_CONNECT_CARRIER_FLAG_FORMAT 0x1

#define READER_MODEFLAG_BITS 8

typedef struct TReaderInfoOpenFlags_
{
    unsigned o_read : 1;
    unsigned o_write : 1;
    unsigned o_trunc : 1;
    unsigned o_creat : 1;
    unsigned o_unauthenticated : 1;
    unsigned o_private : 1;
} TReaderInfoOpenFlags;

/* Номера функций работающих без предварительной регистрации. 0x3xx */
typedef struct TReaderInfoPasswdType_
{
    size_t size_of;
    TRdrLoginInfoType type;
} TReaderInfoPasswdType;

typedef struct TReaderInfoPasswdInfo_
{
    size_t size_of;
    TRdrLoginInfoType type;
    size_t max_length;
    size_t min_length;
} TReaderInfoPasswdInfo;

typedef struct TReaderInfoPasswdName_
{
    size_t size_of;
    TSupSysInfoText term;
} TReaderInfoPasswdName;

typedef struct TReaderInfoPasswdPhrase_
{
    size_t size_of;
    TSupSysInfoText phrase;
} TReaderInfoPasswdPhrase;

typedef struct TReaderInfoCarrierType_
{
    size_t size_of;
    TSupSysENickname type;
    const TCHAR * path_to_item;
} TReaderInfoCarrierType;

/* Номера функций требующих предварительной регистрации. 0x13xx */
typedef struct TSupSysInfoDummy_ TReaderInfoCancelConsequences;

typedef struct TSupSysInfoDummy_ TReaderInfoCancelNotify;

/* Номера функций требующих соединение со считывателем. 0x23xx */
typedef struct TReaderInfoSameMedia_
{
    size_t size_of;
    TSupSysInfoText unique;
    TSupSysEList *media_list;
    unsigned flags;
    TSupSysFindInfo *rdr_info;
    TSupSysFindValidFun valid_fun;
} TReaderInfoSameMedia;

typedef struct TSupSysInfoDummy_ TReaderInfoLock;

typedef struct TSupSysInfoDummy_ TReaderInfoUnlock;

typedef struct TReaderInfoConnectCarrier_
{
    size_t size_of;
    unsigned flags;
    TSupSysEList *media_list;
    TSupSysFindInfo *rdr_info;
    TSupSysFindValidFun valid_fun;
} TReaderInfoConnectCarrier;

typedef struct TSupSysInfoDummy_ TReaderInfoDisconnectCarrier;

typedef struct TSupSysInfoDummy_ TReaderInfoRestoreContext;

typedef struct TReaderInfoLocalMachine_
{
    size_t size_of;
    int flag;
} TReaderInfoLocalMachine;

/* Номера функций требующих соединение с носителем 0x33xx */
typedef struct TReaderInfoUniqueGet_
{
    size_t size_of;
    TSupSysInfoText unique;
} TReaderInfoUniqueGet;

typedef struct TReaderInfoUniqueSet_
{
    size_t size_of;
    TSupSysInfoText unique;
} TReaderInfoUniqueSet;

typedef struct TReaderInfoFolderEnumNext_
{
    size_t size_of;
    const TSupSysEContext *context;
    TSupSysInfoText name;
    TSupSysInfoUntyped add_info;
    TSupSysInfoText prototype;
    unsigned flags;
} TReaderInfoFolderEnumNext;

typedef struct TReaderInfoFolderEnumNext_ TReaderInfoFolderEnumOpen;

typedef struct TReaderInfoFolderEnumNext_ TReaderInfoFolderEnumClose;

typedef struct TReaderInfoFolderOpen_
{
    size_t size_of;
    TSupSysInfoText name;
    size_t mode_bits;
    TReaderInfoOpenFlags *mode;
} TReaderInfoFolderOpen;

typedef TSupSysInfoDummy TReaderInfoFolderClose;

typedef struct TReaderInfoPrime_
{
    size_t size_of;
    TSupSysInfoText passwd;
    int retries;
} TReaderInfoPrime;

/* Номера функций требующих выбора папки. 0x53xx */
typedef struct TReaderInfoPasswdClear_
{
    size_t size_of;
    TSupSysInfoText passwd;
} TReaderInfoPasswdClear;

typedef struct TReaderInfoLogin_
{
    size_t size_of;
    TSupSysInfoText passwd;
    int retries;
} TReaderInfoLogin;

typedef struct TReaderInfoClearTries_
{
    size_t size_of;
    TSupSysInfoText passwd;
    int number;
} TReaderInfoClearTries;

typedef unsigned long TReaderInfoSecDescrInfo; 
typedef TSupSysInfoUntyped TReaderInfoSecDescrDescriptor; 
typedef TSupSysInfoUntyped TReaderInfoSecDescrOutError; 

typedef struct TReaderInfoSecDescrRead_ 
{
    size_t size_of;
    TReaderInfoSecDescrInfo sec_info;
    TReaderInfoSecDescrDescriptor sec_descr; 
    TReaderInfoSecDescrOutError out_error;
} TReaderInfoSecDescrRead;

typedef struct TSupSysInfoDummy_ TReaderInfoLogout;

typedef struct TSupSysInfoDummy_ TReaderInfoPrimeLogin;

/* Номера функций требующих логина. 0x63xx */
typedef TSupSysInfoDummy TReaderInfoFolderClear;

typedef struct TReaderInfoPasswdChange_
{
    size_t size_of;
    TSupSysInfoText new_passwd;
} TReaderInfoPasswdChange;

typedef struct TReaderInfoOpen_
{
    size_t size_of;
    size_t mode_bits;
    TReaderInfoOpenFlags *mode;
    TSupSysInfoText name;
    TRdrFileNumber num;
} TReaderInfoOpen;

typedef TSupSysInfoDummy TReaderInfoClose;

typedef struct TReaderInfoUnlink_
{
    size_t size_of;
    TSupSysInfoText name;
    TRdrFileNumber num;
} TReaderInfoUnlink;

typedef struct TReaderInfoSecDescrWrite_ 
{
    size_t size_of;
    TReaderInfoSecDescrInfo sec_info; 
    TReaderInfoSecDescrDescriptor sec_descr;
    TReaderInfoSecDescrOutError out_error;
} TReaderInfoSecDescrWrite; 

/* Номера функций требующих открытие файла. 0x73xx */
typedef struct TReaderInfoRead_
{
    size_t size_of;
    size_t from;
    TSupSysInfoUntyped info;
} TReaderInfoRead;

typedef struct TReaderInfoWrite_
{
    size_t size_of;
    size_t from;
    TSupSysInfoUntyped info;
} TReaderInfoWrite;

typedef struct TReaderInfoChSize_
{
    size_t size_of;
    size_t length;
} TReaderInfoChSize;

typedef struct TReaderInfoLength_
{
    size_t size_of;
    size_t length;
} TReaderInfoLength;

#define HASH_VALUE_SIZE 32

/* значение хеша. в little-endian (в отличие от числа, передаваемого в функцию 
 * хеширования - оно в big-endian)*/
typedef struct TRdrHashValue_
{
    unsigned char v[HASH_VALUE_SIZE];	
} TRdrHashValue;

typedef struct TReaderFkcHashInit_
{
    size_t size_of;		    //!< Содержит размер структуры TReaderFkcHashInit_
    unsigned int hash_algid;	    //!< ALGID хеша
    TSupSysEContext * handle;	    //!< Здесь будет возвращен хендл хеша
} TReaderFkcHashInit;

typedef struct TReaderFkcHashData_
{
    size_t size_of;		    //!< Содержит размер структуры TReaderFkcHashData_
    TSupSysEContext * handle;	    //!< Здесь передается контекст хеша
    TSupSysInfoUntyped info;	    //!< данные и их длина
    size_t hashed_len;		    //!< возвращаемая длина захешированных данных
} TReaderFkcHashData;

typedef struct TReaderFkcHashGetVal_
{
    size_t size_of;		    //!< Содержит размер структуры TReaderFkcHashGetVal
    TSupSysEContext * handle;	    //!< Здесь передается контекст хеша
    TRdrHashValue hashval;	    //!< возвращаемое значение хеша
} TReaderFkcHashGetVal;

typedef struct TReaderFkcHashDestroy_
{
    size_t size_of;		    //!< Содержит размер структуры TReaderFkcHashDestroy
    TSupSysEContext * handle;	    //!< Здесь передается контекст хеша
} TReaderFkcHashDestroy;

#endif /* !defined( _READER_H ) */
/* end of file: $Id: reader.h 79078 2012-06-28 14:42:26Z borodin $ */
