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
 * \version $Revision: 44146 $
 * \date $Date: 2008-01-10 18:15:59 +0400 (Thu, 10 Jan 2008) $
 * \author $Author: dim $
 * \brief Точка входа в считыватель.
 */

#include "fat12prj.h"
#include "reader.kit/rdr_ver.h"

/*!
 * \ingroup hdimage_internal
 * \brief Массив интерфейсных функций считывателя с номерами.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #hdimage_funs
 */
static const TSupSysFunctionTableItem hdimage_funs[] =
{
    { SUPSYS_FUN_INSTANCE, fat12_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, fat12_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, fat12_context_free },
    { SUPSYS_FUN_IDS_BLOCK, hdimage_info_ids },
    { SUPSYS_FUN_FLAGS, hdimage_info_system_flag },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},

    { SUPSYS_FUN_REGISTER, hdimage_register },
    { SUPSYS_FUN_UNREGISTER, fat12_unregister },

    { READER_FUN_LOCAL_MACHINE, hdimage_local_machine },

    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_folder_enum_close },
    { READER_FUN_FOLDER_OPEN, fat12_folder_open },
    { READER_FUN_FOLDER_CLOSE, fat12_folder_close },

    { READER_FUN_FOLDER_CLEAR, fat12_folder_clear },
    { READER_FUN_OPEN, fat12_open },
    { READER_FUN_CLOSE, fat12_close },
    { READER_FUN_UNLINK, fat12_unlink },

    { READER_FUN_READ, fat12_read },
    { READER_FUN_WRITE, fat12_write },
    { READER_FUN_CHSIZE, fat12_chsize },
    { READER_FUN_LENGTH, fat12_length },
};

/*!
 * \ingroup hdimage_internal
 * \brief Точка входа в считыватель.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #hdimage_funs
 */
static const TSupSysFunctionTable hdimage_table =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( hdimage_funs ) / sizeof( TSupSysFunctionTableItem ),
    hdimage_funs
};

/*!
 * \ingroup hdimage_external
 * \brief Функция возвращающая точку входа в считыватель.
 * \sa #rdr_register, #rdr_register_all, #TSupSysExportFunction
 * \return Указатель на точку входа в считыватель.
 * \internal возвращается указатель на #hdimage_table
 */
const TSupSysEHandle *
    hdimage_reader_get_handle(
    void )
{
    return (const TSupSysEHandle*)&hdimage_table;
}

/* end of file: $Id: hdis.c 44146 2008-01-10 14:15:59Z dim $ */
