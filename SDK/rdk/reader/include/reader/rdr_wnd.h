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

#if !defined( _READER_RDR_WND_H )
#define _READER_RDR_WND_H

#include "reader/rdr.h"
#include "reader/sup_syse.h"

/*! Количество флагов в версии v.1 для диалога. */
#define RDR_WND_CHOICE_FLAGS 2

/*+ Размер строки под код ошибки. +*/
#define RDR_WND_ERROR_TEXT_LENGTH 255

/*! Команда, по которой происходит вызов CALLBACK функции окна: заполнение */
#define RDR_WND_PASSWD_CALLBACK_COMMAND_FILL 0

/*! Команда, по которой происходит вызов CALLBACK функции окна: очистка */
#define RDR_WND_PASSWD_CALLBACK_COMMAND_CLEAR 1

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

typedef struct TRdrWndConfigure_
{
    size_t size_of; /*!< [in] Размер структуры. */
    HWND parent; /*!< [in] Для Win32, главное окно. */
} TRdrWndConfigure;

typedef struct TRdrMediaWndConfigure_
{
    size_t size_of; /*!< [in] Размер структуры. */
    HWND parent; /*!< [in] Для Win32, главное окно. */
} TRdrMediaWndConfigure;

typedef struct TRdrWndChoiceCarrierCallback_
{
    size_t size_of; /*!< [in] Размер структуры. */
    HWND window; 
    TCHAR *err_text; /*!< Текст сообщения об ошибке. */
    TCHAR *unique; /*!< Уникальный номер вставленного носителя. */
    TSupSysEContext *context; /*!< Контекст считывателя. */
    int canceled; /*!< Состояние остановки. */
    void *info;
} TRdrWndChoiceCarrierCallback;

typedef TSupErr (*TRdrWndChoiceCarrierCallbackFun)( TRdrWndChoiceCarrierCallback* );

typedef struct TRdrWndChoiceCarrierFlags_
{
    unsigned disable_ok : 1; 
    unsigned single_reader : 1;
} TRdrWndChoiceCarrierFlags;

typedef struct TRdrWndChoiceCarrier_
{
    size_t size_of; /*!< Размер структуры. */
    HWND parent; /*!< Для Win32, главное окно. */
    TCHAR *header; /*!< Заголовок окна, NULL для стандартного. */
    TCHAR *text; /*!< Текст внутри окна, NULL для стандартного. */
    TRdrWndChoiceCarrierCallbackFun check_fun; /*!< Функция проверки 
	правильности вставленного носителя, NULL если любой носитель. */
    TRdrWndChoiceCarrierCallbackFun find_fun; /*!< Функция поиска. */
    TRdrWndChoiceCarrierCallbackFun exclude_fun; /*!< Функция проверки 
	включения элемента в список. */
    TRdrWndChoiceCarrierCallbackFun cancel_fun; /*!< Функция вызываемая 
	при cancel. */
    union {
	const TSupSysEList *reader_list; /*!< Список считывателей. */
	TSupSysEContext *rdr; /*!< Один считыватель. */
    } source;
    size_t flags_quant;
    TRdrWndChoiceCarrierFlags *flags;
} TRdrWndChoiceCarrier;

typedef struct TRdrWndRetry_
{
    size_t size_of; /*!< Размер структуры. */
    HWND parent; /*!< Для Win32, главное окно. */
    TCHAR *header; /*!< Заголовок окна, NULL для стандартного. */
    TCHAR *text; /*!< Текст внутри окна, NULL для стандартного. */
    TRdrWndChoiceCarrierCallbackFun cancel_fun; /*!< Функция вызываемая 
	при cancel. */
    TRdrWndChoiceCarrierCallbackFun find_fun; /*!< Функция поиска. */
} TRdrWndRetry;

_RDR_DECL TSupErr rdr_wnd_choice_carrier( 
    const TRdrWndChoiceCarrier *init_info, void *ret_info );

_RDR_DECL TSupErr rdr_wnd_retry_set( 
    TSupSysEContext *ctx,
    TRdrWndRetry *init_info, void *use_info );

#ifndef NO_SUP_PROPERTIES
_RDR_DECL TSupErr rdr_wnd_configure( TRdrWndConfigure *info );
_RDR_DECL TSupErr rdr_media_wnd_configure( TRdrMediaWndConfigure *info );
#endif // !NO_SUP_PROPERTIES

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_RDR_WND_H ) */
/*+ end of file: $Id: rdr_wnd.h 75284 2012-02-29 10:11:43Z dim $ +*/
