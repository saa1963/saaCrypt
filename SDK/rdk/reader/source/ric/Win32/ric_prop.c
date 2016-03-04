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
 * \version $Revision: 73217 $
 * \date $Date: 2011-12-15 17:54:57 +0400 (Thu, 15 Dec 2011) $
 * \author $Author: maxdm $
 * \brief Функция настроек (Property page)
 */

#include "ric_prj.h"
#include "ric_rc.h"
#include "reader.kit/supwsys.h"
#include "reader/sup_wnd.h"
#include <windowsx.h>
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <CommCtrl.h>
#include <WinScard.h>
#pragma warning( pop )
#include <WinCrypt.h>
#include "WinCryptEx.h"

#define INVALID_ATR ((size_t)-1)
#define INVALID_CHAR ((unsigned char)-1)

static DECLARE_DLGPROC(size);
static DECLARE_DLGPROC(size_wizard);
static DECLARE_ON_INIT_DIALOG(size);
static DECLARE_ON_INIT_DIALOG(size_wizard);

static DECLARE_ON_NOTIFY(size_wizard);

typedef struct TBaseWizardContext_ {
    const TCHAR *connect;
    unsigned pf;
    const TCHAR *DEF_ATR;
    const TCHAR *DEF_MASK;
    const TCHAR *DEF_FOLDERS;
    const TCHAR *BASE_PATH_CFG;
    LPCTSTR first;
    LPCTSTR last;
    size_t size1;
    size_t size2;
    size_t size6;
    BOOL reserve;
    BOOL winlogon;
    TCHAR ATR[33*2];
    TCHAR mask[33*2];
    TCHAR application[5];
    const TCHAR *NAME;
} TBaseWizardContext;

typedef struct TBasePropertyContext_
{
    const TCHAR *connect;
    const TCHAR *BASE_PATH_CFG;
    const TCHAR *NAME;
    unsigned pf;
} TBasePropertyContext;

static size_t str2atr( const TCHAR *str, unsigned char *atr );
static void atr2str( const unsigned char *atr, size_t length, 
    TCHAR *str );

static BOOL get_scard_csp(TCHAR *csp_name, DWORD size)
{
    DWORD dwIndex, dwProvType, dwSize, dwImpType;
    HCRYPTPROV hProv;

    for(dwIndex = 0; ; dwIndex++)
    {
	dwSize = size;
	if(!CryptEnumProviders(dwIndex, NULL, 0, &dwProvType, csp_name, &dwSize))
	{
	    if(GetLastError() == ERROR_NO_MORE_ITEMS)
		break;
	    else
		continue;
	}
	if(dwProvType != PROV_GOST_2001_DH)
	    continue;
	if(!CryptAcquireContext(&hProv, NULL, csp_name, dwProvType, CRYPT_VERIFYCONTEXT))
	    continue;
	dwSize = sizeof(dwImpType);
	if(!CryptGetProvParam(hProv, PP_IMPTYPE, (BYTE *)&dwImpType, &dwSize, 0))
	{
	    CryptReleaseContext(hProv, 0);
	    continue;
	}
	CryptReleaseContext(hProv, 0);
	if( dwImpType == CRYPT_IMPL_REMOVABLE || dwImpType == CRYPT_IMPL_SOFTWARE )
	    return TRUE;
    }
    return FALSE;
}
static BOOL winlogon_installed()
{
    TCHAR szSCProvider[MAX_PATH];
    return get_scard_csp(szSCProvider, MAX_PATH);
}

static BOOL winlogon_registered(LPCTSTR CardName)
{
    LONG            lReturn = 0;
    SCARDCONTEXT    hContext;
    TCHAR	    szProvider[MAX_PATH];
    TCHAR	    szSCProvider[MAX_PATH];
    DWORD	    cbData = MAX_PATH;

    lReturn =  SCardEstablishContext(
	SCARD_SCOPE_SYSTEM,
	0,
	0,
	&hContext);
    if ( SCARD_S_SUCCESS != lReturn )
	return FALSE;
    lReturn = SCardGetCardTypeProviderName(
	hContext,
	CardName,
	SCARD_PROVIDER_CSP,
	szProvider,
	&cbData);
    SCardReleaseContext(hContext);
    if ( SCARD_S_SUCCESS != lReturn )
	return FALSE;
    if(!get_scard_csp(szSCProvider, MAX_PATH))
	return FALSE;
    if (!_tcscmp(szProvider, szSCProvider))
	return TRUE;
    return FALSE;
}

