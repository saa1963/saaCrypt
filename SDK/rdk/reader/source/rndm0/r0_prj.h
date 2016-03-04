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
 * $Id: r0_prj.h 75332 2012-03-01 10:03:55Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Интерфейс к датчикам случайных чисел (RNDM).
 *
 * Датчик случайных чисел: постоянная последовательность (RNDM0).
 *
 * Мастер include файл для проекта (READER/RNDM/RNDM0).
 *
 * Только для реализации проекта (READER/RNDM/RNDM0).
 *
 * Прямая зависимость от define: 
 *
 * Стандартных: _WIN32
 ****/

#if !defined _READER_RNDM_RNDM0_R0_PRJ_H
#define _READER_RNDM_RNDM0_R0_PRJ_H 

#include "reader.kit/gen_prj.h"

#include "reader/rndm.h" /* Project (READER/RNDM) include.
    Интерфейс к ДСЧ. */
#include "reader.kit/rndm_sys.h" /* Project (READER/RNDM) include.
    Определения необходимые для создания ДСЧ. */

/* Project (READER/RNDM/RNDM0) */
#if defined( _USRDLL )
#define _RNDM0_DECL _STDEXP_DECL
#endif /* defined( _USRDLL ) */

#include "reader/r0.h" /* Project (READER/RNDM/RNDM0) include.
    Интерфейс к ДСЧ 0. */
#if !defined _WIN32
#include "rndm0_rc.h"
#endif /* _WIN32 */

#if defined( _RESOURCE ) || !defined ( SUPPORT_RESOURCE_STD )
extern HINSTANCE RNDM0_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

#endif /* !_READER_RNDM_RNDM0_R0_PRJ_H */
