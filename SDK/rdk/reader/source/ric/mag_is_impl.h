/*
 * Copyright(C) 2008 Проект ИОК
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
 * \version $Revision: 57706 $
 * \date $Date: 2009-10-16 14:20:08 +0400 (Fri, 16 Oct 2009) $
 * \author $Author: borodin $
 * \brief Шаблон определения принадлежности карты к типам Магистра 
 *        (ОС Trust).
 */

#if !defined(ATRUST_FUNS) || !defined(ATRUST_TABLE) || \
    !defined(Atrust_media_get_handle) || \
    !defined(Atrust_property_init) || \
    !defined(Atrust_wizard_init) || \
    !defined(Atrust_info_ids)
#   error "Не задан суффикс точек входа"
#endif
#if !defined(ATRUST_PATH_CFG)
#   error "Не задан путь в реестре"
#endif
#if !defined(ATRUST_ATR)
#   error "Не задан ATR"
#endif
#if !defined(ATRUST_MASK)
#   error "Не задана маска ATR"
#endif
#if !defined(ATRUST_FILE_NAMES)
#   error "Не заданы имена файлов"
#endif
#if !defined(ATRUST_IDS_ITEMS) || \
    !defined(ATRUST_IDS) || \
    !defined(AIDS_TRUST_NAME)
#   error "Не заданы имена ресурсов"
#endif

#if defined( _WIN32 )
static TSupErr Atrust_property_init(TSupSysContext *context, 
				   TSupSysInfo *info );
static TSupErr Atrust_wizard_init(
				 TSupSysContext *context, 
				 TSupSysInfo *info );

static TSupErr Atrust_connect_remove(
				  TSupSysContext *context, 
				  TSupSysInfo *info );
#endif
static TSupErr Atrust_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info );

/*!
 * \ingroup trust_internal
 * \brief Массив интерфейсных функций считывателя с номерами
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem
 */
static const TSupSysFunctionTableItem ATRUST_FUNS[] =
{
    { SUPSYS_FUN_VERSION, trust_info_version },
    { SUPSYS_FUN_COPYRIGHT, trust_info_copyright },
    { SUPSYS_FUN_INSTANCE, ric_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, trust_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, ric_context_free },
    { SUPSYS_FUN_IDS_BLOCK, Atrust_info_ids },
#if !defined _WIN32
    { SUPSYS_FUN_COMPANY, trust_info_company },
#endif
    { SUPSYS_FUN_FLAGS, ric_system_flags },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
#if defined _WIN32
    { SUPSYSW_FUN_PROPERTY_INIT, Atrust_property_init },
    { SUPSYSW_FUN_PROPERTY_DONE, base_property_done },
    { SUPSYSW_FUN_WIZARD_INIT, Atrust_wizard_init },
    { SUPSYSW_FUN_WIZARD_ATTACH, base_wizard_attach },
    { SUPSYSW_FUN_WIZARD_DONE, base_wizard_done },
#endif

    { READER_FUN_PASSWD_LENGTH, ric_passwd_length },

    { SUPSYS_FUN_REGISTER, trust_register },
    { SUPSYS_FUN_UNREGISTER, ric_unregister },

#if defined _WIN32
    { SUPSYS_FUN_CONNECT_REMOVE, Atrust_connect_remove },
#endif

    { READER_FUN_SAME_MEDIA, trust_same_media },
    { READER_FUN_CONNECT_CARRIER, trust_connect_carrier },
    { READER_FUN_DISCONNECT_CARRIER, ric_disconnect_carrier },
    { READER_FUN_RESTORE_CONTEXT, trust_restore_context },

    { RSCARD_FUN_PRECONNECT_CARRIER, trust_preconnect_carrier },
    { RSCARD_FUN_POSTCONNECT_CARRIER, trust_postconnect_carrier },

    { READER_FUN_UNIQUE_GET, trust_unique_get },

    { READER_FUN_FOLDER_ENUM_OPEN, trust_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, trust_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, trust_folder_enum_close },
    { READER_FUN_FOLDER_OPEN, trust_folder_open },

    { READER_FUN_LOGIN, trust_login },
    { READER_FUN_LOGOUT, trust_logout },

    { READER_FUN_PASSWD_CHANGE, ric_passwd_change },
    { READER_FUN_OPEN, trust_file_open },
    { READER_FUN_CLOSE, ric_file_close },

    { READER_FUN_READ, ric_file_read },
    { READER_FUN_WRITE, ric_file_write },
    { READER_FUN_CHSIZE, ric_file_chsize },
    { READER_FUN_LENGTH, ric_file_size }
};

/*!
 * \ingroup trust_internal
 * \brief Точка входа в Магистра (ОС Trust).
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, 
 */
static const TSupSysFunctionTable ATRUST_TABLE =
{
    sizeof(TSupSysFunctionTable),
    sizeof(ATRUST_FUNS)/sizeof(ATRUST_FUNS[0]),
    ATRUST_FUNS
};

/*!
 * \ingroup trust_internal
 * \brief Функция возвращающая точку входа в Магистра (ОС Trust).
 * \sa #rdr_register, #rdr_register_all, #TSupSysExportFunction
 * \return Указатель на точку входа в Магистра.
 * \internal возвращается указатель на #TRUST_TABLE
 */
const TSupSysEHandle *
    Atrust_media_get_handle( void )
{
    return (const TSupSysEHandle*)&ATRUST_TABLE;
}

#if defined( _WIN32 )
TSupErr Atrust_property_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_property_init(context, info, PF_WINLOGON, ATRUST_PATH_CFG, ATRUST_NAME);
}

TSupErr Atrust_wizard_init(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    return base_wizard_init( context, info, 
	PF_WINLOGON,
	ATRUST_ATR,
	ATRUST_MASK,
	ATRUST_FILE_NAMES,
	ATRUST_PATH_CFG,
	ATRUST_NAME
	);
}
TSupErr Atrust_connect_remove(
     TSupSysContext *context, 
     TSupSysInfo *info )
{
    TSupSysInfoConnectRemove *rm = (TSupSysInfoConnectRemove*)info;
    TCHAR *winlogon_name;

    UNUSED( context );
    if( !rm ) return SUP_ERR_VERIFY;
    winlogon_name = format_winlogon_name( ATRUST_NAME, rm->name.text );
    if( !winlogon_name ) return SUP_ERR_VERIFY;
    winlogon_unregister( winlogon_name );
    free( winlogon_name );
    return SUP_ERR_NO;
}
#endif

#include "Win32/ric_rc.h"

static const TSupSysInfoIDSBaseItem ATRUST_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, AIDS_TRUST_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_TRUST, SUPSYS_IDS_ICON_QUANT_V1 },
};

/*!
 * \ingroup trust_internal
 * \brief Описание смещений ресурса для Оскра
 */
static const TSupSysInfoIDSBase ATRUST_IDS = 
{
    sizeof(TSupSysInfoIDSBase),
    sizeof(ATRUST_IDS_ITEMS)/sizeof(ATRUST_IDS_ITEMS[0]),
    ATRUST_IDS_ITEMS
};

/*!
 * \ingroup trust_fun_general
 * \brief Функция возвращающая смещение ресурса для Магистра
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TSupSysInfoIDSBase
 * \return код ошибки.
 * \retval #SUP_ERR_NO смещение ресурса возвращено.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
 */
static TSupErr Atrust_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &ATRUST_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_is_impl.h 57706 2009-10-16 10:20:08Z borodin $ */