static BOOL winlogon_register(LPCTSTR CardName, PBYTE ATR, PBYTE ATRMask, DWORD cbATR)
{
    LONG            lReturn = 0;
    SCARDCONTEXT    hContext;
    TCHAR	    szSCProvider[MAX_PATH];
#ifdef _WIN64
    HKEY	    hKey;
    TCHAR	    szRegPath32[MAX_PATH * 2] = _TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\Cryptography\\Calais\\SmartCards\\");
#endif
    if(!get_scard_csp(szSCProvider, MAX_PATH))
	return FALSE;

    lReturn =  SCardEstablishContext(
	SCARD_SCOPE_SYSTEM,
	0,
	0,
	&hContext);
    if ( SCARD_S_SUCCESS != lReturn )
	return FALSE;

    lReturn = SCardIntroduceCardType(hContext, 
	CardName,
	NULL,
	NULL,
	0,       
	ATR,
	ATRMask,
	cbATR);
    if ( IS_ERROR(lReturn) )
    {
	SCardReleaseContext(hContext);
	return FALSE;
    }
    lReturn = SCardSetCardTypeProviderName(
	hContext,
	CardName,
	SCARD_PROVIDER_CSP,
	szSCProvider);
    SCardReleaseContext(hContext);
    if ( SCARD_S_SUCCESS != lReturn )
    {
	return FALSE;
    }

#ifdef _WIN64
    _tcscat(szRegPath32, CardName);

    lReturn = RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRegPath32, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
    if ( ERROR_SUCCESS != lReturn )
	return FALSE;

    lReturn = RegSetValueEx(hKey, _TEXT("Crypto Provider"), 0, REG_SZ, (LPBYTE)szSCProvider,
	(DWORD)(_tcslen(szSCProvider) + 1) * sizeof(TCHAR));
    if ( ERROR_SUCCESS != lReturn )
    {
	RegCloseKey(hKey);
	return FALSE;
    }
    lReturn = RegSetValueEx(hKey, _TEXT("ATR"), 0, REG_BINARY, (LPBYTE)ATR, cbATR);
    if ( ERROR_SUCCESS != lReturn )
    {
	RegCloseKey(hKey);
	return FALSE;
    }
    lReturn = RegSetValueEx(hKey, _TEXT("ATRMask"), 0, REG_BINARY, (LPBYTE)ATRMask, cbATR);
    if ( ERROR_SUCCESS != lReturn )
    {
	RegCloseKey(hKey);
	return FALSE;
    }
    lReturn = RegCloseKey(hKey);
    if ( ERROR_SUCCESS != lReturn )
	return FALSE;
#endif
    return TRUE;
}

BOOL winlogon_unregister(LPCTSTR CardName)//обратную регистрацию старого провайдера делать не буду, хотя инсталлер придется заставить.
{
    LONG            lReturn = 0;
    SCARDCONTEXT    hContext;
#ifdef _WIN64
    TCHAR	    szRegPath32[MAX_PATH * 2] = _TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\Cryptography\\Calais\\SmartCards\\");
#endif

    lReturn =  SCardEstablishContext(
	SCARD_SCOPE_SYSTEM,
	0,
	0,
	&hContext);
    if ( SCARD_S_SUCCESS != lReturn )
	return FALSE;
    lReturn = SCardForgetCardType(
	hContext,
	CardName);
    SCardReleaseContext(hContext);
    if ( SCARD_S_SUCCESS != lReturn )
	return FALSE;

#ifdef _WIN64
    _tcscat(szRegPath32, CardName);

    lReturn = RegDeleteKeyEx(HKEY_LOCAL_MACHINE, szRegPath32, 0, 0);
    if ( ERROR_SUCCESS != lReturn )
	return FALSE;
#endif
    return TRUE;
}

