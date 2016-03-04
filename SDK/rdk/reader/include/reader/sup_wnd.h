#ifndef _READER_SUPPORT_SUP_WND_H
#define _READER_SUPPORT_SUP_WND_H

#ifdef UNIX
#error Ума не приложу, зачем этот файл на UNIX-е
#endif // UNIX

#include "reader/support.h"
#include <windows.h>
#include <windowsx.h>
#ifdef WINCE
#include <winuserm.h>
#endif // WINCE
#include "comctrl2.h" // Пока только для флага WS_NONAVDONEBUTTON

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
/* На PocketPC и на смартфоне не поддерживаем (совсем) изменение положения окон */
#define NO_CHANGEWINDOWPOSITION
/* Кроме того, не поддерживаем подробный режим просмотра */
#define NO_DETAILS
/* Кроме того, не поддерживаем внутриоконный таймер для закрытия окна */
#define NO_TIMER_REMAIN
// На покете и смартфоне решили не реализовать сложные перегруженные
// диалоги создания разбитых на части контейнеров
#define NO_NK
#endif /* defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) */

#ifndef WINCE
#define SUPPORT_MB_SYSTEMMODAL MB_SYSTEMMODAL
#else /* WINCE */
#define SUPPORT_MB_SYSTEMMODAL MB_APPLMODAL
#endif /* WINCE */

#define DECLARE_DLGPROC(name) INT_PTR CALLBACK name##_dlg_proc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
#define DECLARE_ON_INIT_DIALOG(name) BOOL name##_on_init_dialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
#define DECLARE_ON_NOTIFY(name) LRESULT name##_on_notify (HWND hwnd, int idFrom, LPNMHDR pnmhdr)
#define DECLARE_ON_COMMAND(name) void name##_on_command (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
#define DECLARE_ON_SET_CURSOR(name) BOOL name##_on_set_cursor(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg)
#define DECLARE_ON_TIMER(name) void name##_on_timer(HWND hwnd, UINT id)
#define DECLARE_ON_SYSCOLORCHANGE(name) void name##_on_syscolorchange(HWND hwnd)
#define DECLARE_ON_SIZE(name) void name##_on_size(HWND hwnd, UINT state, int cx, int cy)
#ifndef WINCE
#define DECLARE_ON_GETMINMAXINFO(name) void name##_on_get_min_max_info(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
#endif // WINCE
#define DECLARE_ON_LBUTTONUP(name) void name##_on_lbuttonup(HWND hwnd, int x, int y, UINT keyFlags)
#define DECLARE_ON_RBUTTONUP(name) void name##_on_rbuttonup(HWND hwnd, int x, int y, UINT keyFlags)
#define DECLARE_ON_MOUSEMOVE(name) void name##_on_mousemove(HWND hwnd, int x, int y, UINT keyFlags)
#define DECLARE_ON_HOTKEY(name) void name##_on_hotkey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk)
#define DECLARE_ON_DESTROY(name) void name##_on_destroy(HWND hwnd)
#define DECLARE_ON_CTL_COLOR(name) HBRUSH name##_on_ctl_color(HWND hwnd, HDC hdc, HWND hwndChild, int type)
#define DECLARE_ON_SETTING_CHANGE(name) void name##_on_setting_change(HWND hwnd, UINT uiAction, LPCTSTR lpszSectionName)
#define DECLARE_ON_DELETEITEM(name) void name##_on_deleteitem(HWND hwnd, const DELETEITEMSTRUCT * lpDeleteItem)

/* void Cls_OnSettingChange(HWND hwnd, UINT uiAction, LPCTSTR lpszSectionName) */
#define HANDLE_WM_SETTINGCHANGE(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (UINT)(wParam), (LPCTSTR)(lParam)), 0L)
#define FORWARD_WM_SETTINGCHANGE(hwnd, wFlag, lpszSectionName, fn) \
    (void)(fn)((hwnd), WM_SETTINGCHANGE, (WPARAM)(UINT)(wFlag), \
    (LPARAM)(LPCTSTR)(lpszSectionName))

