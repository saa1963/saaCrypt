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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief Функция получения пути к ключам по умолчанию.
 */

#include "fat12prj.h"

static const TCHAR LOCAL_PATH[] = _TEXT( "\\protected_local" );
static const TCHAR GLOBAL_PATH[] = _TEXT( "\\protected_global" );

TSupErr hdimage_path_default( 
    TCHAR **str, int machine )
{
    size_t length = 0;
    TCHAR *path;
    const TCHAR *p;
    TSupErr code;

    if( machine & 1 )
	p = GLOBAL_PATH;
    else
	p = LOCAL_PATH;
    code = support_path2dir( p, &length, NULL );
    if( code )
	return code;
    path = malloc( ( length + 2 ) * sizeof( TCHAR ) );
    if( path == NULL )
	return SUP_ERR_MEMORY;
    code = support_path2dir( p, &length, path );
    if( code )
    {
	free( path );
	return code;
    }
#ifdef UNIX
    if (!(machine & 1))
    {
	code = support_thread_actualize_uids();
	if (code)
	{
	    free(path);
	    return code;
	}
        if (chmod(path,S_IRWXU)) {
	    code = fat12_os_error();
	    support_thread_deactualize_uids();
	    free( path );
	    return code;
	}
	support_thread_deactualize_uids();
    }
#endif /* UNIX */
    _tcscat(path,_TEXT("/"));
    *str = path;

    return SUP_ERR_NO;
}
/* end of file: $Id: hdidef.c 54220 2009-05-02 02:33:10Z lse $ */