TSupErr oscar2_connect_remove( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoConnectRemove *rm = (TSupSysInfoConnectRemove*)info;
    TCHAR *winlogon_name;

    UNUSED( context );
    if( !rm ) return SUP_ERR_VERIFY;
    winlogon_name = format_winlogon_name( _TEXT( "oscar2" ), rm->name.text );
    if( !winlogon_name ) return SUP_ERR_VERIFY;
    winlogon_unregister( winlogon_name );
    free( winlogon_name );
    return SUP_ERR_NO;
}

/*! 
 * \ingroup ric_fun
 * \brief Функция создания страницы настроек (Property page)
 */
TSupErr base_property_init(
    TSupSysContext *context, 
    TSupSysInfo *info, unsigned pf, const TCHAR *BASE_PATH_CFG, const TCHAR *NAME )
{
    TSupSysWndPropertyInit *inf = (TSupSysWndPropertyInit*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_INFO( info, TSupSysWndPropertyInit );
    SUPSYS_PRE_CCONTEXT( ctx, sizeof( TRICContext ) );

    inf->quant = 1;
    if( inf->pages )
    {
	TBasePropertyContext *ctx = malloc( 
	    sizeof( TBasePropertyContext ) );
	if( !ctx ) return SUP_ERR_MEMORY;
	ctx->connect = inf->connect;
	ctx->pf = pf;
	ctx->BASE_PATH_CFG = BASE_PATH_CFG;
	ctx->NAME = NAME;
	inf->pages->context = ctx;
	inf->pages->hDlg = MAKEINTRESOURCE( IDD_SIZE );
	inf->pages->dlg_proc = size_dlg_proc;
    }
    return SUP_ERR_NO;
}

/*! 
 * \ingroup ric_fun
 * \brief Функция удаления страницы настроек (Property page)
 */
TSupErr base_property_done(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysWndPropertyDone *inf = (TSupSysWndPropertyDone*)info;
    UNUSED( context );
    free( inf->init_info->pages->context );
    return SUP_ERR_NO;
}

/*! 
 * \ingroup ric_fun
 * \brief Функция создания страницы wizard
 */
TSupErr base_wizard_init( TSupSysContext *context, 
    TSupSysInfo *info, unsigned pf,
    const TCHAR *DEF_ATR, const TCHAR *DEF_MASK,
    const TCHAR *DEF_FOLDERS, const TCHAR *BASE_PATH_CFG, const TCHAR *NAME )
{
    TSupSysWndWizardInit *inf = (TSupSysWndWizardInit*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_INFO( info, TSupSysWndWizardInit );
    SUPSYS_PRE_CCONTEXT( ctx, sizeof( TRICContext ) );

    inf->quant = 1;
    if( inf->pages )
    {
	TBaseWizardContext *local = malloc( sizeof( TBaseWizardContext ) );
	if( !local )
	    return SUP_ERR_MEMORY;
	local->pf = pf;
	local->DEF_ATR = DEF_ATR;
	local->DEF_MASK = DEF_MASK;
	local->DEF_FOLDERS = DEF_FOLDERS;
	local->BASE_PATH_CFG = BASE_PATH_CFG;
	local->NAME = NAME;
	local->connect = inf->connect;
	inf->pages->context = local;
	inf->pages->hDlg = MAKEINTRESOURCE( IDD_SIZE_WIZARD );
	inf->pages->dlg_proc = size_wizard_dlg_proc;
	inf->pages->HeaderTitle = MAKEINTRESOURCE( IDS_BASE_FSIZE_TITLE);
	inf->pages->HeaderSubTitle = MAKEINTRESOURCE( IDS_BASE_FSIZE_SUBTITLE);
	hex_line_class_create( RIC_DLL_INSTANCE );
    }
    return SUP_ERR_NO;
}

/*! 
 * \ingroup ric_fun
 * \brief Функция создания страницы wizard
 */
TSupErr base_wizard_attach(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysWndWizardAttach *inf = (TSupSysWndWizardAttach*)info;
    TBaseWizardContext *ctx;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysWndWizardAttach );
    ctx = (TBaseWizardContext*)inf->init_info->pages->context;

    ctx->first = inf->start;
    ctx->last = inf->end;
    return SUP_ERR_NO;
}

/*! 
 * \ingroup ric_fun
 * \brief Функция удаления страницы wizard
 */
TSupErr base_wizard_done(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysWndWizardDone *inf = (TSupSysWndWizardDone*)info;
    TBaseWizardContext *ctx = (TBaseWizardContext*)inf->init_info->pages->context;

    UNUSED( context );
    if( inf->action == TSupSysWndPropertyCmdApply )
    {
	unsigned char atr[32];
	unsigned char mask[32];
	size_t length = 0;
	TCHAR *cfg = inf->init_info->connect ? inf->init_info->connect : DEFAULT_CONNECT;
	size_t path_length = _tcslen( ctx->BASE_PATH_CFG ) 
	    + _tcslen( cfg )
	    + _tcslen( BASE_FILE_CFG ) + 1
	    + _tcslen( RIC_ATR ) 
	    + _tcslen( RIC_MASK ) 
	    + _tcslen( RIC_FOLDERS ) + 1;	
	TCHAR *path = malloc( path_length * sizeof( TCHAR ) );
	if( !path )
	    return SUP_ERR_MEMORY;
	if( ctx->pf & PF_FILES )
	{
	    _sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
		BASE_FILE_CFG, 1 );
	    support_registry_put_long( path, (long)ctx->size1 );
	    _sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
		BASE_FILE_CFG, 2 );
	    support_registry_put_long( path, (long)ctx->size2 );
	    _sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
		BASE_FILE_CFG, 6 );
	    support_registry_put_long( path, (long)ctx->size6 );
	    if( ctx->reserve )
	    {
		_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
		    BASE_FILE_CFG, 4 );
		support_registry_put_long( path, (long)ctx->size1 );
		_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
		    BASE_FILE_CFG, 5 );
		support_registry_put_long( path, (long)ctx->size2 );
	    }
	}
	_sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
	    RIC_ATR );
	length = str2atr( ctx->ATR, atr );
	support_registry_put_hex( path, length, atr );
	_sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
	    RIC_MASK );
	length = str2atr( ctx->mask, mask );
	support_registry_put_hex( path, length, mask );
	if ( ctx->pf & PF_APPLICATION )
	{
	    _sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
		RIC_FOLDERS );
	    support_registry_put_string( path, ctx->application );	    
	}
	free( path );
	if( ctx->pf & PF_WINLOGON )
	{
	    if (ctx->winlogon)/* Add winlogon Apply*/
	    {
		TCHAR *winlogon_name = format_winlogon_name( ctx->NAME, ctx->connect );
		if( winlogon_name )
		{
		    winlogon_register( winlogon_name, atr, mask, (DWORD)length);
		    free( winlogon_name );
		}
	    }
	}
    }
    if( inf->action == TSupSysWndPropertyCmdClose )
    {
	hex_line_class_done( RIC_DLL_INSTANCE );
	free( inf->init_info->pages->context );
    }
    return SUP_ERR_NO;
}

