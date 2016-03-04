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
 * \version $Revision: 59786 $
 * \date $Date: 2010-01-28 20:49:24 +0400 (Thu, 28 Jan 2010) $
 * \author $Author: borodin $
 * \brief Связующий интерфейс от считывателя и смарт-карты к READER.
 */

#if !defined( READER_SCARD_SCARD_H )
#define READER_SCARD_SCARD_H

#include "reader.kit/reader.h"

#define RSCARD_PROTOCOL_T0 0x1
#define RSCARD_PROTOCOL_T1 0x2

#define RSCARD_FUN_IS_SAME_ATR 0x2500
#define RSCARD_FUN_PRECONNECT_CARRIER 0x2501
#define RSCARD_FUN_POSTCONNECT_CARRIER 0x2502

#define RSCARD_FUN_CONNECT_CARRIER READER_FUN_CONNECT_CARRIER
#define RSCARD_FUN_SAME_MEDIA READER_FUN_SAME_MEDIA
#define RSCARD_FUN_RESTORE_CONTEXT READER_FUN_RESTORE_CONTEXT

#define RSCARD_DISPOSITION_LEAVE 0
#define RSCARD_DISPOSITION_RESET 1
#define RSCARD_DISPOSITION_UNPOWER 2
#define RSCARD_DISPOSITION_EJECT 3
#define RSCARD_DISPOSITION_CONNECT 0x80

typedef struct TSCardInfoAPDU_
{
    unsigned char cla;
    unsigned char ins;
    unsigned char p1;
    unsigned char p2;
    TSupSysInfoUntyped input;
    TSupSysInfoUntyped output;
    unsigned char sw1;
    unsigned char sw2;
} TSCardInfoAPDU;

typedef struct TSCardInfoBerTLVSearch_
{
    unsigned char class_type;
    unsigned char class_tag;
    unsigned char simple_tag;
    TSupSysInfoUntyped input;
    TSupSysInfoUntyped output;
} TSCardInfoBerTLVSearch;

typedef struct TSCardInfoSimpleTLVSearch_
{
    unsigned char simple_tag;
    TSupSysInfoUntyped input;
    TSupSysInfoUntyped output;
} TSCardInfoSimpleTLVSearch;

typedef TSupErr (*TSCardAPDUCmd)( TSupSysEContext *ctx, TSCardInfoAPDU *);

typedef TSupErr (*TSCardBerTLVSearch)( TSCardInfoBerTLVSearch *);

typedef TSupErr (*TSCardSimpleTLVSearch)( TSCardInfoSimpleTLVSearch *);

typedef unsigned TDisposition;

typedef struct TSCardInfoConnectCarrier_
{
    size_t size_of;
    unsigned protocol;
    TSupSysInfoUntyped atr;
    TSupSysEContext *reader;
    TSCardAPDUCmd apdu_cmd;
    TSCardBerTLVSearch ber_tlv_search;
    TSCardSimpleTLVSearch simple_tlv_search;
    TDisposition begin_transaction_disposition;
    TDisposition end_transaction_disposition;
    TDisposition disconnect_disposition;
    TDisposition connect_disposition;
    TDisposition reconnect_disposition;
    TCHAR *connect;
} TSCardInfoConnectCarrier;

typedef struct TSCardInfoPreconnectCarrier_
{
    size_t size_of;
    unsigned protocol;
    TSupSysInfoUntyped atr;
    TSupSysEContext *reader;
    TSCardBerTLVSearch ber_tlv_search;
    TSCardSimpleTLVSearch simple_tlv_search;
    TDisposition begin_transaction_disposition;
    TDisposition end_transaction_disposition;
    TDisposition disconnect_disposition;
    TDisposition connect_disposition;
    TDisposition reconnect_disposition;
    TCHAR *connect;
} TSCardInfoPreconnectCarrier;

typedef struct TSCardInfoPostconnectCarrier_
{
    size_t size_of;
    TSCardAPDUCmd apdu_cmd;
} TSCardInfoPostconnectCarrier;


typedef struct TSCardInfoIsSame_
{
    size_t size_of;
    unsigned protocol;
    TSupSysInfoUntyped atr;
} TSCardInfoIsSame;

#endif /* READER_SCARD_SCARD_H */
