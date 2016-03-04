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
 * \version $Revision: 57706 $
 * \date $Date: 2009-10-16 14:20:08 +0400 (Fri, 16 Oct 2009) $
 * \author $Author: borodin $
 * \brief Описание внутренних функций работы с картой РИК.
 */

#if !defined( READER_RIC_RIC_INT_H )
#define READER_RIC_RIC_INT_H

#if !defined( _RESOURCE )
/* Заменитель ресурса РИК. */
extern TSupResourceInstance RIC_RESOURCE;
#else
#define RIC_RESOURCE RIC_DLL_INSTANCE
extern HINSTANCE RIC_DLL_INSTANCE;
#endif /* !defined( _RESOURCE ) */

#define RIC_UNIQUE_LENGTH 12
#define OSCAR_UNIQUE_LENGTH 14
#define TRUST_UNIQUE_LENGTH 17
#define MAX_UNIQUE_LENGTH 17

#define RIC_STATUS_START 0
#define RIC_STATUS_PIN_VERIFIED 1
#define RIC_STATUS_FILE_SELECTED 2

#define RIC_APPLICATION 0x0b00

#define RIC_BLOCK_READ 64
#define RIC_BLOCK_WRITE 64
#define OSCAR_BLOCK_READ 118
#define OSCAR_BLOCK_WRITE 32
#define TRUST_BLOCK_READ 118
#define TRUST_BLOCK_WRITE 32

#define RIC_PATH_CFG _TEXT( "\\config\\KeyCarriers\\RIC\\" )
#define OSCAR_PATH_CFG _TEXT( "\\config\\KeyCarriers\\Oscar\\" )
#define OSCAR2_PATH_CFG _TEXT( "\\config\\KeyCarriers\\Oscar2\\" )

#define BASE_FILE_CFG _TEXT( "\\size_" )
#define RIC_ATR _TEXT( "\\ATR" )
#define RIC_MASK _TEXT( "\\Mask" )
#define RIC_FOLDERS _TEXT( "\\Folders" )
#define DEFAULT_CONNECT _TEXT( "Default" )

#define PF_FILES 1
#define PF_WINLOGON 2
#define PF_APPLICATION 4

/* Массив соответствия номера файла реальному номеру. */
extern const unsigned char OSCAR2_FILE_INDEX[6];
/* Порядок записи файлов по файлам на карте. */
extern const unsigned char *OSCAR2_FILE_NUMBERS[2];

typedef TSupErr (*UniqueFormFun)( unsigned char unique_num[4], 
    TCHAR *unique );

struct TRICContext_;
typedef struct TRICContext_ TRICContext;

typedef TSupErr (*Change_pin)( const TRICContext *ctx, 
    unsigned char new_pin[8] );

struct TRICContext_ 
{
    TSCardAPDUCmd apdu;
    TSCardBerTLVSearch ber_tlv_search;
    TSCardSimpleTLVSearch simple_tlv_search;
    TSupSysEContext *reader;
    unsigned char pin[8];
    unsigned char unique[4];
    unsigned char file;
    unsigned short application;
    unsigned status;
    TCHAR *connect;
    size_t file_length;
    /* Значения различные для OSCAR / RIC */
    size_t unique_length;
    UniqueFormFun unique_form;
    Change_pin change_pin;
    size_t block_len_read;
    size_t block_len_write;
    unsigned char class_tag;
};