INT_PTR CALLBACK
size_dlg_proc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED( wParam );
    UNUSED( lParam );
    switch (message) {
	HANDLE_MSG (hwnd, WM_INITDIALOG, size_on_init_dialog);
    }
    return FALSE;
}

INT_PTR CALLBACK
size_wizard_dlg_proc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED( wParam );
    UNUSED( lParam );
    switch (message) {
	HANDLE_MSG (hwnd, WM_INITDIALOG, size_wizard_on_init_dialog);
	HANDLE_MSG (hwnd, WM_NOTIFY, size_wizard_on_notify);
    }
    return FALSE;
}

BOOL
size_on_init_dialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    PROPSHEETPAGE *psp = (PROPSHEETPAGE *)lParam;
    TBasePropertyContext *ctx = 
	psp ? (TBasePropertyContext*) psp->lParam : NULL;
    long length = 0;
    size_t l;
    TCHAR text[20];
    const TCHAR *cfg = ctx && ctx->connect ? ctx->connect : DEFAULT_CONNECT;
    size_t path_length = _tcslen( ctx->BASE_PATH_CFG ) 
	+ _tcslen( cfg )
	+ _tcslen( BASE_FILE_CFG ) + 1
	+ _tcslen( RIC_ATR ) 
	+ _tcslen( RIC_MASK ) 
	+ _tcslen( RIC_FOLDERS ) + 1;
    TCHAR *path = malloc( path_length * sizeof( TCHAR ) );
    unsigned char hex[32];
    TCHAR str_hex[33*2];
    TSupErr code;

    UNUSED( hwndFocus ); 
    if( !ctx ) 
    {
	if( path ) free( path );
	return FALSE;
    }
    if( !path ) return FALSE;

    if( ctx->pf & PF_FILES )
    {
	HWND file1 = GetDlgItem( hwnd, IDC_EDIT_FILE1 );
	HWND st_file1 = GetDlgItem( hwnd, IDC_STATIC_FILE1 );
	HWND file2 = GetDlgItem( hwnd, IDC_EDIT_FILE2 );
	HWND st_file2 = GetDlgItem( hwnd, IDC_STATIC_FILE2 );
	HWND file6 = GetDlgItem( hwnd, IDC_EDIT_FILE6 );
	HWND st_file6 = GetDlgItem( hwnd, IDC_STATIC_FILE6 );
	HWND reserve = GetDlgItem( hwnd, IDC_RESERVE );
	
	_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
	    BASE_FILE_CFG, 1 );
	support_registry_get_long( path, &length );
	_sntprintf( text, 20, _TEXT( "%ld" ), length );
	SetWindowText( file1, text );
	ShowWindow( file1, SW_SHOW );
	ShowWindow( st_file1, SW_SHOW );

	_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
	    BASE_FILE_CFG, 2 );
	support_registry_get_long( path, &length );
	_sntprintf( text, 20, _TEXT( "%ld" ), length );
	SetWindowText( file2, text );
	ShowWindow( file2, SW_SHOW );
	ShowWindow( st_file2, SW_SHOW );

	_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
	    BASE_FILE_CFG, 6 );
	support_registry_get_long( path, &length );
	_sntprintf( text, 20, _TEXT( "%ld" ), length );
	SetWindowText( file6, text );
	ShowWindow( file6, SW_SHOW );
	ShowWindow( st_file6, SW_SHOW );

	length = 0;
	_sntprintf( path, path_length, _TEXT( "%s%s%s%d" ), ctx->BASE_PATH_CFG, cfg,
	    BASE_FILE_CFG, 4 );
	support_registry_get_long( path, &length );
	if( length )
	    CheckDlgButton( hwnd, IDC_RESERVE, TRUE );
	ShowWindow( reserve, SW_SHOW );
    }
    if( ctx->pf & PF_APPLICATION )
    {
	HWND app = GetDlgItem( hwnd, IDC_APPLICATION );
	HWND st_app = GetDlgItem( hwnd, IDC_STATIC_APPLICATION );
	ShowWindow( app, SW_SHOW );
	ShowWindow( st_app, SW_SHOW );
	_sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
	    RIC_FOLDERS );
	code = support_registry_get_string( path, &l, NULL );
	if( !code )
	{
	    TCHAR *folders = NULL;
	    folders = malloc( ( l + 1 ) * sizeof( TCHAR ) );
	    if( !folders )
	    {
		free( path );
		return FALSE;
	    }
	    support_registry_get_string( path, &l, folders );
	    SetDlgItemText( hwnd, IDC_APPLICATION, folders );
	}
    }
    if( ctx->pf & PF_WINLOGON )
    {
	if( winlogon_installed() ) /* Есть winlogon */
	{
	    TCHAR *winlogon_name = format_winlogon_name( ctx->NAME, ctx->connect );
	    HWND winlogon = GetDlgItem( hwnd, IDC_WINLOGON );
	    if( winlogon_name 
		&& winlogon_registered( winlogon_name ) ) /* Уже зарегистрирован в Winlogon */
		CheckDlgButton( hwnd, IDC_WINLOGON, TRUE );
	    if( winlogon_name )
		free( winlogon_name );
	    ShowWindow( winlogon, SW_SHOW );
	}
    }
    _sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
	RIC_ATR );
    l = sizeof( hex );
    if( support_registry_get_hex( path, &l, hex ) )
	l = 0;
    if( l > sizeof( hex ) )
	l = 0;
    atr2str( hex, l, str_hex );
    SetDlgItemText( hwnd, IDC_ATR, str_hex );

    _sntprintf( path, path_length, _TEXT( "%s%s%s" ), ctx->BASE_PATH_CFG, cfg,
	RIC_MASK );
    l = sizeof( hex );
    if( support_registry_get_hex( path, &l, hex ) )
	l = 0;
    if( l > sizeof( hex ) )
	l = 0;
    atr2str( hex, l, str_hex );
    SetDlgItemText( hwnd, IDC_MASK, str_hex );
    free( path );

    return TRUE;
}

