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
 * $Id: fat12prj.h 75486 2012-03-04 12:45:49Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Мастер include файл для проекта (READER/FAT12).
 *
 * Прямая зависимость от define: 
 *
 * Стандартных: _MSC_VER, _WIN32
 *
 * Определенных пользователем: _USRDLL
 *
 ****/

#if !defined( _READER_FAT12_FAT12PRJ_H )
#define _READER_FAT12_FAT12PRJ_H /*+ Запрещение двойного включения. +*/

#if defined( _MSC_VER )
#pragma once
#define WIN32_LEAN_AND_MEAN 
#endif /* defined( _MSC_VER ) */

#include "reader.kit/gen_prj.h"
#include "reader/dprint.h"

/*+ Standart. +*/
#if !defined _WIN32
#include <sys/stat.h> /*+ Std include. +*/
#include <errno.h> /*+ DOS include. +*/
#endif /* _WIN32 */

#ifdef SOLARIS
#include <volmgt.h>
#endif

#if !defined ( UNIX ) && !defined ( UNDER_CE ) 
#include <shlobj.h> 
#endif /* UNIX */

/*+ Project (READER/SUPPORT) +*/
#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"

/*+ Project (READER/RDR) +*/
#include "reader/rdr.h" /*+ Project (READER) include.
    Внешний интерфейс работы со считывателями. +*/
#include "reader.kit/reader.h" /*+ Project (READER/RDR) include.
    Определения необходимые для создания библиотеки считывателя. +*/

/*+ Project (READER/FAT12) +*/
#if defined( _USRDLL )
#define _FAT12_DECL _STDEXP_DECL
#define _HDIMAGE_DECL _STDEXP_DECL
#endif /* defined( _USRDLL ) */

#include "reader/fat12.h"
#include "reader/hdimage.h"

/*+ Project (READER/FAT12) внутренние include файлы. +*/
#include "fat12int.h" /*+ Project (READER/FAT12) include.
    Совместно используемые функции для реализации проекта FAT12. +*/
#include "fat12inf.h"
#include "hdimginf.h"

/*+ Project(READER/FAT12) include.
    Ресурсные константы. +*/
#ifdef UNIX
#include "fat12_rc.h" 
#else /* UNIX */
#include "Win32/fat12_rc.h" 
#endif /* UNIX */

#if defined( _USRDLL ) 
_HDIMAGE_DECL const TSupSysEHandle* reader_get_handle( size_t i );
#endif /* defined( _USRDLL ) */

#endif /* !defined( _READER_FAT12_FAT12PRJ_H ) */
/*+ end of file: $Id: fat12prj.h 75486 2012-03-04 12:45:49Z dim $ +*/
