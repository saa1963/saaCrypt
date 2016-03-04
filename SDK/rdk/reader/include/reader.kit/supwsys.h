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

/*
 * \file supwsys.h,v
 * \version 1.4
 * \date 2007/05/29 15:29:33
 * \author dim
 * \brief Объявления необходимые для написания оконнного интерфейса
 *  поддержки элемента перечисления.
 */

#if !defined( _SUPPORT_SUPWSYS_H )
#define _SUPPORT_SUPWSYS_H

#include "reader/support.h"
#pragma warning( push )
#pragma warning( disable:4201 )
#include <prsht.h>
#pragma warning( pop )

#define SUPSYSW_FUN_PROPERTY_INIT 0x200
#define SUPSYSW_FUN_PROPERTY_DONE 0x201
#define SUPSYSW_FUN_WIZARD_INIT 0x202
#define SUPSYSW_FUN_WIZARD_ATTACH 0x203
#define SUPSYSW_FUN_WIZARD_DONE 0x204

typedef enum TSupSysWndPropertyAction_ {
    TSupSysWndPropertyCmdApply,
    TSupSysWndPropertyCmdClose
} TSupSysWndPropertyAction;

typedef struct TSupSysWndPropertyPage_
{
    DLGPROC dlg_proc;
    void *context;
    LPCTSTR hDlg;
} TSupSysWndPropertyPage;

typedef struct TSupSysWndWizardPage_
{
    DLGPROC dlg_proc;
    void *context;
    LPCTSTR hDlg;
    TCHAR *HeaderTitle;
    TCHAR *HeaderSubTitle;
} TSupSysWndWizardPage;

typedef struct TSupSysWndPropertyInit_
{
    size_t size_of;
    size_t quant;
    TSupSysWndPropertyPage *pages;
    TCHAR *connect;
} TSupSysWndPropertyInit;

typedef struct TSupSysWndPropertyDone_
{
    size_t size_of;
    TSupSysWndPropertyAction action;
    TSupSysWndPropertyInit *init_info;
} TSupSysWndPropertyDone;

typedef struct TSupSysWndWizardInit_
{
    size_t size_of;
    size_t quant;
    TSupSysWndWizardPage *pages;
    TCHAR *connect;
} TSupSysWndWizardInit;

typedef struct TSupSysWndWizardDone_
{
    size_t size_of;
    TSupSysWndPropertyAction action;
    TSupSysWndWizardInit *init_info;
} TSupSysWndWizardDone;

typedef struct TSupSysWndWizardAttach_
{
    size_t size_of;
    LPCTSTR start;
    LPCTSTR end;
    TSupSysWndWizardInit *init_info;
} TSupSysWndWizardAttach;

_SUPPORT_DECL TSupErr hex_line_class_create(
    TSupModuleInstance hInstance );
_SUPPORT_DECL TSupErr hex_line_class_done(
    TSupModuleInstance hInstance );

_SUPPORT_DECL TSupErr word_line_class_create(
    TSupModuleInstance hInstance );
_SUPPORT_DECL TSupErr word_line_class_done(
    TSupModuleInstance hInstance );

#endif
