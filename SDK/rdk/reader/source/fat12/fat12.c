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
 * $Id: fat12.c 75486 2012-03-04 12:45:49Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Совместно используемые функции для реализации проекта FAT12.
 *
 * Функции: fat12_*
 *
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*++++
 * Функция получения кода ошибки в формате драйвера по коду ошибки Windows.
 ++++*/
TSupErr fat12_os_error( void )
/*+
 * Возвращает: переведенный из Windows код ошибки.
 +*/
{
#if defined _WIN32
    DWORD w = GetLastError();
    SetLastError( 0 );
    switch( w )
    {
	/*+ 0 -> SUP_ERR_NO +*/
	case 0:
	    return SUP_ERR_NO;
	/*+ ERROR_FILE_NOT_FOUND -> RDR_ERR_FILE_NOT_FOUND +*/
	case ERROR_PATH_NOT_FOUND:
	case ERROR_FILE_NOT_FOUND:
	    return RDR_ERR_FILE_NOT_FOUND;
	/*+ ERROR_WRONG_DISK -> RDR_ERR_ANOTHER_MEDIA +*/
	case ERROR_WRONG_DISK:
	    return RDR_ERR_ANOTHER_MEDIA;
	/*+ ERROR_NOT_ENOUGH_MEMORY -> SUP_ERR_MEMORY +*/
	case ERROR_NOT_ENOUGH_MEMORY:
	    return SUP_ERR_MEMORY;
	/*+ ERROR_WRITE_PROTECT -> RDR_ERR_WRITE_PROTECT +*/
	case ERROR_WRITE_PROTECT:
	    return RDR_ERR_WRITE_PROTECT;
	/*+ ERROR_REQUEST_ABORTED, ERROR_NOT_READY -> RDR_ERR_NO_CARRIER +*/
	case ERROR_REQUEST_ABORTED:
	    return SUP_ERR_CANCEL;
	case ERROR_INVALID_HANDLE:
	case ERROR_NOT_READY:
	    return RDR_ERR_NO_CARRIER;
	/*+ ERROR_INVALID_DRIVE, ERROR_ACCESS_DENIED, ERROR_SHARING_VIOLATION
	ERROR_LOCK_VIOLATION, ERROR_DRIVE_LOCKED, ERROR_BUSY ->
	SUP_ERR_CONNECT +*/
	case ERROR_ACCESS_DENIED:
		return SUP_ERR_RIGHTS;
	case ERROR_INVALID_DRIVE:	
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
	case ERROR_DRIVE_LOCKED:
	case ERROR_BUSY:
	    return SUP_ERR_CONNECT;
	case ERROR_FILE_INVALID:
	    return RDR_ERR_MEDIA_CHANGED;
	case ERROR_DISK_FULL:
	case ERROR_HANDLE_DISK_FULL:
	    return RDR_ERR_FULL;
	case ERROR_ALREADY_EXISTS:
	case ERROR_FILE_EXISTS:
	    return RDR_ERR_FILE_EXIST;
    }
    /*+ otherwise  -> RDR_ERR_INVALID_MEDIA+*/
    return RDR_ERR_INVALID_MEDIA;
#else /* _WIN32 */
    switch( errno )
    {
      /*+ 0 - SUP_ERR_NO +*/
    case 0:
      return SUP_ERR_NO;
      
      /*+ ENOMEM, ECONTR, EINVMEM - SUP_ERR_MEMORY +*/
    case ENOMEM:
#ifndef UNIX
    case ECONTR:
    case EINVMEM:
#endif /* UNIX */
      return SUP_ERR_MEMORY;
      
      /*+ ENOFILE, ENOPATH, EACCES, ENOTSAM, ENOENT - RDR_ERR_NO_CARRIER +*/
#ifdef UNIX
    case ENOENT:
    case ENOTDIR:
    case EISDIR:
#else /* UNIX */
    case ENOFILE:
    case ENOPATH:
    case ENOTSAM:
#endif /* UNIX */
    case EACCES:
      return RDR_ERR_NO_CARRIER;
      
      /*+ EINVDRV, EINVACC - SUP_ERR_CONNECT +*/
#ifdef UNIX
    case EMFILE:
    case ENOSPC:
    case ENFILE:
#else /* UNIX */  
    case EINVDRV:
    case EINVACC:
#endif /* UNIX */
      return SUP_ERR_CONNECT;
    }
    return RDR_ERR_INVALID_MEDIA;
    /*+ otherwise  -> RDR_ERR_INVALID_MEDIA+*/
#endif /* _WIN32 */
}

/*+ end of file: $Id: fat12.c 75486 2012-03-04 12:45:49Z dim $ +*/
