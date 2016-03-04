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
 * \version $Revision: 70756 $
 * \date $Date: 2011-09-09 16:43:32 +0400 (Fri, 09 Sep 2011) $
 * \author $Author: maxdm $
 * \brief Функция соединения с носителем.
 */

#include "ric_prj.h"


/*! 
 * \ingroup ric_fun
 * \brief Функция соединения с носителем.
 */
TSupErr ric_preconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSCardInfoPreconnectCarrier *inf = (TSCardInfoPreconnectCarrier*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_INFO( info, TSCardInfoPreconnectCarrier );
    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );
    
    ctx->reader = inf->reader;
    ctx->ber_tlv_search = inf->ber_tlv_search;
    ctx->simple_tlv_search = inf->simple_tlv_search;
    if(inf->connect)
    {
	ctx->connect = malloc((_tcslen(inf->connect) + 1) * sizeof(TCHAR));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
	_tcscpy(ctx->connect, inf->connect);
    }
	else
    {
	ctx->connect = malloc((_tcslen(_TEXT("Default")) + 1) * sizeof(TCHAR));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
	_tcscpy(ctx->connect, _TEXT("Default"));
	
    }
    inf->size_of = sizeof( TSCardInfoPreconnectCarrier );
    inf->begin_transaction_disposition = RSCARD_DISPOSITION_RESET;
    inf->end_transaction_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->connect_disposition = RSCARD_DISPOSITION_RESET;
    inf->disconnect_disposition = RSCARD_DISPOSITION_RESET;
    inf->reconnect_disposition = RSCARD_DISPOSITION_RESET;
    return SUP_ERR_NO;
}


/*! 
 * \ingroup ric_fun
 * \brief Функция соединения с носителем.
 */
TSupErr ric_postconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSCardInfoPostconnectCarrier *inf = (TSCardInfoPostconnectCarrier*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_INFO( info, TSCardInfoPostconnectCarrier );
    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );
    
    ctx->apdu = inf->apdu_cmd;
    code = ric_unique_num( ctx, ctx->unique );
    if( code )
	return code;
    inf->size_of = sizeof( TSCardInfoPostconnectCarrier );
    return SUP_ERR_NO;
}


/*! 
 * \ingroup ric_fun
 * \brief Функция соединения с носителем.
 */
TSupErr ric_connect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSCardInfoConnectCarrier *inf = (TSCardInfoConnectCarrier*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_INFO( info, TSCardInfoConnectCarrier );
    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );
    
    ctx->apdu = inf->apdu_cmd;
    ctx->reader = inf->reader;
    ctx->ber_tlv_search = inf->ber_tlv_search;
    ctx->simple_tlv_search = inf->simple_tlv_search;
    if(inf->connect)
    {
	ctx->connect = malloc((_tcslen(inf->connect) + 1) * sizeof(TCHAR));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
	_tcscpy(ctx->connect, inf->connect);
    }
	else
    {
	ctx->connect = malloc((_tcslen(_TEXT("Default")) + 1) * sizeof(TCHAR));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
	_tcscpy(ctx->connect, _TEXT("Default"));
	
    }
    code = ric_unique_num( ctx, ctx->unique );
    if( code )
	return code;
    inf->size_of = sizeof( TSCardInfoConnectCarrier );
    inf->begin_transaction_disposition = RSCARD_DISPOSITION_RESET;
    inf->end_transaction_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->connect_disposition = RSCARD_DISPOSITION_RESET;
    inf->disconnect_disposition = RSCARD_DISPOSITION_RESET;
    inf->reconnect_disposition = RSCARD_DISPOSITION_RESET;
    return SUP_ERR_NO;
}

/* end of file: $Id: ric_con.c 70756 2011-09-09 12:43:32Z maxdm $ */
