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
 *
 * \brief Краткое описание модуля.
 *
 * Проект: Датчик случайных чисел: 0 последовательность(RNDM0).
 *
 * Внешний интерфейс ДСЧ.
 *
 * Функция: rndm0_rndm_get_handle
 */

#if !defined( _READER_RNDM_RNDM0_R0_EXP_H )
#define _READER_RNDM_RNDM0_R0_EXP_H 

#include "reader/std_decl.h"
#include "reader/rndm.h" 

#if !defined( _RNDM0_DECL )
#if defined( RNDM0_IMPORTS )
#define _RNDM0_DECL _STDIMPL_DECL
#else /* defined( RNDM0_IMPORTS ) */
#define _RNDM0_DECL
#endif /* !defined( RNDM0_IMPORTS ) */
#endif /* !defined( _RNDM0_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* Получение точки входа в библиотеку счиытвателя. */
_RNDM0_DECL const TRndmHandle* rndm0_rndm_get_handle( void );

/* Получение точки входа в библиотеку счиытвателя. */
_RNDM0_DECL const TRndmHandle* fapsi0_rndm_get_handle( void );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* defined( _READER_RNDM_RNDM0_R0_EXP_H ) */
/* end of file:$Id: r0.h 41056 2007-05-29 20:41:40Z dim $ */
