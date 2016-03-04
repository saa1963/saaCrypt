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
 * \version $Revision: 66111 $
 * \date $Date: 2011-02-03 19:03:44 +0400 (Thu, 03 Feb 2011) $
 * \author $Author: maxdm $
 * \brief Функция поиска следующей папки.
 */

#include "fat12prj.h"

static int valid_name( const TCHAR *folder, TSupFileType type );

/*! 
 * \ingroup fat12_fun_reader
 * \brief Функция поиска следующей папки.
 *
 * Функция поиска следующей директории.
 * \param context [in] контекст считывателя. Не используется.
 * \param info [in/out] структура #TReaderInfoFolderEnumNext
 * \sa #TReaderInfoFolderEnumNext, #READER_FUN_FOLDER_ENUM_NEXT,
 *  #TSupSysFunction, #fat12_info_folder_enum_open, 
 *  #fat12_info_folder_enum_close,
 * \return Код ошибки
 */
#define FMT_3X _TEXT("%03x")

TSupErr fat12_info_folder_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumNext *inf = (TReaderInfoFolderEnumNext*)info;
    TFat12FolderEnumNew *folder_enum;
    TSupFileEnum *f;
    TSupFileType type;
    TSupErr code;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumNext );
    SUPSYS_PRE_STRING_PTRS( inf->name.text, inf->name.length );
    UNUSED( context );

    inf->size_of = sizeof( TReaderInfoFolderEnumNext );
    folder_enum = (TFat12FolderEnumNew*)(inf->add_info.info);
    if( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW )
    {
	TCHAR *ptr;
	int exist = DIR_EXIST;
	ptr = folder_enum->full_prefix + _tcslen( folder_enum->full_prefix );
	for( ; folder_enum->num <= 0xfff; folder_enum->num++ )
	{
	    _sntprintf( ptr, 4, FMT_3X , (unsigned)folder_enum->num );
	    exist = fat12_is_dir_exist( folder_enum->full_prefix );
	    *ptr = 0;
	    if( exist == DIR_NOTEXIST )
		break;
	}
	if( folder_enum->num >= 0xfff )
	    return RDR_ERR_FILE_NOT_FOUND;
	_tcscpy( inf->name.text, folder_enum->prefix ); 
	_sntprintf( inf->name.text + _tcslen( inf->name.text ), 4, FMT_3X,
	    (unsigned)folder_enum->num++ );
	inf->name.length = _tcslen( inf->name.text );
	return SUP_ERR_NO;
    }
    f = (TSupFileEnum*)inf->add_info.info;
    code = support_nextent( f, inf->name.text, &type );
    while( !code && !valid_name( inf->name.text, type ) )
	code = support_nextent( f, inf->name.text, &type );
    if( code )
	return RDR_ERR_FILE_NOT_FOUND;
    inf->name.length = _tcslen( inf->name.text );
    return SUP_ERR_NO;
}

static int valid_name( const TCHAR *folder, TSupFileType type )
{
    size_t i;
    size_t j;
    if( _tcslen( folder ) > 12 )
	return 0;
    if( type != SUP_FTYPE_DIR )
	return 0;
    for( i = 0, j = 0; i < 12 && folder[i]; i++ )
    {
	if( folder[i] == '.' )
	{
	    SUPSYS_PRE( j == 0 );    
	    j++;
	    continue;
	}
	if( j )
	    j++;
	if( fat12_convert( folder[i] ) != folder[i] )
	    return 0;
    }
    if( j > 4 )
	return 0;
    return 1;
}
/* end of file: $Id: f12dcnxt.c 66111 2011-02-03 15:03:44Z maxdm $ */
