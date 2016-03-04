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

#if !defined( _READER_HDIMAGE_HDIMGEXP_H )
#define _READER_HDIMAGE_HDIMGEXP_H

#include "reader/std_decl.h"
#include "reader/rdr.h"

#if !defined( _HDIMAGE_DECL )
#if defined( HDIMAGE_IMPORTS )
#define _HDIMAGE_DECL _STDIMPL_DECL
#else /* defined( HDIMAGE_IMPORTS ) */
#define _HDIMAGE_DECL
#endif /* !defined( HDIMAGE_IMPORTS ) */
#endif /* !defined( _HDIMAGE_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** Функции. **** */

_HDIMAGE_DECL const TSupSysEHandle* hdimage_reader_get_handle(
    void );

_HDIMAGE_DECL const TSupSysEHandle* hsm_reader_get_handle(
    void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_HDIMAGE_HDIMGEXP_H ) */
/* end of file:$Id: hdimage.h 42536 2007-09-26 13:50:44Z chudov $ */
