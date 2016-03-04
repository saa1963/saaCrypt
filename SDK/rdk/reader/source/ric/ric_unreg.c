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

TSupErr ric_unregister( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoUnregister *inf = (TSupSysInfoUnregister*)info;
    SUPSYS_PRE_INFO( info, TSupSysInfoUnregister );

    if( inf->unload )
	support_unload_library();
    if( context == NULL )
	return SUP_ERR_NO;
    free( context );
    return SUP_ERR_NO;
}
/* end of file: $Id: ric_unreg.c 68807 2011-06-27 07:16:13Z dim $ */
