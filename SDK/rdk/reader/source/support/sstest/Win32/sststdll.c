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

#include "sststprj.h"

#if defined( _RESOURCE )
/*+ INSTANCE DLL. +*/
HINSTANCE SSTEST_RESOURCE;
#endif /* defined( _RESOURCE ) */

TSupportDbContext *support_context_io = NULL;


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
	support_context_io = support_print_init( NULL, DTEXT( "sstest" ),
	    SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );

	#if defined _RESOURCE
	SSTEST_RESOURCE = instance;
	#endif /* _RESOURCE */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
    }
    return TRUE;
}
/*+ end of file: $Id: sststdll.c 61962 2010-05-24 09:22:53Z dim $ +*/
