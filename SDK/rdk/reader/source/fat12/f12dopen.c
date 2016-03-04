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
 * \version $Revision: 75486 $
 * \date $Date: 2012-03-04 16:45:49 +0400 (Sun, 04 Mar 2012) $
 * \author $Author: dim $
 * \brief Функция открытия папки.
 */

#include "fat12prj.h"

/*!
 * \ingroup fat12_fun_reader
 * \brief Функция открытия папки.
 * \param context [in] контекст считывателя. 
 * \param info [in/out] структура #TReaderInfoFolderOpen
 * \sa TReaderInfoFolderOpen, READER_FUN_FOLDER_OPEN, 
 *  TSupSysFunction, fat12_folder_close, #rdr_folder_open, #rdr_folder_close
 */
TSupErr fat12_folder_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderOpen *inf = (TReaderInfoFolderOpen*)info;
    TCHAR *folder;
    TCHAR *path;
    TFat12Context *ctx = (TFat12Context*)context;
    int exist;
    size_t i;
    size_t j;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderOpen );
    SUPSYS_PRE( ctx->folder == NULL );

    inf->size_of = sizeof( TReaderInfoFolderOpen );

    if( !inf->name.text )
	return SUP_ERR_NO;
    if( inf->name.length > 12 )
	return SUP_ERR_PARAM;

    SUPSYS_PRE_READ_PTRS( inf->name.text, ( inf->name.length + 1 )
			  * sizeof( TCHAR ) );
    SUPSYS_PRE( !(inf->name.text [inf->name.length]) );
    SUPSYS_PRE( _tcslen( inf->name.text ) == inf->name.length );

    folder = inf->name.text;
    for( i = 0, j = 0; i < 12 && folder[i]; i++ )
    {
	if( folder[i] == '.' )
	{
	    if ( j != 0 )
		return SUP_ERR_PARAM;
	    j++;
	    continue;
	}
	if( j )
	    j++;
	if ( fat12_convert( folder[i] ) != folder[i] )
	    return SUP_ERR_PARAM;
    }
    if ( j > 4 )
	return SUP_ERR_PARAM;

    path = (TCHAR*)malloc( ( 12 + _tcslen( folder ) + 1
			     + 1 + _tcslen( ctx->path ) + 1
			     + 1 ) * sizeof( TCHAR ) );
    if( path == NULL )
	return SUP_ERR_MEMORY;
    ctx->folder = (TCHAR*)malloc( ( 12 + 1 ) * sizeof( TCHAR ) );
    if( ctx->folder == NULL )
    {
	free( path ); path = NULL;
	return SUP_ERR_MEMORY;
    }
    _tcscpy( path, ctx->path );
    _tcscat( path, folder );
    _tcscpy( ctx->folder, folder );
    /* Проверка корректности соединения. */
    exist = fat12_is_dir_exist( path );
    if( !inf->mode->o_creat && ( exist == DIR_INVALID || !exist ) )
    {
	free( path );
	free( ctx->folder ); ctx->folder = NULL;
	return RDR_ERR_FILE_NOT_FOUND;
    }
    if( inf->mode->o_creat && ( exist == DIR_INVALID || exist ) )
    {
	free( path );
	free( ctx->folder ); ctx->folder = NULL;
	return RDR_ERR_FILE_EXIST;
    }
    if( !inf->mode->o_creat )
    {
	free( path );
	return SUP_ERR_NO;
    }
#ifdef UNIX
    {
	TSupErr code = support_thread_actualize_uids ();
	if ( code )
	    return code;
    }
    exist = !_tmkdir( path, ((TFat12Context *)ctx)->machine?S_IRWXG|S_IRWXU:S_IRWXU )
	&& !chmod(path,((TFat12Context *)ctx)->machine?S_IRWXG|S_IRWXU:S_IRWXU);
    support_thread_deactualize_uids ();
    if( !exist )
#else
    if( !CreateDirectory( path, NULL ) )
#endif /* UNIX */
    {
	TSupErr code = fat12_os_error();
	free( path );
	free( ctx->folder ); ctx->folder = NULL;
	return code;
    }

    free( path );
    return SUP_ERR_NO;
}

#ifdef UNIX
int fat12_is_dir_exist( const TCHAR *path )
{ 
    struct stat buf;
    int rc; 
    if ( support_thread_actualize_uids () )
	return DIR_INVALID;
    rc = stat(path, &buf);
    support_thread_deactualize_uids ();
    if ( rc ) 
	return (errno==ENOENT)?DIR_NOTEXIST:DIR_INVALID;
    if (!S_ISDIR(buf.st_mode))
	return DIR_INVALID;
    return DIR_EXIST;
}
#elif defined _WIN32
int fat12_is_dir_exist( const TCHAR *path )
{
    DWORD attrib;
    attrib = GetFileAttributes( path ); 
    if( attrib == 0xffffffff ) 
	return DIR_NOTEXIST;
    if( !( attrib & FILE_ATTRIBUTE_DIRECTORY ) )
	return DIR_INVALID;
    if( attrib & ( FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_SYSTEM
	| FILE_ATTRIBUTE_HIDDEN ) )
	return DIR_INVALID;
    return DIR_EXIST;
}
#else
#error fat12_is_dir_exist not implemented.
#endif /* _WIN32, UNIX */
/* end of file: $Id: f12dopen.c 75486 2012-03-04 12:45:49Z dim $ */
