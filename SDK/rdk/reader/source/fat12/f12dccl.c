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
 * \version $Revision: 12081 $
 * \date $Date: 2003-10-01 15:27:51 +0400 (Wed, 01 Oct 2003) $
 * \author $Author: chudov $
 * \brief Функция закрытия контекста поиска следующей папки.
 */

#include "fat12prj.h"

/*! 
 * \ingroup fat12_fun_reader
 * \brief Функция закрытия контекста поиска следующей папки.
 *
 * Уничтожение контекста перечисления папок выделенного 
 *  функцией #fat12_info_folder_enum_open
 * \param context [in] контекст считывателя. Не используется.
 * \param info [in] структура #TReaderInfoFolderEnumClose
 * \sa #TReaderInfoFolderEnumClose, #READER_FUN_FOLDER_ENUM_CLOSE, 
 *  #TSupSysFunction, #fat12_info_folder_enum_open, #fat12_info_folder_enum_next,
 * \return Код ошибки
 */
TSupErr fat12_info_folder_enum_close(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumClose *inf = (TReaderInfoFolderEnumClose*)info;
    TFat12FolderEnumNew *folder_enum;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumClose );

    UNUSED(context);

    inf->size_of = sizeof( TReaderInfoFolderEnumClose );
    if( inf->add_info.length == 0 || inf->add_info.info == NULL )
	return SUP_ERR_NO;
    if( !( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
    {
	support_closedir((TSupFileEnum*)inf->add_info.info);
	inf->add_info.length = 0;
	inf->add_info.info = NULL;
	return SUP_ERR_NO;
    }
    folder_enum = (TFat12FolderEnumNew*)inf->add_info.info;
    if( folder_enum->full_prefix ) free( folder_enum->full_prefix );
    free( folder_enum );
    inf->add_info.length = 0;
    inf->add_info.info = NULL;
    return SUP_ERR_NO;
}
/* end of file: $Id: f12dccl.c 12081 2003-10-01 11:27:51Z chudov $ */
