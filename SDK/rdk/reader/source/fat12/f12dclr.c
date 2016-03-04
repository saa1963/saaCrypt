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
 * \version $Revision: 23182 $
 * \date $Date: 2005-01-24 16:39:11 +0400 (Mon, 24 Jan 2005) $
 * \author $Author: cav $
 * \brief Функция очистки папки.
 */

#include "fat12prj.h"
#ifdef UNIX
#include <unistd.h>
#endif /* UNIX */

/*!
 * \ingroup fat12_fun_reader
 * \brief Функция очистки папки.
 * \param context [in] контекст считывателя. 
 * \param info [in/out] структура #TReaderInfoFolderClear
 * \sa TReaderInfoFolderClear, READER_FUN_FOLDER_OPEN, 
 *  TSupSysFunction, fat12_folder_close, fat12_folder_open
 *  #rdr_folder_open, #rdr_folder_close, #rdr_folder_clear
 */
TSupErr fat12_folder_clear( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderClear *inf = (TReaderInfoFolderClear*)info;
    TCHAR *path;
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderClear );

    inf->size_of = sizeof( TReaderInfoFolderOpen );

    if( !ctx->folder )
	return SUP_ERR_NO;

    path = (TCHAR*)malloc( ( _tcslen( ctx->folder ) 
	+ 2 + _tcslen( ctx->path )
	+ 1 ) * sizeof( TCHAR ) );
    if( path == NULL )
	return SUP_ERR_MEMORY;
    _tcscpy( path, ctx->path );
    _tcscat( path, ctx->folder );
#ifdef UNIX
    if( _trmdir( path ) )
#else /* UNIX */
    if( !RemoveDirectory( path ) )
#endif /* UNIX */
    {
	TSupErr code = fat12_os_error();
	free( path );
	free( ctx->folder ); ctx->folder = NULL;
	return code;
    }
    free( ctx->folder ); ctx->folder = NULL;
    free( path );

    return SUP_ERR_NO;
}
/* end of file: $Id: f12dclr.c 23182 2005-01-24 12:39:11Z cav $ */
