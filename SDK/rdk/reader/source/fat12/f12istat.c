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

/****
 * $Id: f12istat.c 75486 2012-03-04 12:45:49Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции поддержки информации с ключевой дискеты.
 * Номера поддерживаемых info-функций.
 *
 * Константа: FAT12_INFO_SUPPORT
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/
#include "reader.kit/rdr_ver.h"

/*+ Номера поддерживаемых info-функций. +*/
static const TSupSysFunctionTableItem fat12_funs[] =
{
    { SUPSYS_FUN_NICKNAME, fat12_info_nickname },
    { SUPSYS_FUN_NAME, fat12_info_name },
    { SUPSYS_FUN_INSTANCE, fat12_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, fat12_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, fat12_context_free },
    { SUPSYS_FUN_FLAGS, fat12_info_system_flag },
#ifndef UNIX
    { SUPSYS_FUN_CALL, fat12_aux_call },
#endif
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
    { SUPSYS_FUN_REGISTER, fat12_register },
    { SUPSYS_FUN_UNREGISTER, fat12_unregister },
    { SUPSYS_FUN_CONNECT, fat12_connect },
    { SUPSYS_FUN_DISCONNECT, fat12_disconnect },
    { READER_FUN_SAME_MEDIA, fat12_info_same_media },
#ifdef UNIX
    { READER_FUN_LOCK, fat12_lock },
    { READER_FUN_UNLOCK, fat12_unlock },
#endif /* UNIX */
    { READER_FUN_CONNECT_CARRIER, fat12_connect_carrier },
    { READER_FUN_DISCONNECT_CARRIER, fat12_disconnect_carrier },
    { READER_FUN_RESTORE_CONTEXT, fat12_restore_context },
#ifndef UNIX
    { READER_FUN_UNIQUE_GET, fat12_unique_get },
#ifndef WINCE
    { READER_FUN_UNIQUE_SET, fat12_unique_set },
#endif // WINCE
#endif /* UNIX */

#ifndef _WIN32
    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_locked_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_locked_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_locked_folder_enum_close },
#else
    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_folder_enum_close },
#endif
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

static const TSupSysFunctionTableItem FAT12_GROUP_FUNS[] =
{
    { SUPSYS_FUN_INSTANCE, fat12_info_instance },
    { SUPSYS_FUN_FLAGS, fat12_group_info_system_flag },
    { SUPSYS_FUN_GROUP_ENUM_OPEN, fat12_group_enum_open },
    { SUPSYS_FUN_GROUP_ENUM_NEXT, fat12_group_enum_next },
    { SUPSYS_FUN_GROUP_ENUM_CLOSE, fat12_group_enum_close },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
};

static const TSupSysFunctionTable FAT12_GROUP_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( FAT12_GROUP_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    FAT12_GROUP_FUNS
};

static const TSupSysFunctionTable fat12_table =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( fat12_funs ) / sizeof( TSupSysFunctionTableItem ),
    fat12_funs
};

const TSupSysEHandle *fat12_reader_get_handle( void )
{
    return (const TSupSysEHandle*)&fat12_table;
}

const TSupSysEHandle *
fat12_reader_group_handle( void )
{
    return (const TSupSysEHandle*)&FAT12_GROUP_TABLE;
}
/*+ end of file: $Id: f12istat.c 75486 2012-03-04 12:45:49Z dim $ +*/
