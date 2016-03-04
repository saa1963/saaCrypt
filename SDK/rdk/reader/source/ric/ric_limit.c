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

TSupErr ric_file_chsize( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoChSize *inf = (TReaderInfoChSize*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoChSize );

    if( ctx->file_length < inf->length )
    {
	inf->length = ctx->file_length;
	return RDR_ERR_FULL;
    }
    inf->length = ctx->file_length;
    inf->size_of = sizeof( TReaderInfoChSize );
    return SUP_ERR_NO;
}

TSupErr ric_file_size( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoLength *inf = (TReaderInfoLength*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoLength );

    inf->length = ctx->file_length;
    inf->size_of = sizeof( TReaderInfoLength );
    return SUP_ERR_NO;
}

/* end of file: $Id: ric_limit.c 5933 2002-03-25 19:04:06Z cav $ */