#if defined(_WIN32) && !defined(_WIN64)
// Из-за ошибок в системных заголовках (которые заключаются в следующем:
// для _WIN32 Функции SetWindowLongPtrX переопределены на SetWindowLongX,
// а у последних последний параметр имеет тип LONG, а не LONG_PTR)
// везде при 32-битной компиляции выдаются 64-битные предупреждения.
// Эти предупреждения не имеют здесь никакого смысла, но нужно,
// чтобы все было чисто. Т.к. я не хочу отключать совсем 64-битные
// предупреждения, то исправляю ошибку здесь.
#undef SetWindowLongPtrA
#undef SetWindowLongPtrW
#undef SetWindowLongPtr

#define SetWindowLongPtrA(hWnd,nIndex,dwNewLong) SetWindowLongA(hWnd,nIndex,(LONG)(dwNewLong))
#define SetWindowLongPtrW(hWnd,nIndex,dwNewLong) SetWindowLongW(hWnd,nIndex,(LONG)(dwNewLong))
#ifdef UNICODE
#define SetWindowLongPtr  SetWindowLongPtrW
#else
#define SetWindowLongPtr  SetWindowLongPtrA
#endif // !UNICODE

#undef GetWindowLongPtrA
#undef GetWindowLongPtrW
#undef GetWindowLongPtr

#define GetWindowLongPtrA (LONG_PTR)GetWindowLongA
#define GetWindowLongPtrW (LONG_PTR)GetWindowLongW
#ifdef UNICODE
#define GetWindowLongPtr  GetWindowLongPtrW
#else
#define GetWindowLongPtr  GetWindowLongPtrA
#endif // !UNICODE

#undef SetClassLongPtrA
#undef SetClassLongPtrW
#undef SetClassLongPtr

#define SetClassLongPtrA(hWnd,nIndex,dwNewLong) SetClassLongA(hWnd,nIndex,(LONG)(dwNewLong))
#define SetClassLongPtrW(hWnd,nIndex,dwNewLong) SetClassLongW(hWnd,nIndex,(LONG)(dwNewLong))
#ifdef UNICODE
#define SetClassLongPtr  SetClassLongPtrW
#else
#define SetClassLongPtr  SetClassLongPtrA
#endif // !UNICODE

#undef GetClassLongPtrA
#undef GetClassLongPtrW
#undef GetClassLongPtr

#define GetClassLongPtrA (LONG_PTR)GetClassLongA
#define GetClassLongPtrW (LONG_PTR)GetClassLongW
#ifdef UNICODE
#define GetClassLongPtr  GetClassLongPtrW
#else
#define GetClassLongPtr  GetClassLongPtrA
#endif // !UNICODE

#endif // _WIN32

#ifndef WINCE

#define GetUserWindowLong(hWnd)    GetWindowLongPtr(hWnd, DWLP_USER)
#define GetUserWindowLongWnd(hWnd) GetWindowLongPtr(hWnd, GWLP_WNDPROC)

#define SetResultWindowLong(hWnd,dwNewLong) SetWindowLongPtr(hWnd,DWLP_MSGRESULT,dwNewLong)
#define SetUserWindowLong(hWnd,dwNewLong)   SetWindowLongPtr(hWnd,DWLP_USER,dwNewLong)
#define SetWindowProcLong(hWnd,dwNewLong)   SetWindowLongPtr(hWnd,GWLP_WNDPROC,dwNewLong)

#define SetCursorClassLong(hWnd,dwNewLong)  SetClassLongPtr(hWnd,GCLP_HCURSOR,dwNewLong)

#else /* WINCE */

// Для CE функции XxxPtr не определны, не буду определять их и я,
// создам замечательные макросы и буду использовать только их

#define GetUserWindowLong(hWnd)    GetWindowLong(hWnd, DWL_USER)
#define GetUserWindowLongWnd(hWnd) GetWindowLong(hWnd, GWL_WNDPROC)

#define SetResultWindowLong(hWnd,dwNewLong) SetWindowLong(hWnd,DWL_MSGRESULT,dwNewLong)
#define SetUserWindowLong(hWnd,dwNewLong)   SetWindowLong(hWnd,DWL_USER,dwNewLong)
#define SetWindowProcLong(hWnd,dwNewLong)   SetWindowLong(hWnd,GWL_WNDPROC,dwNewLong)

