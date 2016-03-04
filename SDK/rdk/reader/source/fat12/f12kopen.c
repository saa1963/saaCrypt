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

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*++++
 * Функция открытия файла. 
 ++++*/
TSupErr fat12_open( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TCHAR *fname;
    TReaderInfoOpen *inf = (TReaderInfoOpen*)info;
    TFat12Context *ctx = (TFat12Context*)context;
    TReaderInfoOpenFlags *mode;
    TSupErr code;

#ifdef UNIX
    char str_mode[3];
#else
    DWORD dwCreationDisposition = 0;
#endif
    
    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoOpen );
    SUPSYS_PRE( ctx->file_name == NULL );
    SUPSYS_PRE_READ_PTRS( inf->mode, sizeof( TReaderInfoOpenFlags ) );
    SUPSYS_PRE( inf->mode_bits >= READER_MODEFLAG_BITS );
    SUPSYS_PRE( inf->name.length );
    SUPSYS_PRE_STRING_PTRS( inf->name.text, inf->name.length );
#ifdef UNIX
    SUPSYS_PRE( ctx->handle == NULL );
#else
    SUPSYS_PRE( ctx->handle == INVALID_HANDLE_VALUE );
#endif
    SUPSYS_PRE( ctx->path != NULL );
     
    inf->size_of = sizeof( TReaderInfoOpen );
    fname = inf->name.text;
    ctx->file_name = malloc( ( _tcslen( fname ) + 1 + _tcslen( ctx->path ) 
	+ 2 + ( ctx->folder ? _tcslen( ctx->folder ) : 0 ) + 1 )
	* sizeof( TCHAR ) );
    
    if( ctx->file_name == NULL ) return SUP_ERR_MEMORY;
    _tcscpy( ctx->file_name, ctx->path );
    if( ctx->folder ) 
    {
	_tcscat( ctx->file_name, ctx->folder );
	_tcscat( ctx->file_name, _TEXT( "/" ) );
    }
    _tcscat( ctx->file_name, fname );
    mode = inf->mode;
#ifdef UNIX
    str_mode[2]=str_mode[1]='\0';
    
#if 0
    r  +  +  +  -  -
	w  +  +  -  +  +
	c  +  -  ?  +  -
	mode w+ r+ r  w  r+
#endif
	
	if(mode->o_write){
	    str_mode[0]=((mode->o_creat)?'w':'r');
	    if(mode->o_read||!mode->o_creat) str_mode[1]='+';
	} else
	    str_mode[0]='r';

	code = support_thread_actualize_uids ();
	if ( SUP_ERR_NO != code )
	{
	    free( ctx->file_name ); ctx->file_name = NULL;
	    ctx->handle = NULL;
	    return code;
	}
	ctx->handle = fopen( ctx->file_name, str_mode);
	support_thread_deactualize_uids ();
	/* free( file_name ); */
	/* Проверить на Write Protect */
	/* Проверить на наличие свободного места. */
	if( ctx->handle == NULL ){
	    code = fat12_os_error();
	    free( ctx->file_name ); ctx->file_name = NULL;
	    ctx->handle = NULL;
	    if( errno == ENOENT ) return RDR_ERR_FILE_NOT_FOUND;
	    return code;
	}
	if(fchmod(fileno(ctx->handle),(((TFat12Context *)context)->machine)?S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP:S_IRUSR|S_IWUSR )){
	    code = fat12_os_error();
	    free( ctx->file_name ); ctx->file_name = NULL;
	    fclose(ctx->handle);
	    ctx->handle = NULL;
	    return code;
	}

#else /* UNIX */
    ctx->access = 0;
    if( mode->o_read )
	ctx->access |= GENERIC_READ;
    if( mode->o_write )
	ctx->access |= GENERIC_WRITE;
    if( mode->o_creat )
	dwCreationDisposition |= CREATE_ALWAYS;
    else
	dwCreationDisposition |= OPEN_EXISTING;
    
    ctx->handle = CreateFile( ctx->file_name, ctx->access,
	FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
	dwCreationDisposition, FILE_ATTRIBUTE_ARCHIVE, NULL );
    if( ctx->handle == INVALID_HANDLE_VALUE ){
	code = fat12_os_error();
	free( ctx->file_name ); ctx->file_name = NULL;
	return code; 
    }
#ifndef WINCE
    // То ли вызов FlushFileBuffers непостредственно после CreateFile
    // на CE приводит к ошибке ERROR_NOT_SUPPORTED, то ли сама функция 
    // FlushFileBuffers всегда возвращает ERROR_NOT_SUPPORTED.
    if( mode->o_write )
    {
	if( !FlushFileBuffers( ctx->handle ) )
	{
	    code = fat12_os_error();
	    CloseHandle( ctx->handle );
	    free( ctx->file_name ); ctx->file_name = NULL;
	    return code; 
	}
    }
#endif //WINCE

#endif /* UNIX */
	
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kopen.c 73857 2012-01-18 10:55:45Z givf $ +*/
