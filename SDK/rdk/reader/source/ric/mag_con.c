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
 * \version $Revision: 70757 $
 * \date $Date: 2011-09-09 16:48:46 +0400 (Fri, 09 Sep 2011) $
 * \author $Author: maxdm $
 * \brief Функция соединения с носителем.
 */

#include "ric_prj.h"

#ifdef DEBUG_APDU
TSCardAPDUCmd apdu_external;

/*! 
 * \ingroup trust_fun
 * \brief Внутренняя отладка.
 */
TSupErr apdu_internal(TSupSysEContext *context, TSCardInfoAPDU *apdu)
{
    TSupErr rc;
    FILE *f;
    int i;
    f=fopen("\\trust.log","a+b");
    fprintf(f,"CLA=%02X INS=%02X P1=%02X P2=%02X\n",(unsigned)apdu->cla,(unsigned)apdu->ins,(unsigned)apdu->p1,(unsigned)apdu->p2);
    if (apdu->input.length) {
	fprintf(f,"Lc=%02X\n",(unsigned)apdu->input.length);
	for (i=0;i<apdu->input.length;++i)
	    fprintf(f,"%02X",(unsigned)apdu->input.info[i]);
	fprintf(f,"\n");
    }

    rc=apdu_external(context,apdu);

    fprintf(f,"SW1=%02X SW2=%02X\n",(unsigned)apdu->sw1,(unsigned)apdu->sw2);
    if ((apdu->sw1==0x90 || apdu->sw1==0x61) && apdu->output.length) {
	fprintf(f,"La=%02X\n",(unsigned)apdu->output.length);
	for (i=0;i<apdu->output.length;++i)
	    fprintf(f,"%02X",(unsigned)apdu->output.info[i]);
	fprintf(f,"\n");
    }
    fprintf(f,"\n");
    fclose(f);
    return rc;	
}
#endif


/*! 
 * \ingroup trust_fun
 * \brief Функция соединения с носителем.
 */
TSupErr trust_preconnect_carrier(
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
	ctx->connect = _tcsdup(inf->connect);
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
    }
    else
    {
	ctx->connect = _tcsdup(_TEXT("Default"));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
    }
    inf->size_of = sizeof( TSCardInfoPreconnectCarrier );
    inf->begin_transaction_disposition = RSCARD_DISPOSITION_RESET;
    inf->end_transaction_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->connect_disposition = RSCARD_DISPOSITION_RESET;
    inf->disconnect_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->reconnect_disposition = RSCARD_DISPOSITION_RESET;
    return SUP_ERR_NO;
}

TSupErr trust_postconnect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSCardInfoPostconnectCarrier *inf = (TSCardInfoPostconnectCarrier*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_INFO( info, TSCardInfoPostconnectCarrier );
    SUPSYS_PRE_CCONTEXT( ctx,  TRICContext );
    
#   ifdef DEBUG_APDU
	ctx->apdu = apdu_internal;
	apdu_external = inf->apdu_cmd;
#   else
	ctx->apdu = inf->apdu_cmd;
#   endif
    code = trust_unique_num( ctx, ctx->unique );
    if( code )
	return code;
    inf->size_of = sizeof( TSCardInfoPostconnectCarrier );
    return SUP_ERR_NO;
}


/*! 
 * \ingroup trust_fun
 * \brief Функция соединения с носителем.
 */
TSupErr trust_connect_carrier(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSCardInfoConnectCarrier *inf = (TSCardInfoConnectCarrier*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_INFO( info, TSCardInfoConnectCarrier );
    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );
    
#   ifdef DEBUG_APDU
	ctx->apdu = apdu_internal;
	apdu_external = inf->apdu_cmd;
#   else
	ctx->apdu = inf->apdu_cmd;
#   endif
    ctx->reader = inf->reader;
    ctx->ber_tlv_search = inf->ber_tlv_search;
    ctx->simple_tlv_search = inf->simple_tlv_search;
    if(inf->connect)
    {
	ctx->connect = _tcsdup(inf->connect);
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
    }
    else
    {
	ctx->connect = _tcsdup(_TEXT("Default"));
	if(!ctx->connect)
	    return SUP_ERR_MEMORY;  
    }
    code = trust_unique_num( ctx, ctx->unique );
    if( code )
	return code;
    inf->size_of = sizeof( TSCardInfoConnectCarrier );
    inf->begin_transaction_disposition = RSCARD_DISPOSITION_RESET;
    inf->end_transaction_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->connect_disposition = RSCARD_DISPOSITION_RESET;
    inf->disconnect_disposition = RSCARD_DISPOSITION_LEAVE;
    inf->reconnect_disposition = RSCARD_DISPOSITION_RESET;
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_con.c 70757 2011-09-09 12:48:46Z maxdm $ */