BOOL
size_wizard_on_init_dialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    PROPSHEETPAGE *psp = (PROPSHEETPAGE *)lParam;
    TBaseWizardContext *ctx = 
	psp ? (TBaseWizardContext*) psp->lParam : NULL;
    HWND atr = GetDlgItem( hwnd, IDC_ATR );
    HWND mask = GetDlgItem( hwnd, IDC_MASK );
    HWND application = GetDlgItem( hwnd, IDC_APPLICATION );
    UNUSED(hwndFocus);

    if( !ctx )
	return FALSE;
    SetUserWindowLong( hwnd, psp->lParam );
    SetWindowText( atr, ctx->DEF_ATR );
    Edit_LimitText( atr, 32*2 );
    SetWindowText( mask, ctx->DEF_MASK );
    Edit_LimitText( mask, 32*2 );    
    if( ctx->pf & PF_FILES )
    {
	HWND file1 = GetDlgItem( hwnd, IDC_EDIT_FILE1 );
	HWND file1_st = GetDlgItem( hwnd, IDC_STATIC_FILE1 );
	HWND file1_rem = GetDlgItem( hwnd, IDC_STATIC_FILE1_REM );
	HWND file2 = GetDlgItem( hwnd, IDC_EDIT_FILE2 );
	HWND file2_st = GetDlgItem( hwnd, IDC_STATIC_FILE2 );
	HWND file2_rem = GetDlgItem( hwnd, IDC_STATIC_FILE2_REM );
	HWND file6 = GetDlgItem( hwnd, IDC_EDIT_FILE6 );
	HWND file6_st = GetDlgItem( hwnd, IDC_STATIC_FILE6 );
	HWND file6_rem = GetDlgItem( hwnd, IDC_STATIC_FILE6_REM );
	HWND reserve = GetDlgItem( hwnd, IDC_RESERVE );

	SetWindowText( file1, _TEXT( "60" ) );
	ShowWindow( file1, SW_SHOW );
	ShowWindow( file1_st, SW_SHOW );
	ShowWindow( file1_rem, SW_SHOW );
	Edit_LimitText( file1, 4 );
	SetWindowText( file2, _TEXT( "70" ) );
	ShowWindow( file2, SW_SHOW );
	ShowWindow( file2_st, SW_SHOW );
	ShowWindow( file2_rem, SW_SHOW );
	Edit_LimitText( file2, 4 );
	SetWindowText( file6, _TEXT( "62" ) );
	ShowWindow( file6, SW_SHOW );
	ShowWindow( file6_st, SW_SHOW );
	ShowWindow( file6_rem, SW_SHOW );
	Edit_LimitText( file6, 4 );
	CheckDlgButton( hwnd, IDC_RESERVE, TRUE );
	ShowWindow( reserve, SW_SHOW );
    }
    if( ctx->pf & PF_APPLICATION )
    {
	HWND app = GetDlgItem( hwnd, IDC_APPLICATION );
	HWND st_app = GetDlgItem( hwnd, IDC_STATIC_APPLICATION );
	ShowWindow( app, SW_SHOW );
	ShowWindow( st_app, SW_SHOW );
	SetWindowText( application, ctx->DEF_FOLDERS );
	Edit_LimitText( application, 4 );
    }
    if( ctx->pf & PF_WINLOGON )
    {
	HWND winlogon = GetDlgItem( hwnd, IDC_WINLOGON );
	if( winlogon_installed() ) /* Winlogon ready*/
	{
	    CheckDlgButton( hwnd, IDC_WINLOGON, TRUE );
	    ShowWindow( winlogon, SW_SHOW );
	}
    }
    return TRUE;
}

