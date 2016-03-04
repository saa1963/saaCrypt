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
 * \brief Внешний интерфейс карты РИК.
 */

#if !defined( READER_RIC_RIC_H )
#define READER_RIC_RIC_H

#include "reader/std_decl.h"
#include "reader/sup_syse.h"
#include "reader/rdr.h"

#if !defined( _RIC_DECL )
#if defined( RIC_IMPORTS )
#define _RIC_DECL _STDIMPL_DECL
#else /* defined( RIC_IMPORTS ) */
#define _RIC_DECL
#endif /* !defined( RIC_IMPORTS ) */
#endif /* !defined( _RIC_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** Функции. **** */
_RIC_DECL const TSupSysEHandle* ric_media_get_handle(
    void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* READER_RIC_RIC_H */
/* end of file: $Id: ric.h 41056 2007-05-29 20:41:40Z dim $ */