#define SetCursorClassLong(hWnd,dwNewLong)  SetClassLongPtr(hWnd,GCL_HCURSOR,dwNewLong)

#endif /* WINCE */

#define GetDlgItemTextLength(hDlg,nIDDlgItem) \
    SendDlgItemMessage(hDlg,nIDDlgItem,WM_GETTEXTLENGTH,0,0)

    /* void Cls_OnHelp_X1(HWND hwnd, LPHELPINFO lphi, const DWORD *ptr) */
#define HANDLE_WM_HELP_X1(hwnd, wParam, lParam, fn, ids) \
    ((fn)((hwnd), (LPHELPINFO)lParam, ids), 1L)
#define HANDLE_WM_CONTEXTMENU_X1(hwnd, wParam, lParam, fn, ids) \
    ((fn)((hwnd), (HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), ids), 0L)
#define HANDLE_MSG_X1(hwnd, message, fn,x1) \
    case (message): return HANDLE_##message##_X1((hwnd), (wParam), (lParam), (fn), (x1))

#ifndef NO_CHANGEWINDOWPOSITION
_SUPPORT_DECL TSupErr support_wnd_store_window_placement(
    HWND hwnd, const TCHAR *place );
_SUPPORT_DECL TSupErr support_wnd_store_window_size(
    HWND hwnd, const TCHAR *place );
_SUPPORT_DECL TSupErr support_wnd_set_window_placement( 
    HWND hwnd, const TCHAR *place );
_SUPPORT_DECL TSupErr support_wnd_set_window_size( 
    HWND hwnd, const TCHAR *place );
#else /* NO_CHANGEWINDOWPOSITION */
#define support_wnd_store_window_placement(hwnd,place)
#define support_wnd_store_window_size(hwnd,place)
#define support_wnd_set_window_placement(hwnd,place)
#define support_wnd_set_window_size(hwnd,place)
#endif /* NO_CHANGEWINDOWPOSITION */

#ifndef NO_TIMER_REMAIN
_SUPPORT_DECL TSupErr support_wnd_set_time( HWND hwnd, int item, 
    time_t t );
#define SET_TIMER_REMAIN(hwnd,context_time,nIdStatic,idTimer,uTimeOut) \
    support_wnd_set_time(hwnd,nIdStatic,context_time); \
    if( (context_time) > 0 ) { \
	SetTimer( hwnd, idTimer, uTimeOut, NULL ); \
    }
#define KILL_TIMER_REMAIN(hwnd,nIdStatic) KillTimer(hwnd,nIdStatic)
#else // NO_TIMER_REMAIN
#define support_wnd_set_time(hwnd,item,t)
#define SET_TIMER_REMAIN(hwnd,context_time,nIdStatic,idTimer,uTimeOut)
#define KILL_TIMER_REMAIN(hwnd,nIdStatic)
#endif // NO_TIMER_REMAIN

#ifndef WINCE
#define support_wnd_get_message_time(hDlg) GetMessageTime()
#else /* WINCE */
// На CE есть только такая функция
#define support_wnd_get_message_time(hDlg) GetMessageQueueReadyTimeStamp(hDlg)
#endif /* WINCE */

#ifndef WINCE
_SUPPORT_DECL INT_PTR support_wnd_dialog_box_param( 
    HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent,
    DLGPROC lpDialogFunc, LPARAM dwInitParam);
_SUPPORT_DECL WORD support_wnd_language_get();
_SUPPORT_DECL TSupErr support_wnd_language_update();
_SUPPORT_DECL TSupErr support_wnd_language_done();
#else // WINCE
#define support_wnd_dialog_box_param DialogBoxParam
#define support_wnd_language_get() (WORD)GetACP()
#define support_wnd_language_update()
#define support_wnd_language_done()
typedef LPCDLGTEMPLATE PROPSHEETPAGE_RESOURCE;
// Из C++ файла atlosapice.h (нельзя его вкслючить, т.к. он не является Си-файлом):
// The following are not included in Windows CE SDK headers, but they are included in
// the windows headers, and they are utilized by ATL.
#ifndef IS_INTRESOURCE 
#define IS_INTRESOURCE(_r) (((ULONG_PTR)(_r) >> 16) == 0)
#endif // IS_INTRESOURCE
#endif // WINCE

