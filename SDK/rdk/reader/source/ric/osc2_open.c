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
#include "limits.h" 

TSupErr oscar2_file_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoOpen *inf = (TReaderInfoOpen*)info;
    TSupErr code;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoOpen );
    SUPSYS_PRE_READ_PTRS( inf->mode, sizeof( TReaderInfoOpenFlags ) );
    SUPSYS_PRE( inf->mode_bits >= READER_MODEFLAG_BITS );
    SUPSYS_PRE( inf->mode != 0 );
    SUPSYS_PRE( inf->num <= (int)(USHRT_MAX) && inf->num > 0 );

    code = oscar2_select_file( ctx, (unsigned char)inf->num, 
	&( ctx->file_length ) );
    if( code )
    {
	ctx->file_length = 0;
	return code;
    }
    ctx->file = (unsigned char)inf->num;
    ctx->status |= RIC_STATUS_FILE_SELECTED;
    inf->size_of = sizeof( TReaderInfoOpen );
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2_open.c 65760 2011-01-18 18:57:49Z dedal $ */
