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
 * \brief Определение принадлежности карты к типу Оскар.
 */

#include "ric_prj.h"
#include "reader.kit/rdr_ver.h"

#if defined( _WIN32 )
static TSupErr oscar_property_init( TSupSysContext *context, 
    TSupSysInfo *info );

static TSupErr oscar_wizard_init( TSupSysContext *context, 
    TSupSysInfo *info );
#endif

/*!
 * \ingroup oscar_internal
 * \brief Массив интерфейсных функций считывателя с номерами
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem
 */
static const TSupSysFunctionTableItem OSCAR_FUNS[] =
{
    { SUPSYS_FUN_INSTANCE, ric_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, ric_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, ric_context_free },
    { SUPSYS_FUN_IDS_BLOCK, oscar_info_ids },
    { SUPSYS_FUN_FLAGS, ric_system_flags },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
#if defined _WIN32
    { SUPSYSW_FUN_PROPERTY_INIT, oscar_property_init },
    { SUPSYSW_FUN_PROPERTY_DONE, base_property_done },
    { SUPSYSW_FUN_WIZARD_INIT, oscar_wizard_init },
    { SUPSYSW_FUN_WIZARD_ATTACH, base_wizard_attach },
    { SUPSYSW_FUN_WIZARD_DONE, base_wizard_done },
#endif

    { READER_FUN_PASSWD_LENGTH, ric_passwd_length },

    { SUPSYS_FUN_REGISTER, oscar_register },
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
 * \ingroup oscar_internal
 * \brief Точка входа в ОСКАР.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, 
 */
static const TSupSysFunctionTable OSCAR_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( OSCAR_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    OSCAR_FUNS
};

/*!
 * \ingroup ric_external
 * \brief Функция возвращающая точку входа в Оскар.
 * \sa #rdr_register, #rdr_register_all, #TSupSysExportFunction
 * \return Указатель на точку входа в Оскар.
 * \internal возвращается указатель на #OSCAR_TABLE
 */
const TSupSysEHandle *
    oscar_media_get_handle( void )
{
    return (const TSupSysEHandle*)&OSCAR_TABLE;
}

#if defined( _WIN32 )
TSupErr oscar_property_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_property_init( context, info, PF_APPLICATION, OSCAR_PATH_CFG, _TEXT( "oscar" ) );
}

TSupErr oscar_wizard_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_wizard_init( context, info, 
	PF_APPLICATION,
	_TEXT( "0000000000000043525950544F5052" ),
	_TEXT( "00000000000000FFFFFFFFFFFFFFFF" ),
	_TEXT( "0B00" ),
	OSCAR_PATH_CFG,
	_TEXT( "oscar" )
	);
}
#endif
/* end of file: $Id: osc_is.c 56676 2009-09-03 10:35:31Z borodin $ */
