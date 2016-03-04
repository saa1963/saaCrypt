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

TSupErr ric_restore_context( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoRestoreContext *inf = (TReaderInfoRestoreContext*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoRestoreContext );

    if( ctx->status & RIC_STATUS_PIN_VERIFIED )
    {
	code = ric_verify_pin( ctx, ctx->pin, NULL );
	if( code )
	    return code;
    }
    if( ctx->status & RIC_STATUS_FILE_SELECTED )
    {
	code = ric_select_file( ctx, ctx->file, &( ctx->file_length ) );
	if( code )
	    return code;
    }
    inf->size_of = sizeof( TReaderInfoRestoreContext );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_rest.c 4794 2001-11-23 17:27:09Z cav $ */
