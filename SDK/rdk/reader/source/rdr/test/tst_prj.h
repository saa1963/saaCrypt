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
 * $Id: tst_prj.h 75490 2012-03-04 13:04:37Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 * Интерфейс ко всем считывателям (RDR).
 * Библиотека тестирования (TEST).
 * Мастер include файл для проекта (READER/RDR/TEST).
 *
 * Прямая зависимость от define:
 *
 * Стандартных: Win32
 ****/

#if !defined( _READER_RDR_TEST_TST_PRJ_H )
#define _READER_RDR_TEST_TST_PRJ_H /*+ Запрещение двойного включения. +*/

#define _READER_TEST_DECL _STDEXP_DECL

#include "reader.kit/gen_prj.h"

/*+ Project (READER). +*/
#include "reader/rdr.h" /*+ Project (READER) include.
    Внешний интерфейс работы со считывателями. +*/

/*+ Project (READER/sup_sys). +*/
#include "reader.kit/sup_sys.h"

/*+ Project (READER/RDR/TEST). +*/
#include "reader.kit/test/tst_rdr.h" /*+ Project (READER/RDR/TEST) include.
    Функции тестирования считывателя. +*/

/*+ Project (READER/RDR/TEST) внутренние include файлы. +*/
/*+ Project (READER/RDR/TEST) include.
    Ресурсные константы. +*/
#include "reader/sup_syse.h"
#include "reader.kit/test/sstst.h"

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_CALL NORM
#define DB_LOG CONS
#define DB_UNUSUAL EXCL
#include "reader/dprint.h"

extern TSupportDbContext *support_context_io;

#endif /* !defined( _READER_RDR_TEST_TST_PRJ_H ) */
/*+ end of file:tst_prj.h +*/
