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

/*!
 * \ingroup fat12_interface
 * \brief Разрыв связи с дисководом.
 * \return \link fat12_error Код ошибки\endlink
 */
TSupErr fat12_disconnect( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TSupSysInfoDisconnect *inf = (TSupSysInfoDisconnect*)info;

    SUPSYS_PRE_INFO( info, TSupSysInfoDisconnect );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );

    inf->size_of = sizeof( TSupSysInfoDisconnect );
    if( ctx->path == NULL )
	return SUP_ERR_NO;
    free( ctx->path ); ctx->path = NULL;

    return SUP_ERR_NO;
}
