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

#include "fat12prj.h" /* Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). */

/*++++ 
 * Функция чтения из файла. 
 ++++*/
TSupErr fat12_read( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoRead *inf = (TReaderInfoRead*)info;
    TFat12Context *ctx = (TFat12Context*)context;

#ifdef UNIX
    size_t loc_read = 0;
    TSupErr code;
#else /* UNIX */
    DWORD loc_read = 0;
    void *buffer = (void*)inf->info.info;
#endif /* UNIX */

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoRead );
    SUPSYS_PRE_WRITE_PTRS( inf->info.info, inf->info.length );

    inf->size_of = sizeof( TReaderInfoRead );
#ifdef UNIX
    if( fseek( ctx->handle, inf->from, SEEK_SET ) )
	return fat12_os_error();
    errno=0;
    loc_read = fread( inf->info.info, 1, inf->info.length, ctx->handle );
    if((code = fat12_os_error()))
	return code;
#else /* UNIX */
    if( ctx->handle == INVALID_HANDLE_VALUE )
	return RDR_ERR_MEDIA_CHANGED;
    SUPSYS_PRE( inf->from <= MAXLONG );
    if( SetFilePointer( ctx->handle, (long)inf->from, NULL, 
	FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	return fat12_os_error(); 
    SUPSYS_PRE( inf->info.length <= MAXLONG );
    if( !ReadFile( ctx->handle, buffer, (long)inf->info.length, &loc_read, 
	NULL ) )
	return fat12_os_error();
#endif /* UNIX */
    inf->info.length -= loc_read;
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kread.c 44147 2008-01-10 14:18:38Z dim $ +*/
