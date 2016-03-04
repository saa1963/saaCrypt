#ifndef _COMCTRL2_H_INCLUDED
#define _COMCTRL2_H_INCLUDED

#ifdef _WIN32
// Уберем совсем предупреждение C4201
// (nonstandard extension used : nameless struct/union)
// т.к. оно активно используется в системных заголовках,
// а предназначенное для борьбы с ним лекарство - макрос NONAMELESSUNION
// не работает в полном объеме.
#pragma warning(disable:4201)
#include <windows.h>
#include <commctrl.h>

#ifdef WINCE
#include <aygshell.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#include <tpcshell.h> // Для SHSendBackToFocusWindow
#endif // defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)

// У Макса были проблемы со внешними ссылками на commctrl.dll и aygshell.dll
// Такие DLL-ки не получается загрузить в ядро (вроде в filesys.exe)
// а нам это очень нужно, т.к. неизвестен другой способ установки обходов.
// Поэтому здесь реализуется отложенная загрузка DLL (системного понятия 
// delayload на CE нет) путем реализации прокси-функций в этом файле.
//
// Известна еще слудующая проблема.
// На смартфоне почему-то функция SHSendBackToFocusWindow не загружается по имени,
// а грузится только через ordinal !!!
// (Для выяснения этого факта потеряно много времени).
//
// При отсутствии функции этот модуль теперь посылает исключение,
// а не маскирует ошибку, возвращая нули.

// Из DelayImp.cpp и DelayImp.h
#define FACILITY_VISUALCPP  ((LONG)0x6d)
#define VcppException(sev,err)  ((sev) | (FACILITY_VISUALCPP<<16) | err)

#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant

#define EXPORT_RESOLVE_MODULE(dll) __declspec(selectany) HMODULE h##dll = 0;
#define EXPORT_RESOLVE(dll,retval,conv,func,ordinal,argsdef,args,retmethod) \
typedef retval (conv * func##_t) argsdef; \
__declspec(selectany) func##_t pfn##func = NULL; \
__inline retval conv func##argsdef { \
    if (!pfn##func) { \
	if (!h##dll) { h##dll = LoadLibraryW(L#dll L".dll"); } \
	if (!h##dll) { \
	    MessageBox(0, L#dll L".dll load error", L"CryptoPro CSP", MB_ICONSTOP); \
	    DebugBreak(); RaiseException(VcppException( \
	    ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND), 0, 0, 0); } \
	pfn##func = (func##_t)GetProcAddressW(h##dll, L#func); \
	if (!pfn##func && (ordinal)>0) { \
	    pfn##func = (func##_t)GetProcAddressW(h##dll, \
	    (LPCWSTR)MAKEINTRESOURCE(ordinal)); } \
	if (!pfn##func) { \
	    MessageBox(0, L"Failed to find function " L#func L" in " L#dll L".dll", \
		L"CryptoPro CSP", MB_ICONSTOP); \
	    DebugBreak(); RaiseException(VcppException( \
	    ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND), 0, 0, 0); } \
    } \
    retmethod pfn##func##args; \
}

// commctrl.dll
EXPORT_RESOLVE_MODULE(commctrl)
EXPORT_RESOLVE(commctrl, HPROPSHEETPAGE, WINAPI, CreatePropertySheetPageW, 0,
    (IN LPCPROPSHEETPAGEW lppsph), (lppsph), return)
EXPORT_RESOLVE(commctrl, int, WINAPI, PropertySheetW, 0,
    (IN LPCPROPSHEETHEADERW lppsph), (lppsph), return)
EXPORT_RESOLVE(commctrl, void, WINAPI, InitCommonControls, 0, (void), (), (void))
EXPORT_RESOLVE(commctrl, BOOL, WINAPI, InitCommonControlsEx, 0,
    (IN LPINITCOMMONCONTROLSEX lpInitCtrls), (lpInitCtrls), return)

// aygshell.dll
EXPORT_RESOLVE_MODULE(aygshell)
EXPORT_RESOLVE(aygshell, BOOL, WINAPI, SHInitExtraControls, 0, (void), (), return)
EXPORT_RESOLVE(aygshell, BOOL, WINAPI, SHInitDialog, 0,
    (PSHINITDLGINFO pshidi), (pshidi), return)
EXPORT_RESOLVE(aygshell, BOOL, WINAPI, SHCreateMenuBar, 0,
    (SHMENUBARINFO * pmb), (pmb), return)
EXPORT_RESOLVE(aygshell, HWND, WINAPI, SHFindMenuBar, 0,
    (HWND hwnd), (hwnd), return)
EXPORT_RESOLVE(aygshell, void, WINAPI, SHNavigateBack, 0, (void), (), (void))

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
EXPORT_RESOLVE(aygshell, void, WINAPI, SHSendBackToFocusWindow, 97, // ordinal!!!
    (UINT uMsg, WPARAM wp, LPARAM lp), (uMsg, wp, lp), (void))
#endif // defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)

#ifdef WIN32_PLATFORM_PSPC
EXPORT_RESOLVE(aygshell, BOOL, WINAPI, SHSipPreference, 0,
    (HWND hwnd, SIPSTATE st), (hwnd,st), return)
#endif // WIN32_PLATFORM_PSPC

#pragma warning(push)

#else // !WINCE
#define SHInitExtraControls()
#endif // !WINCE
#else // !_WIN32
#define SHInitExtraControls()
#endif // _WIN32


#endif // _COMCTRL2_H_INCLUDED