LPCDLGTEMPLATE support_wnd_load_dialog_template( 
    HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent);

#ifdef WIN32_PLATFORM_PSPC
// На покете полужирный цветной системный фонт
HFONT __inline support_wnd_set_title_font(HWND hwnd, int nId, BOOL bRepaint)
{
    HFONT hFont = (HFONT)SendDlgItemMessage(hwnd, nId, WM_GETFONT, 0, 0);
    HFONT hFontBold = NULL;
    LOGFONT lf = { 0 };

    if(GetObject(hFont, sizeof(LOGFONT), &lf) != sizeof(LOGFONT))
	return 0;
    lf.lfWeight = FW_SEMIBOLD;
    hFontBold =  CreateFontIndirect(&lf);
    if (!hFontBold)
	return 0;
    SendDlgItemMessage(hwnd, nId, WM_SETFONT, (WPARAM)hFontBold,
	MAKELPARAM(bRepaint, 0));
    return hFontBold;
}
HBRUSH __inline support_wnd_set_title_color( HWND hwndChild, HDC hdc, int nId )
{
    int nIDStaticCtrl = GetDlgCtrlID( hwndChild );
    if( nIDStaticCtrl != nId )
	return 0;
    SetTextColor( hdc, RGB(0, 0, 156) );
    return GetSysColorBrush( COLOR_WINDOW );
    // Может быть, вернуть 0, если изменения для цвета букв пройдут.
}
#endif //WIN32_PLATFORM_PSPC

#ifdef WIN32_PLATFORM_WFSP
#ifndef MulDiv
#define MulDiv(x, y, z) (LONG)(((LONGLONG)((LONG)(x)) * (LONGLONG)((LONG)(y)))\
    / (LONGLONG)((LONG)(z)))
#endif // MulDiv

// На смартфоне жирный фонт Nina 9 bold
HFONT __inline support_wnd_set_title_font(HWND hwnd, int nId, BOOL bRepaint)
{
    HFONT hFont = (HFONT)SendDlgItemMessage(hwnd, nId, WM_GETFONT, 0, 0);
    HFONT hFontBold = NULL;
    LOGFONT lf = { 0 };

    if(GetObject(hFont, sizeof(LOGFONT), &lf) != sizeof(LOGFONT))
	return 0;
    lf.lfWeight = FW_BOLD;
    lf.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC(hwnd), LOGPIXELSY), 72);
    hFontBold =  CreateFontIndirect(&lf);
    if (!hFontBold)
	return 0;
    SendDlgItemMessage(hwnd, nId, WM_SETFONT, (WPARAM)hFontBold,
	MAKELPARAM(bRepaint, 0));
    return hFontBold;
}
#define support_wnd_set_title_color(hwndChild,hdc,nId)
#endif // WIN32_PLATFORM_WFSP

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)

long __inline support_wnd_get_window_width( HWND hwnd )
{
    RECT rect;
    if( GetClientRect( hwnd, &rect ) )
	return rect.right - rect.left;
    return 0;
}

void __inline support_wnd_extend_title_line( HWND hDlg, int nId, BOOL bRepaint )
{
    HWND hwndLine = GetDlgItem( hDlg, nId );
    if (hwndLine) {
	const int nTitleHeight = 24;
	int nWidth = support_wnd_get_window_width( hDlg );
	MoveWindow( hwndLine, 0, nTitleHeight, nWidth, 1, bRepaint );
    }
}

void __inline support_wnd_extend_dlg_item( HWND hDlg, int nId, BOOL bRepaint )
{
    HWND hwnd = GetDlgItem( hDlg, nId );
    if( hwnd )
    {
	RECT rect;
	if( GetWindowRect( hwnd, &rect ) )
	{
	    int nWidth = support_wnd_get_window_width( hDlg );
	    int nTitleHeight = rect.bottom - rect.top;
	    int delta;
	    MapWindowRect( HWND_DESKTOP, hDlg, &rect );
	    delta = rect.left;
	    MoveWindow( hwnd, rect.left, rect.top,
		nWidth - 2*delta, nTitleHeight, bRepaint );
	}
    }
}

