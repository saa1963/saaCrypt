/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

#if !defined( _READER_RDR_WND_H )
#define _READER_RDR_WND_H

#include "reader/rdr.h"
#include "reader/sup_syse.h"

/*! ���������� ������ � ������ v.1 ��� �������. */
#define RDR_WND_CHOICE_FLAGS 2

/*+ ������ ������ ��� ��� ������. +*/
#define RDR_WND_ERROR_TEXT_LENGTH 255

/*! �������, �� ������� ���������� ����� CALLBACK ������� ����: ���������� */
#define RDR_WND_PASSWD_CALLBACK_COMMAND_FILL 0

/*! �������, �� ������� ���������� ����� CALLBACK ������� ����: ������� */
#define RDR_WND_PASSWD_CALLBACK_COMMAND_CLEAR 1

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

typedef struct TRdrWndConfigure_
{
    size_t size_of; /*!< [in] ������ ���������. */
    HWND parent; /*!< [in] ��� Win32, ������� ����. */
} TRdrWndConfigure;

typedef struct TRdrMediaWndConfigure_
{
    size_t size_of; /*!< [in] ������ ���������. */
    HWND parent; /*!< [in] ��� Win32, ������� ����. */
} TRdrMediaWndConfigure;

typedef struct TRdrWndChoiceCarrierCallback_
{
    size_t size_of; /*!< [in] ������ ���������. */
    HWND window; 
    TCHAR *err_text; /*!< ����� ��������� �� ������. */
    TCHAR *unique; /*!< ���������� ����� ������������ ��������. */
    TSupSysEContext *context; /*!< �������� �����������. */
    int canceled; /*!< ��������� ���������. */
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
    size_t size_of; /*!< ������ ���������. */
    HWND parent; /*!< ��� Win32, ������� ����. */
    TCHAR *header; /*!< ��������� ����, NULL ��� ������������. */
    TCHAR *text; /*!< ����� ������ ����, NULL ��� ������������. */
    TRdrWndChoiceCarrierCallbackFun check_fun; /*!< ������� �������� 
	������������ ������������ ��������, NULL ���� ����� ��������. */
    TRdrWndChoiceCarrierCallbackFun find_fun; /*!< ������� ������. */
    TRdrWndChoiceCarrierCallbackFun exclude_fun; /*!< ������� �������� 
	��������� �������� � ������. */
    TRdrWndChoiceCarrierCallbackFun cancel_fun; /*!< ������� ���������� 
	��� cancel. */
    union {
	const TSupSysEList *reader_list; /*!< ������ ������������. */
	TSupSysEContext *rdr; /*!< ���� �����������. */
    } source;
    size_t flags_quant;
    TRdrWndChoiceCarrierFlags *flags;
} TRdrWndChoiceCarrier;

typedef struct TRdrWndRetry_
{
    size_t size_of; /*!< ������ ���������. */
    HWND parent; /*!< ��� Win32, ������� ����. */
    TCHAR *header; /*!< ��������� ����, NULL ��� ������������. */
    TCHAR *text; /*!< ����� ������ ����, NULL ��� ������������. */
    TRdrWndChoiceCarrierCallbackFun cancel_fun; /*!< ������� ���������� 
	��� cancel. */
    TRdrWndChoiceCarrierCallbackFun find_fun; /*!< ������� ������. */
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
