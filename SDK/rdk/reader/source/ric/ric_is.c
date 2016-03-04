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
 * \version $Revision: 56676 $
 * \date $Date: 2009-09-03 14:35:31 +0400 (Thu, 03 Sep 2009) $
 * \author $Author: borodin $
 * \brief Определение принадлежности карты к типу РИК.
 */

#include "ric_prj.h"
#include "reader.kit/rdr_ver.h"

#if defined( _WIN32 )
static TSupErr ric_property_init( TSupSysContext *context, 
    TSupSysInfo *info );

static TSupErr ric_wizard_init( TSupSysContext *context, 
    TSupSysInfo *info );
#endif

/*!
 * \ingroup ric_internal
 * \brief Массив интерфейсных функций считывателя с номерами
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem
 */
static const TSupSysFunctionTableItem RIC_FUNS[] =
{
    { SUPSYS_FUN_INSTANCE, ric_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, ric_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, ric_context_free },
    { SUPSYS_FUN_FLAGS, ric_system_flags },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
#if defined _WIN32
    { SUPSYSW_FUN_PROPERTY_INIT, ric_property_init },
    { SUPSYSW_FUN_PROPERTY_DONE, base_property_done },
    { SUPSYSW_FUN_WIZARD_INIT, ric_wizard_init },
    { SUPSYSW_FUN_WIZARD_ATTACH, base_wizard_attach },
    { SUPSYSW_FUN_WIZARD_DONE, base_wizard_done },
#endif

    { READER_FUN_PASSWD_LENGTH, ric_passwd_length },

    { SUPSYS_FUN_REGISTER, ric_register },
    { SUPSYS_FUN_UNREGISTER, ric_unregister },

    { READER_FUN_SAME_MEDIA, ric_same_media },
    { READER_FUN_CONNECT_CARRIER, ric_connect_carrier },
    { READER_FUN_DISCONNECT_CARRIER, ric_disconnect_carrier },
#ifndef CHUDOVS_PATCH
    { READER_FUN_RESTORE_CONTEXT, ric_restore_context },
#endif

    { RSCARD_FUN_PRECONNECT_CARRIER, ric_preconnect_carrier },
    { RSCARD_FUN_POSTCONNECT_CARRIER, ric_postconnect_carrier },

    { READER_FUN_UNIQUE_GET, ric_unique_get },

    { READER_FUN_FOLDER_OPEN, ric_folder_open },

    { READER_FUN_LOGIN, ric_login },
    { READER_FUN_LOGOUT, ric_logout },

    { READER_FUN_PASSWD_CHANGE, ric_passwd_change },
    { READER_FUN_OPEN, ric_file_open },
    { READER_FUN_CLOSE, ric_file_close },

    { READER_FUN_READ, ric_file_read },
    { READER_FUN_WRITE, ric_file_write },
    { READER_FUN_CHSIZE, ric_file_chsize },
    { READER_FUN_LENGTH, ric_file_size },
};

/*!
 * \ingroup ric_internal
 * \brief Точка входа в РИК.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, 
 */
static const TSupSysFunctionTable RIC_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( RIC_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    RIC_FUNS
};

/*!
 * \ingroup ric_external
 * \brief Функция возвращающая точку входа в РИК.
 * \sa #rdr_register, #rdr_register_all, #TSupSysExportFunction
 * \return Указатель на точку входа в РИК.
 * \internal возвращается указатель на #RIC_TABLE
 */
const TSupSysEHandle *
    ric_media_get_handle( void )
{
    return (const TSupSysEHandle*)&RIC_TABLE;
}

#if defined( _WIN32 )
TSupErr ric_property_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_property_init( context, info, PF_APPLICATION, RIC_PATH_CFG, _TEXT( "RIC" ) );
}

TSupErr ric_wizard_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_wizard_init( context, info, 
	PF_APPLICATION,
	_TEXT( "000000002E630E00000000" ),
	_TEXT( "00000000FFFFFF00000000" ),
	_TEXT( "0B00" ),
	RIC_PATH_CFG,
	_TEXT( "RIC" )
	);
}
#endif
/* end of file: $Id: ric_is.c 56676 2009-09-03 10:35:31Z borodin $ */
