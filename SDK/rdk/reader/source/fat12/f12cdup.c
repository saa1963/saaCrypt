/*
 * Copyright(C) 2000-2009 Проект ИОК
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

/*!
 * \ingroup fat12_info_general
 * \brief Функция дублирования контекста считывателя.
 * \return Код ошибки
 */
TSupErr fat12_context_dup( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoContextDup *inf = (TSupSysInfoContextDup*)info;
    TFat12Context *dest;
    TFat12Context *src = (TFat12Context*)context;

    /* Провека входных параметров. */
    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TSupSysInfoContextDup );

    dest = calloc(sizeof(TFat12Context),1);
    if( dest == NULL )
	return SUP_ERR_MEMORY;
    dest->path = NULL;
    if( src->path && ( src->flags & FAT12_FLAG_HDIMAGE ) )
    {
	dest->path = malloc( ( _tcslen( src->path ) + 1 ) * sizeof( TCHAR ) );
	if( !dest->path )
	{
	    free( dest );
	    return SUP_ERR_MEMORY;
	}
	_tcscpy( dest->path, src->path );
    }
    dest->path_to_item = NULL;
    if ( src->path_to_item )
    {
	dest->path_to_item = malloc( ( _tcslen( src->path_to_item ) + 1 ) * sizeof( TCHAR ) );
	if( !dest->path_to_item )
	{
	    if (dest->path)
	      free(dest->path);
	    free( dest );
	    return SUP_ERR_MEMORY;
	}
	_tcscpy( dest->path_to_item, src->path_to_item );

    }
    dest->volume_label = NULL;
    dest->volume_serial = (unsigned long)-1;
    dest->file_name = NULL;
    dest->folder = NULL;
    dest->flags = src->flags;
    _tcscpy( dest->nickname, src->nickname );
    _tcscpy( dest->connect, src->connect );

#ifdef UNIX
    dest->machine = 0;
    support_mutex_copy(&(dest->lock), src->lock);
    dest->n_lock = 0;
    dest->handle = NULL;
#else /* UNIX */
    dest->access = 0;
    dest->handle = INVALID_HANDLE_VALUE;
#endif /* UNIX */
    inf->dest_context = dest;
    return SUP_ERR_NO;
}
/* end of file: $Id: f12cdup.c 67029 2011-03-21 10:23:36Z dim $ */
