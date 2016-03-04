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
 * \version $Revision: 70756 $
 * \date $Date: 2011-09-09 16:43:32 +0400 (Fri, 09 Sep 2011) $
 * \author $Author: maxdm $
 * \brief Функция проверки вставленного носителя контексту
 */

#include "ric_prj.h"

/*! 
 * \ingroup ric_fun
 * \brief Функция проверки вставленного носителя контексту
 */
TSupErr ric_same_media(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoSameMedia *inf = (TReaderInfoSameMedia*)info;
    TRICContext *ctx = (TRICContext*)context;
    TSupErr code;
    unsigned char unique[4];
    TCHAR str_unique[MAX_UNIQUE_LENGTH + 1];
    TCHAR *buffer;

    SUPSYS_PRE_INFO( info, TReaderInfoSameMedia );
    SUPSYS_PRE_CCONTEXT( ctx, TRICContext );

    code = ric_unique_num( ctx, unique );
    if( code )
	return code;
    if( !memcmp( ctx->unique, unique, 4 ) )
	return SUP_ERR_NO;
    inf->size_of = sizeof( TReaderInfoSameMedia );
    if( inf->unique.text == NULL || inf->unique.length == 0 )
    {
	inf->unique.length = ctx->unique_length;
	return RDR_ERR_ANOTHER_MEDIA;
    }
    ctx->unique_form( unique, str_unique );
    buffer = inf->unique.text;
    _tcsncpy( buffer, str_unique, inf->unique.length );
    buffer[inf->unique.length] = 0;
    return RDR_ERR_ANOTHER_MEDIA;
}
/* end of file: $Id: ric_same.c 70756 2011-09-09 12:43:32Z maxdm $ */
