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

/*!
 * \file $RCSfile$
 * \version $Revision: 37543 $
 * \date $Date: 2006-11-27 20:07:59 +0400 (Mon, 27 Nov 2006) $
 * \author $Author: chudov $
 * \brief Функция получения ресурса.
 */

#include "ric_prj.h"

/*! 
 * \ingroup ric_funs
 * \brief Функция получения ресурса.
 *
 * Функция при сборке Windows DLL возвращает DLL instance, в остальных
 *  случаях возвращается заменитель ресурса.
 * \param context [in] контекст РИК. Не используется.
 * \param info [in/out] структура #TSupSysInfoInstance
 * \sa #TSupSysInfoInstance, READER_FUN_INFO_INSTANCE, 
 *  TSupSysFunction
 */
TSupErr ric_info_instance(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoInstance *inf = (TSupSysInfoInstance*)info;

    SUPSYS_PRE_INFO( info, TSupSysInfoInstance );
    UNUSED( context );
    inf->size_of = sizeof( TSupSysInfoInstance );
    inf->instance = RIC_RESOURCE;
    return SUP_ERR_NO;
}

static unsigned char RIC_FLAGS_CONNECT[] = 
{
    0 << 7 /* SUPSYS_FLAG_CONNECT_MANY */
    | 0 << 6 /* SUPSYS_FLAG_CONNECT_COM */
    | 0 << 5 /* SUPSYS_FLAG_CONNECT_LPT */
    | 0 << 4 /* SUPSYS_FLAG_CONNECT_AUTO */
    | 1 << 3 /* SUPSYS_FLAG_CONNECT_MULTY */
    | 0 << 2 /* SUPSYS_FLAG_CONNECT_PNPGROUP */
};

TSupErr ric_system_flags(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoFlags *flags = (TSupSysInfoFlags*)info;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoFlags );
    if( flags->type != SUPSYS_FLAG_BLOCK_CONNECT )
	return SUP_ERR_UNSUPPORTED;
    if( flags->string.bits > SUPSYS_FLAG_CONNECT_VERSION_3 )
	flags->string.bits = SUPSYS_FLAG_CONNECT_VERSION_3;
    memcpy( flags->string.string, RIC_FLAGS_CONNECT, 
	flags->string.bits / 8 + ( flags->string.bits % 8 ? 1 : 0 ) );
    flags->string.bits = SUPSYS_FLAG_CONNECT_VERSION_3;
    return SUP_ERR_NO;
}

/* end of file: $Id: ric_inst.c 37543 2006-11-27 16:07:59Z chudov $ */
