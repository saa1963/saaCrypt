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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief Внешний интерфейс карты Магистра (ОС Trust).
 */

#if !defined( READER_TRUST_H )
#define READER_TRUST_H

#include "reader/std_decl.h"
#include "reader/sup_syse.h"
#include "reader/rdr.h"

#if !defined( _TRUST_DECL )
#if defined( TRUST_IMPORTS )
#define _TRUST_DECL _STDIMPL_DECL
#else /* defined( TRUST_IMPORTS ) */
#define _TRUST_DECL
#endif /* !defined( TRUST_IMPORTS ) */
#endif /* !defined( _TRUST_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* **** Функции. **** */
_TRUST_DECL const TSupSysEHandle* trustd_media_get_handle(
    void );

_TRUST_DECL const TSupSysEHandle* trust_media_get_handle(
    void );

_TRUST_DECL const TSupSysEHandle* trusts_media_get_handle(
    void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* READER_TRUST_H */
/* end of file: $Id: trust.h 54220 2009-05-02 02:33:10Z lse $ */
