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

#include "fat12prj.h"

/*
 * Функция востановления контекста после получения ошибки о смене носителя.
 */
TSupErr fat12_restore_context(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TReaderInfoRestoreContext *inf = (TReaderInfoRestoreContext*)info;

    SUPSYS_PRE_INFO( info, TReaderInfoRestoreContext );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );

    UNUSED(ctx);
    UNUSED(inf);
    inf->size_of = sizeof( TReaderInfoRestoreContext );
#ifndef UNIX
    if( ctx->handle != INVALID_HANDLE_VALUE )
    {
	BY_HANDLE_FILE_INFORMATION file_info;
	TSupErr code;
	if( GetFileInformationByHandle( ctx->handle, &file_info ) )
	    return SUP_ERR_NO;
	if( !( ctx->access & GENERIC_WRITE ) )
	{
	    code = fat12_os_error();
	    if( code != RDR_ERR_MEDIA_CHANGED )
		return code;
	}
	ctx->handle = INVALID_HANDLE_VALUE;
    }
    if( ctx->file_name )
    {
	ctx->handle = CreateFile( ctx->file_name, ctx->access, 0, NULL, 
	    OPEN_EXISTING, 0, NULL );
	if( ctx->handle == INVALID_HANDLE_VALUE )
	    return fat12_os_error();
    }
#endif /* UNIX */

    return SUP_ERR_NO;
}
/* end of file: $Id: f12trest.c 8314 2002-11-04 13:15:34Z pooh $ */
