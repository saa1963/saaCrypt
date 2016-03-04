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


#include "fat12prj.h" /* Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). */

TSupErr fat12_aux_call(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoCall *inf = (TSupSysInfoCall*)info;
    TSupErr code;
#if defined _WIN32 && !defined UNDER_CE
    unsigned int last_error_mode;
#endif /* _WIN32 && !UNDER_CE */

    SUPSYS_PRE_INFO( info, TSupSysInfoCall );
    inf->size_of = sizeof( TSupSysInfoCall );
    if( inf->fun == NULL )
	return SUP_ERR_UNSUPPORTED;
#if defined _WIN32 && !defined UNDER_CE
    last_error_mode = SetErrorMode( SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS );
    if( last_error_mode & ~( SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS ) )
	SetErrorMode( last_error_mode | SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS );
#endif /* _WIN32 && !UNDER_CE */
    code = inf->fun( context, inf->info );
#if defined _WIN32 && !defined UNDER_CE
    SetErrorMode( last_error_mode );
#endif /* _WIN32 && !UNDER_CE */
    return code;
}
/* end of file: $Id: f12acall.c 75486 2012-03-04 12:45:49Z dim $ */
