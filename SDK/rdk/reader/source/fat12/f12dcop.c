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
 * \version $Revision: 66107 $
 * \date $Date: 2011-02-03 13:58:04 +0400 (Thu, 03 Feb 2011) $
 * \author $Author: dim $
 * \brief Функция открытия контекста поиска следующей папки.
 */

#include "fat12prj.h" 

/*! 
 * \ingroup hdimage_fun_reader
 * \brief Функция открытия контекста поиска следующей папки.
 *
 * Функция находит первую папку, но не выдает ее.
 * \param context [in] контекст считывателя. 
 * \param info [in/out] структура #TReaderInfoFolderEnumOpen
 * \sa #TReaderInfoFolderEnumOpen, #READER_FUN_FOLDER_ENUM_OPEN, 
 *  #TSupSysFunction, #fat12_info_folder_enum_close, #fat12_info_folder_enum_next,
 * \return Код ошибки
 */
TSupErr fat12_info_folder_enum_open(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumOpen *inf = (TReaderInfoFolderEnumOpen*)info;
    TCHAR *path;
    TFat12Context *ctx = (TFat12Context*)context;
    TSupErr code;
    TFat12FolderEnumNew *folder_enum;
    size_t i;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumOpen );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_STRING_PTR( ctx->path );

    inf->size_of = sizeof( TReaderInfoFolderEnumOpen );

    if( !( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
    {
	code = support_opendir( ctx->path, &inf->name.length,
				(TSupFileEnum**)&inf->add_info.info );
	inf->add_info.length = 4;
	return code;
    }
    path = malloc( ( _tcslen( ctx->path ) + 2 + 12 + 2 ) * sizeof( TCHAR ) );
    if( path == NULL )
	return SUP_ERR_MEMORY;
    _tcscpy( path, ctx->path );
    folder_enum = malloc( sizeof( TFat12FolderEnumNew ) );
    if( folder_enum == NULL )
    {
	free( path );
	return SUP_ERR_MEMORY;
    }
    folder_enum->full_prefix = NULL;
    inf->add_info.length = sizeof( *folder_enum );
    inf->add_info.info = (unsigned char*)folder_enum;
    for( i = 0; i < 8 && inf->prototype.text[i]; i++ )
    {
	folder_enum->prefix[i] = fat12_convert( inf->prototype.text[i] );
	folder_enum->prefix[i + 1] = 0;
    }
    _tcscat( folder_enum->prefix, _TEXT( "." ) );
    folder_enum->num = 0;
    folder_enum->full_prefix = path;
    _tcscat( folder_enum->full_prefix, folder_enum->prefix );
    inf->name.length = 12;
    return SUP_ERR_NO;
}
/* end of file: $Id: f12dcop.c 66107 2011-02-03 09:58:04Z dim $ */
  
