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
 * \version $Revision: 12092 $
 * \date $Date: 2003-10-01 20:04:37 +0400 (Wed, 01 Oct 2003) $
 * \author $Author: chudov $
 * \brief Функция поиска следующей папки.
 */

#include "fat12prj.h"

/*! 
 * \ingroup fat12_fun_reader
 * \brief Функция поиска следующей папки.
 *
 * Функция поиска следующей директории.
 * \param context [in] контекст считывателя. Не используется.
 * \param info [in/out] структура #TReaderInfoFolderEnumNext
 * \sa #TReaderInfoFolderEnumNext, #READER_FUN_FOLDER_ENUM_NEXT,
 *  #TSupSysFunction, #fat12_info_locked_folder_enum_open, 
 *  #fat12_info_locked_folder_enum_close,
 * \return Код ошибки
 */
TSupErr fat12_info_locked_folder_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumNext *inf = (TReaderInfoFolderEnumNext*)info;
    char * info_ptr;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumNext );
//    SUPSYS_PRE_STRING_PTRS( inf->name.text, inf->name.length );
    UNUSED( context );

    if( ( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
	return fat12_info_folder_enum_next (context, info);

    inf->size_of = sizeof( TReaderInfoFolderEnumNext );
    info_ptr = (char*)inf->add_info.info;

    if (* (size_t*) info_ptr >= inf->add_info.length)
	return RDR_ERR_FILE_NOT_FOUND;
    // TODO: check, that 0 is before end;
    _tcscpy ( inf->name.text, info_ptr + * (size_t*) info_ptr );
    inf->name.length = _tcslen( inf->name.text );
    * (size_t*) info_ptr += sizeof (TCHAR) * (inf->name.length + 1);
    return SUP_ERR_NO;
}

/* end of file: $Id: f12dsnxt.c 12092 2003-10-01 16:04:37Z chudov $ */