/* Функция разрегистрации. */
TSupErr ric_unregister(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция освобождения контекста. */
TSupErr ric_context_free(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция дублирования контекста. */
TSupErr ric_context_dup(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция соединения с носителем. */
TSupErr ric_connect_carrier(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция предварительного соединения с носителем. */
TSupErr ric_preconnect_carrier(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция окончательного соединения с носителем. */
TSupErr ric_postconnect_carrier(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция разъединения с носителем. */
TSupErr ric_disconnect_carrier(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция проверки соответствия вставленного носителя контексту. */
TSupErr ric_same_media(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция восстановления соединения после разрыва. */
TSupErr ric_restore_context(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_unique_get(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_login(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_logout(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_file_open(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_file_close(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_file_chsize(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_file_read(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_file_write(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция получения ресурса. */
TSupErr ric_info_instance(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_passwd_length(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_passwd_change(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_error( unsigned char sw1, unsigned char sw2 );

TSupErr ric_unique_num( const TRICContext *reader, 
    unsigned char unique_num[4] );

TSupErr ric_verify_pin( const TRICContext *ctx, unsigned char pin[8],
    int *retries );

TSupErr ric_select_file( const TRICContext *ctx, 
    unsigned char file, size_t *length );

TSupErr ric_file_size( TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_folder_open( TSupSysContext *context, TSupSysInfo *info );

/* ----------------- base ----------------- */

TCHAR *format_winlogon_name( const TCHAR *winlogon_prefix, const TCHAR *connect );
#ifndef UNIX
BOOL winlogon_unregister(LPCTSTR CardName);
#endif
TSupErr base_property_init( 
    TSupSysContext *context, TSupSysInfo *info, unsigned files,
    const TCHAR *BASE_PATH_CFG, const TCHAR *NAME );

TSupErr base_property_done( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr base_wizard_init( 
    TSupSysContext *context, TSupSysInfo *info, unsigned files,
    const TCHAR *def_atr, const TCHAR *def_mask, const TCHAR *def_folder,
    const TCHAR *BASE_PATH_CFG, const TCHAR *NAME );

TSupErr base_wizard_done( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr base_wizard_attach(
    TSupSysContext *context, TSupSysInfo *info );

/* ----------------- РИК ----------------- */
TSupErr ric_unique_form( unsigned char unique_num[4],
    TCHAR *unique );

/* Функция регистрации. */
TSupErr ric_register(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_change_pin( const TRICContext *ctx, 
    unsigned char new_pin[8] );

/* ----------------- Оскар ----------------- */
TSupErr oscar_unique_form( unsigned char unique_num[4],
    TCHAR *unique );

/* Функция регистрации. */
TSupErr oscar_register(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar_change_pin( const TRICContext *ctx, 
    unsigned char new_pin[8] );

TSupErr oscar_info_ids( 
    TSupSysContext *context, TSupSysInfo *info );

/* ----------------- Оскар c 2-мя файлами ----------------- */
TSupErr oscar2_file_open( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_select_file( 
    const TRICContext *ctx, unsigned char file, size_t *length );

TSupErr oscar2_folder_open( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_file_read( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_file_write( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_file_offset(
	unsigned char file_id, size_t *offset, TCHAR *connect);

TSupErr oscar2_restore_context( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_info_ids(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr oscar2_file_length( 
    unsigned char file_id, size_t *length, TCHAR *connect );

TSupErr oscar2_connect_remove( 
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ric_system_flags( 
    TSupSysContext *context, TSupSysInfo *info );

/* ----------------- Магистра ----------------- */

#define DeclSupSysFunction(f) \
    TSupErr f( TSupSysContext *context, \
		TSupSysInfo *info )

DeclSupSysFunction(trust_info_version);
DeclSupSysFunction(trust_info_copyright);
DeclSupSysFunction(trust_info_company);
DeclSupSysFunction(trust_context_dup);
DeclSupSysFunction(trust_register);
DeclSupSysFunction(trust_same_media);
DeclSupSysFunction(trust_connect_carrier);
DeclSupSysFunction(trust_preconnect_carrier);
DeclSupSysFunction(trust_postconnect_carrier);
DeclSupSysFunction(trust_restore_context);
DeclSupSysFunction(trust_unique_get);
DeclSupSysFunction(trust_folder_enum_open);
DeclSupSysFunction(trust_folder_enum_next);
DeclSupSysFunction(trust_folder_enum_close);
DeclSupSysFunction(trust_folder_open);
DeclSupSysFunction(trust_login);
DeclSupSysFunction(trust_logout);
DeclSupSysFunction(trust_file_open);

TSupErr trust_unique_form(unsigned char unique_num[4], TCHAR *unique);

TSupErr trust_select_application( 
    const TRICContext *ctx );

TSupErr trust_change_pin(
    const TRICContext *ctx, 
    unsigned char pin[8] );

TSupErr trust_select_file( 
    const TRICContext *ctx,
    unsigned char file_id, 
    size_t *length );

TSupErr trust_unique_num( 
    const TRICContext *ctx, 
    unsigned char unique_num[4] );

TSupErr trust_verify_pin(
    const TRICContext *ctx, 
    unsigned char pin[8],
    int *retries );


#endif /* !defined( READER_RIC_RIC_INT_H ) */
/* end of file: $Id: ric_int.h 57706 2009-10-16 10:20:08Z borodin $ */
