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

#if !defined( _READER_FILE_FAT12_TEST_TST12PRJ_H )
#define _READER_FILE_FAT12_TEST_TST12PRJ_H /*+ Запрещение двойного включения. +*/

#if defined( _MSC_VER )
#pragma once
#define WIN32_LEAN_AND_MEAN /*+ Exclude rarely-used stuff from Windows headers +*/
#endif /* defined( _MSC_VER ) */

#include "reader.kit/gen_prj.h"

/*+ Project (READER) +*/
#include "reader/rdr.h"

/*+ Project (READER/sup_sys). +*/
#include "reader.kit/sup_sys.h"

/* Project (READER/SUPPORT/TEST). */
#include "reader.kit/test/sstst.h"
/* Project (READER/RDR/TEST). */
#include "reader.kit/test/tst_rdr.h"

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_LOG CONS
#include "reader/dprint.h"

#endif /* !defined( _READER_FILE_FAT12_TEST_TST12PRJ_H ) */
/*+ end of file:tst12prj.h +*/