#define SET_TITLE_FONT(hFont,hDlg,nIdLine,nIdText) \
    (hFont) = support_wnd_set_title_font(hDlg,nIdText,FALSE); \
    support_wnd_extend_title_line(hDlg,nIdLine,FALSE); \
    support_wnd_extend_dlg_item(hDlg,nIdText,FALSE)
#define DECLARE_TITLE_FONT(hFont) HFONT hFont;
#define DELETE_TITLE_FONT(hFont) DeleteObject(hFont)
#define SET_TITLE_COLOR(hwndChild,hdc,nId) support_wnd_set_title_color(hwndChild,hdc,nId)
#else // !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
#define SET_TITLE_FONT(hFont,hDlg,nIdLine,nIdText)
#define DECLARE_TITLE_FONT(hFont)
#define DELETE_TITLE_FONT(hFont)
#define SET_TITLE_COLOR(hwndChild,hdc,nId)
#define support_wnd_extend_dlg_item(hDlg,nId,bRepaint)
#define support_wnd_extend_title_line(hDlg,nId,bRepaint)
#endif // !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))

#ifdef WIN32_PLATFORM_PSPC
void __inline support_wnd_disable_nav_done_button(HWND hDlg)
{
    DWORD dwStyle;
    dwStyle = GetWindowLong(hDlg, GWL_STYLE) | WS_NONAVDONEBUTTON;
    SetWindowLong(hDlg, GWL_STYLE, dwStyle);
}
#define DISABLE_NAV_DONE_BUTTON(hDlg) support_wnd_disable_nav_done_button(hDlg)
#else // !WIN32_PLATFORM_PSPC
#define DISABLE_NAV_DONE_BUTTON(hDlg)
#endif // !WIN32_PLATFORM_PSPC

#if defined(WIN32_PLATFORM_PSPC)
#define SH_INIT_DIALOG_FLAGS SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_SIPDOWN | SHIDIF_EMPTYMENU
#elif defined(WIN32_PLATFORM_WFSP)
#define SH_INIT_DIALOG_FLAGS SHIDIF_SIZEDLGFULLSCREEN
#endif // ! (WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP)

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
BOOL __inline support_wnd_sh_init_dialog(HWND hDlg)
{
    SHINITDLGINFO shidi;
    shidi.dwMask = SHIDIM_FLAGS;
    shidi.hDlg = hDlg;
    shidi.dwFlags = SH_INIT_DIALOG_FLAGS;
    return SHInitDialog(&shidi);
}
// Этим макросом удобно пользоваться внутри обработчика WM_INITDIALOG
#define SH_INIT_DIALOG(hDlg) if (!support_wnd_sh_init_dialog(hDlg)) { \
    EndDialog(hDlg, -1); return FALSE; }
#else //!(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
#define SH_INIT_DIALOG(hDlg)
#endif //!(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))

