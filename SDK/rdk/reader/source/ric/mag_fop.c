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

TSupErr trust_folder_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupErr code;
    TReaderInfoFolderOpen *inf = (TReaderInfoFolderOpen*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_CONTEXT( context, TRICContext );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderOpen );

    if( inf->name.length && inf->name.length==1 && inf->name.text[0]>='A' && inf->name.text[0]<='Z')
    {
	ctx->application=0x100+(unsigned char)((inf->name.text[0]-'A')*0x10);
    }
    else
    {
	//TODO: выдать ошибку
	ctx->application = 0x100;
    }
    code = trust_select_application( ctx );
    if( code )
	return code;
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_fop.c 54220 2009-05-02 02:33:10Z lse $ */
