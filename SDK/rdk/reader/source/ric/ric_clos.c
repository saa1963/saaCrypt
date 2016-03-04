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

TSupErr ric_file_close( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoClose *inf = (TReaderInfoClose*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoClose );

    ctx->status &= ~RIC_STATUS_FILE_SELECTED;
    ctx->file = 0;
    ctx->file_length = 0;
    inf->size_of = sizeof( TReaderInfoClose );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_clos.c 3583 2001-08-06 16:07:46Z cav $ */
