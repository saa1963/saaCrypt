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
 * \brief Функция закрытия контекста поиска следующей папки.
 */

#include "fat12prj.h"

/*! 
 * \ingroup fat12_fun_reader
 * \brief Функция закрытия контекста поиска следующей папки.
 *
 * Уничтожение контекста перечисления папок выделенного 
 *  функцией #fat12_info_locked_folder_enum_open
 * \param context [in] контекст считывателя. Не используется.
 * \param info [in] структура #TReaderInfoFolderEnumClose
 * \sa #TReaderInfoFolderEnumClose, #READER_FUN_FOLDER_ENUM_CLOSE, 
 *  #TSupSysFunction, #fat12_info_folder_enum_open, #fat12_info_folder_enum_next,
 * \return Код ошибки
 */
TSupErr fat12_info_locked_folder_enum_close(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumClose *inf = (TReaderInfoFolderEnumClose*)info;
    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumClose );
    UNUSED(context);

    if( ( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
	return fat12_info_folder_enum_close (context, info);
    if( inf->add_info.length == 0 || inf->add_info.info == NULL )
	return SUP_ERR_NO;
    free (inf->add_info.info);
    inf->add_info.length = 0;
    inf->add_info.info = NULL;
    return SUP_ERR_NO;
}
/* end of file: $Id: f12dscl.c 12092 2003-10-01 16:04:37Z chudov $ */