static LRESULT
size_wizard_on_notify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    LPNMLISTVIEW list_view = (LPNMLISTVIEW)pnmhdr;
    TBaseWizardContext *ctx = (TBaseWizardContext*)GetUserWindowLong( hwnd );

    UNUSED( idFrom );

    switch (list_view->hdr.code) {
    case PSN_SETACTIVE : // Enable the Next and Back button
	{
	    PropSheet_SetWizButtons( GetParent( hwnd ), 
		PSWIZB_NEXT | PSWIZB_BACK);
	    break;
	}
    case PSN_WIZNEXT:
	{
	    ctx->size1 = GetDlgItemInt( hwnd, IDC_EDIT_FILE1, NULL, FALSE );
	    ctx->size2 = GetDlgItemInt( hwnd, IDC_EDIT_FILE2, NULL, FALSE );
	    ctx->size6 = GetDlgItemInt( hwnd, IDC_EDIT_FILE6, NULL, FALSE );
	    ctx->reserve = IsDlgButtonChecked( hwnd, IDC_RESERVE ) == BST_CHECKED;
	    ctx->winlogon = IsDlgButtonChecked( hwnd, IDC_WINLOGON ) == BST_CHECKED;
	    GetDlgItemText( hwnd, IDC_ATR, ctx->ATR, 
		sizeof( ctx->ATR ) / sizeof( TCHAR ) );
	    GetDlgItemText( hwnd, IDC_MASK, ctx->mask, 
		sizeof( ctx->mask ) / sizeof( TCHAR ) );
	    if( ctx->pf & PF_APPLICATION )
	    {
		GetDlgItemText( hwnd, IDC_APPLICATION, ctx->application, 
		    sizeof( ctx->application ) / sizeof( TCHAR ) );
	    }
	    SetResultWindowLong( hwnd, (LPARAM)( ctx->last ) );
	    ctx->winlogon = IsDlgButtonChecked( hwnd, IDC_WINLOGON ) == BST_CHECKED;
	    return -1;
	}
    case PSN_WIZBACK:
	{
	    SetResultWindowLong( hwnd, (LPARAM)( ctx->first ) );
	    return -1;
	}
    }
    return 0;
}

