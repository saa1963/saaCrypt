/*
 * Copyright(C) 2007 Проект ИОК
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
 * \version $Revision: 42884 $
 * \date $Date: 2007-10-17 12:51:28 +0400 (Wed, 17 Oct 2007) $
 * \author $Author: raa $
 * \brief Функция получения полного имени считывателя.
 */

#include "fat12prj.h"

TSupErr fat12_info_name( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoItemName *inf = (TSupSysInfoItemName*)info;
    TFat12Context *ctx = (TFat12Context*)context;
#ifndef UNDER_CE
    TSupErr code;
    size_t res_len;
#endif
    SUPSYS_PRE_INFO( info, TSupSysInfoItemName );
    SUPSYS_PRE_CCONTEXT( context, TFat12Context );
    if( inf->name.length )
	SUPSYS_PRE_WRITE_PTRS( inf->name.text,
	( inf->name.length + 1 ) * sizeof( TCHAR ) );

#ifdef UNDER_CE
    if( inf->name.length )
    {
	// TODO: improve
	_tcsncpy( inf->name.text, ctx->nickname + 6, inf->name.length );
	inf->name.text[inf->name.length] = 0;
    }
    inf->name.length = _tcslen( ctx->nickname );
#else /* UNDER_CE */
    res_len = inf->name.length;
    code = support_resource_string(
	FAT12_RESOURCE, IDS_FAT12_NAME, inf->name.text, &res_len );
    if( code )
	return code;

    if( inf->name.length )
    {
	if( inf->name.length > res_len )
	    inf->name.text[res_len] = _TEXT(' ');
	if( inf->name.length > res_len + 1)
 	    _tcsncpy( inf->name.text + res_len + 1, ctx->connect, inf->name.length - res_len - 1);
	inf->name.text[inf->name.length] = 0;
    }
    inf->name.length = res_len + 1 + _tcslen(ctx->connect);
#endif /* UNDER_CE */

    inf->size_of = sizeof( TSupSysInfoNickname );
    return SUP_ERR_NO;
}
