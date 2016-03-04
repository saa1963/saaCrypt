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

#if !defined( _READER_FAT12_FAT12EXP_H )
#define _READER_FAT12_FAT12EXP_H

#include "reader/std_decl.h"
#include "reader/rdr.h"

#if !defined( _FAT12_DECL )
#if defined( FAT12_IMPORTS )
#define _FAT12_DECL _STDIMPL_DECL
#else /* defined( FAT12_IMPORTS ) */
#define _FAT12_DECL
#endif /* !defined( FAT12_IMPORTS ) */
#endif /* !defined( _FAT12_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

_FAT12_DECL const TSupSysEHandle *fat12_reader_get_handle( void );
_FAT12_DECL const TSupSysEHandle *default_reader_get_handle( void );
_FAT12_DECL const TSupSysEHandle *fat12_reader_group_handle( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_FAT12_FAT12EXP_H ) */
/*+ end of file:$Id: fat12.h 54220 2009-05-02 02:33:10Z lse $ +*/
