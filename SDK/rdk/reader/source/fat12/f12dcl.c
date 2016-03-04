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
 * \version $Revision: 4395 $
 * \date $Date: 2001-10-31 18:44:33 +0400 (Wed, 31 Oct 2001) $
 * \author $Author: cav $
 * \brief Функция закрытия папки.
 */

#include "fat12prj.h"

/*!
 * \ingroup fat12_fun_reader
 * \brief Функция закрытия папки.
 * \param context [in] контекст считывателя. 
 * \param info [in/out] структура #TReaderInfoFolderClose
 * \sa TReaderInfoFolderClose, READER_FUN_FOLDER_CLOSE, 
 *  TSupSysFunction, fat12_folder_open, #rdr_folder_open, #rdr_folder_close
 */
TSupErr fat12_folder_close( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderClose );

    if( ctx->folder )
    {
	free( ctx->folder );
	ctx->folder = NULL;
    }
    return SUP_ERR_NO;
}
/* end of file: $Id: f12dcl.c 4395 2001-10-31 14:44:33Z cav $ */