#ifdef WIN32_PLATFORM_WFSP
// На смартфоне используем MenuBar
HWND __inline support_wnd_sh_create_menu_bar(HWND hDlg, HINSTANCE hInstRes, UINT nToolBarId)
{
    SHMENUBARINFO mbi;
    memset(&mbi, 0, sizeof(SHMENUBARINFO));
    mbi.cbSize     = sizeof(SHMENUBARINFO);
    mbi.hwndParent = hDlg;
    mbi.nToolBarId = nToolBarId;
    mbi.hInstRes   = hInstRes;
    if (!SHCreateMenuBar(&mbi))
	return 0;
    return mbi.hwndMB;
}
void __inline support_wnd_override_tback_key(HWND hDlg)
{
    HWND hwnd = SHFindMenuBar(hDlg);
    SendMessage (hwnd, SHCMBM_OVERRIDEKEY, VK_TBACK,
	MAKELPARAM (SHMBOF_NODEFAULT | SHMBOF_NOTIFY,
	SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
}
// Обычно нужно перехватить кнопку Back и сделать хорошие вещи
#define SH_CREATE_MENU_BAR(hDlg,hInstRes,nToolBarId) \
    if (support_wnd_sh_create_menu_bar(hDlg,hInstRes,nToolBarId)) { \
	support_wnd_override_tback_key(hDlg); } else { EndDialog(hDlg,-1); };
#else // !WIN32_PLATFORM_WFSP
#define SH_CREATE_MENU_BAR(hDlg,hInstRes,nToolBarId)
#endif // !WIN32_PLATFORM_WFSP

BOOL __inline EnableDlgItem( HWND hDlg, int idCtrl, BOOL bVal )
{
    return EnableWindow( GetDlgItem( hDlg, idCtrl ), bVal );
}
BOOL __inline IsDlgItemEnabled(HWND hDlg, int idCtrl )
{
    return IsWindowEnabled( GetDlgItem( hDlg, idCtrl ) );
}
BOOL __inline ShowDlgItem( HWND hDlg, int idCtrl, BOOL bShow )
{
    return ShowWindow( GetDlgItem(hDlg, idCtrl ),
	bShow ? SW_SHOW : SW_HIDE );
}

#ifdef WIN32_PLATFORM_WFSP
#define ENABLE_BUTTON(hDlg,idCtrl,bVal) SendMessage(SHFindMenuBar(hDlg),TB_ENABLEBUTTON,idCtrl,bVal)
#define IS_BUTTON_ENABLED(hDlg,idCtrl) (SendMessage(SHFindMenuBar(hDlg),TB_GETSTATE,idCtrl,0)&TBSTATE_ENABLED)
#define SHOW_BUTTON(hDlg,idCtrl,bVal)
#else // WIN32_PLATFORM_WFSP
#define ENABLE_BUTTON(hDlg,idCtrl,bVal) EnableDlgItem(hDlg,idCtrl,bVal)
#define IS_BUTTON_ENABLED(hDlg,idCtrl) IsDlgItemEnabled(hDlg,idCtrl)
#define SHOW_BUTTON(hDlg,idCtrl,bVal) ShowDlgItem(hDlg,idCtrl,bVal)
#endif // !WIN32_PLATFORM_WFSP

#ifdef WINCE
#define SetDlgItemInputMode(hwnd,id,mode) SendDlgItemMessage(hwnd,id,EM_SETINPUTMODE,0,mode)
#else  // !WINCE
#define SetDlgItemInputMode(hwnd,id,mode)
#endif // !WINCE

#ifdef WIN32_PLATFORM_WFSP
#define SetDlgItemClearOnBackPressHoldExtendedStyle(hwnd,id) \
    SendDlgItemMessage(hwnd,id,EM_SETEXTENDEDSTYLE,\
	ES_EX_CLEARONBACKPRESSHOLD,ES_EX_CLEARONBACKPRESSHOLD)
#else // !WINCE
#define SetDlgItemClearOnBackPressHoldExtendedStyle(hwnd,id)
#endif // !WINCE

#define HANDLE_MSG(hwnd, message, fn)    \
    case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))

#ifdef WIN32_PLATFORM_WFSP
#define HANDLE_MSG_WM_HOTKEY(hwnd, fn) HANDLE_MSG(hwnd, WM_HOTKEY, fn)
#else // !WIN32_PLATFORM_WFSP
// Не на смартфоне необходимости в обработке WM_HOTKEY нет, т.к.
// только на смартфоне мы должны поймать нажатие VK_TBACK
#define HANDLE_MSG_WM_HOTKEY(hwnd, fn)
#endif // !WIN32_PLATFORM_WFSP

#ifdef WIN32_PLATFORM_PSPC
#define SipPreference(hwnd,state) SHSipPreference(hwnd,state)
#else //WIN32_PLATFORM_PSPC
#define SipPreference(hwnd,state)
#endif //WIN32_PLATFORM_PSPC

#if !(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
VOID CALLBACK TimerProcSetKBLang(
    HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#endif //!(defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* _READER_SUPPORT_SUP_WND_H */