static unsigned char c2d( TCHAR c )
{
    if( c >= '0' && c <= '9' ) return (unsigned char)( c - '0' );
    if( c >= 'A' && c <= 'F' ) return (unsigned char)( c - 'A' + 10 ); 
    if( c >= 'a' && c <= 'f' ) return (unsigned char)( c - 'a' + 10 ); 
    return INVALID_CHAR;
}

static size_t str2atr( const TCHAR *str, unsigned char *atr )
{
    size_t length = 0;
    for( ; *str; str += 2, atr++, length++ )
    {
	unsigned char a, b;
	if( !str[1] ) return INVALID_ATR;
	a = c2d( *str );
	b = c2d( str[1] );
	if( a == INVALID_CHAR || b == INVALID_CHAR )
	    return INVALID_ATR;
	*atr = ( a << 4 ) | b;
    }
    return length;
}

static TCHAR d2c( unsigned char d )
{
    if( d >= 10 ) return d - 10 + 'A';
    return d + '0';
}

static void atr2str( const unsigned char *atr, size_t length, 
    TCHAR *str )
{
    for( ; length; length--, str += 2, atr++ )
    {
	*str = d2c( *atr >> 4 );
	str[1] = d2c( *atr & 0xF );
    }
    *str = 0;
}

TCHAR *format_winlogon_name( const TCHAR *winlogon_prefix, const TCHAR *connect )
{
    TCHAR *name = malloc( ( _tcslen( winlogon_prefix ) + 1 + ( connect ? _tcslen( connect ) : 0 ) + 1) * sizeof( TCHAR ) );
    if( !name ) return NULL;
    _tcscpy( name, winlogon_prefix );
    if( !connect ) return name;
    _tcscat( name, _TEXT( "_" ) );
    _tcscat( name, connect );
    return name;
}
/* end of file: $Id: ric_prop.c 73217 2011-12-15 13:54:57Z maxdm $ */
