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
 * Функция закрытия файла. 
 ++++*/
TSupErr fat12_close( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TFat12Context *ctx = (TFat12Context*)context;
  TReaderInfoClose *inf = (TReaderInfoClose*)info;
  TSupErr code=SUP_ERR_NO;
  
  SUPSYS_PRE_INFO( info, TReaderInfoClose );
  
  inf->size_of = sizeof( TReaderInfoClose );
  if( context == NULL ) return SUP_ERR_NO;
  if( ctx->handle == NULL)
    return SUP_ERR_NO;
#ifdef UNIX
  if( fclose( ctx->handle ) ){
    code = fat12_os_error();
    if( !code ) code=RDR_ERR_INVALID_MEDIA;
  }
  ctx->handle = NULL;
#else /* UNIX */
  if( ctx->handle != INVALID_HANDLE_VALUE )
    {
      FILETIME time1;
      FILETIME time2;
      SYSTEMTIME cur_time;
      unsigned int last_error_mode;
      last_error_mode = 0;
      
      GetSystemTime( &cur_time );
      SystemTimeToFileTime( &cur_time, &time2 );
      GetFileTime( ctx->handle, &time1, NULL, NULL );
      if( ( ctx->access & GENERIC_WRITE )
	  && !SetFileTime( ctx->handle, &time1, &time2, &time2 ) ) {
	  code = fat12_os_error();
	  if( code != RDR_ERR_MEDIA_CHANGED ) goto done;
      }
#ifndef UNDER_CE 
      last_error_mode = SetErrorMode( 0 );
#endif
      CloseHandle( ctx->handle ); 
#ifndef UNDER_CE
      SetErrorMode( last_error_mode );
#endif
    }
done:
  ctx->handle = INVALID_HANDLE_VALUE;
#endif /* UNIX */
  free( ctx->file_name ); ctx->file_name = NULL;
  
  return code;
}

/*+ end of file: $Id: f12kcls.c 58819 2009-11-30 10:21:07Z cross $ +*/
