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

#include "ric_prj.h"

#if defined( _RESOURCE )
/*!
 * \ingroup ric_internal
 * \brief INSTANCE DLL. 
 */
HINSTANCE RIC_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

/*!
 * \ingroup ric_internal
 * \brief Win32 точка входа в DLL. 
 * \param instance [in] DLL INSTANCE.
 * \param reason [in] тип входа.
 * \param reserved [in] статическая или динамическая загрузка. 
 * \return TRUE всегда.
 */
BOOL WINAPI DllMain(
    HINSTANCE instance,
    DWORD reason,
    LPVOID reserved )
{
    UNUSED( reserved );
    if( reason == DLL_PROCESS_ATTACH )
    {
	#if defined( _RESOURCE )
	RIC_DLL_INSTANCE = instance;
	#endif /* defined( _RESOURCE ) */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
    }
    return TRUE;
}
/* end of file: $Id: ric_dll.c 75491 2012-03-04 13:21:29Z dim $ */
