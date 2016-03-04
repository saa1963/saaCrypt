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

#include "ric_prj.h"

TSupErr trust_logout( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
#if 0
    TReaderInfoLogout *inf = (TReaderInfoLogout*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSCardInfoAPDU apdu = { 0x80, 0xEE, 0x00, 0x00 };
    TSupErr code;
#endif

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoLogout );

return SUP_ERR_NO;  //TODO: разобраться
#if 0
//    code = trust_select_application(ctx);
//code=SUP_ERR_NO;
    apdu.input.length = 0;
    apdu.input.info = NULL;
    apdu.output.length = 0;
    apdu.output.info = NULL;

    code = ctx->apdu( ctx->reader, &apdu );
    if( code == RDR_ERR_NO_CARRIER )
	return SUP_ERR_NO;
    if( code )
	return code;
    ctx->status = RIC_STATUS_START;
    inf->size_of = sizeof( TReaderInfoLogout );
    return SUP_ERR_NO;
#endif
}
/* end of file: $Id: mag_lout.c 54220 2009-05-02 02:33:10Z lse $ */
