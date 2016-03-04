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
 * \version $Revision: 41056 $
 * \date $Date: 2007-05-30 00:41:40 +0400 (Wed, 30 May 2007) $
 * \author $Author: dim $
 * \brief Внешний интерфейс карты Оскар.
 */

#if !defined( READER_OSCAR_OSCAR_H )
#define READER_OSCAR_OSCAR_H

#include "reader/std_decl.h"
#include "reader/sup_syse.h"
#include "reader/rdr.h"

#if !defined( _OSCAR_DECL )
#if defined( OSCAR_IMPORTS )
#define _OSCAR_DECL _STDIMPL_DECL
#else /* defined( OSCAR_IMPORTS ) */
#define _OSCAR_DECL
#endif /* !defined( OSCAR_IMPORTS ) */
#endif /* !defined( _OSCAR_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** Функции. **** */
_OSCAR_DECL const TSupSysEHandle* oscar_media_get_handle(
    void );

_OSCAR_DECL const TSupSysEHandle* oscar2_media_get_handle(
    void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* READER_OSCAR_OSCAR_H */
/* end of file: $Id: oscar.h 41056 2007-05-29 20:41:40Z dim $ */
