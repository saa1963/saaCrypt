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

#include "r0_prj.h" /*+ Project (READER/RNDM/RNDM0) include.
    Мастер include файл для проекта (READER/RNDM/RNDM0). +*/

#if defined( _RESOURCE )
/*! \ingroup rndm0_int 
 * \brief DLL Instance. 
 *
 * Instance библиотеки ДСЧ 0.
 */
HINSTANCE RNDM0_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

/*! \ingroup rndm0_int
 * \brief Win32 точка входа в DLL. 
 *
 * Инициализация / деинициализация критичекой секции, instance, 
 *  common controls.
 * \param instance [in] DLL INSTANCE
 * \param reason [in] тип входа
 * \param reserved [in] статическая или динамическая загрузка
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
	RNDM0_DLL_INSTANCE = instance;
	#endif /* defined( _RESOURCE ) */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
	return TRUE;
    }
    return TRUE;
}
/* end of file: $Id: rndm0dll.c 75490 2012-03-04 13:04:37Z dim $ */
