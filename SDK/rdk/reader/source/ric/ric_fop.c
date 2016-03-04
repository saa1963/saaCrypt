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

#define GET_X_DIGIT(x) ((x)>='0'&&(x)<='9'?(x)-'0':((x)>='a'&&(x)<='f'?\
    (x)-'a'+10:(x)-'A'+10))

TSupErr ric_folder_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupErr code;
    TReaderInfoFolderOpen *inf = (TReaderInfoFolderOpen*)info;
    TRICContext *ctx = (TRICContext*)context;
    size_t length;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderOpen );

    if( inf->name.length )
    {
	if( inf->name.length != 4
	    || !_istxdigit( inf->name.text[0] )
	    || !_istxdigit( inf->name.text[1] )
	    || !_istxdigit( inf->name.text[2] )
	    || !_istxdigit( inf->name.text[3] ) )
	{
	    return SUP_ERR_PARAM;
	}
	ctx->application = (unsigned short) ( GET_X_DIGIT( inf->name.text[0] ) << 12
	    | GET_X_DIGIT( inf->name.text[1] ) << 8
	    | GET_X_DIGIT( inf->name.text[2] ) << 4
	    | GET_X_DIGIT( inf->name.text[3] ) );
    }
    else
	ctx->application = RIC_APPLICATION;
    code = ric_select_file( ctx, 1, &length );
    if( code )
	return code;
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_fop.c 10393 2003-06-06 10:58:56Z chudov $ */
