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

/****
 * $Id: fat12dll.c 75487 2012-03-04 12:47:53Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Точка входа для Win32 DLL.
 *
 * Функция: DllMain
 *
 * Константа: FAT12_DLL_INSTANCE
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*+ INSTANCE DLL. +*/
#if defined( _RESOURCE )
HINSTANCE FAT12_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

///*++++
// * Win32 точка входа в DLL. 
// +++++*/
//BOOL WINAPI DllMain(
//    HINSTANCE, /*+ (i) DLL INSTANCE. +*/
//    DWORD, /*+ (i) Тип входа. +*/
//    LPVOID ); /*+ (i) Статическая или динамическая загрузка. +*/

/*++++
 * Win32 точка входа в DLL. 
 +++++*/
BOOL WINAPI DllMain(
    HINSTANCE instance, /*+ (i) DLL INSTANCE. +*/
    DWORD reason, /*+ (i) Тип входа. +*/
    LPVOID reserved ) /*+ (i) Статическая или динамическая загрузка. +*/
/*+
 * Возвращает: 
 *
 * TRUE, всегда
 +*/
{
    UNUSED( reserved );
    if( reason == DLL_PROCESS_ATTACH )
    {
	#if defined( _RESOURCE )
	FAT12_DLL_INSTANCE = instance;
	#endif /* defined( _RESOURCE ) */
#if !defined (UNDER_CE) 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	
	DisableThreadLibraryCalls( instance ); 
    }
    return TRUE;
}
/*+ end of file:$Id: fat12dll.c 75487 2012-03-04 12:47:53Z dim $ +*/
