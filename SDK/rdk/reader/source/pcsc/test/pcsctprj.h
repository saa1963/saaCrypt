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

#if !defined( _READER_PCSC_TEST_PCSCTPRJ_H )
#define _READER_PCSC_TEST_PCSCTPRJ_H

#define TRUST_TEST
//#define OSCAR2_TEST
//#define OSCAR_TEST
//#define RIC_TEST
//#define ET_HS_TEST
//#define EMV_TEST

#include "reader.kit/gen_prj.h"

/* Project (READER). */
#include "reader/rdr.h"

/*+ Project (READER/sup_sys). +*/
#include "reader.kit/sup_sys.h"

#include "reader.kit/test/sstst.h"
/* Project (READER/RDR/TEST). */
#include "reader.kit/test/tst_rdr.h"

/* Project (READER/PCSC). */
#include "reader/pcsc.h"

#if defined( ET_HS_TEST )
#include "reader/et_hs.h"
#endif /* defined( ET_HS_TEST ) */
#if defined( RIC_TEST )
#include "reader/ric.h"
#endif /* defined( RIC_TEST ) */
#if defined( OSCAR_TEST )
#include "reader/oscar.h"
#endif /* defined( OSCAR_TEST ) */
#if defined( OSCAR2_TEST )
#include "reader/oscar.h"
#endif /* defined( OSCAR2_TEST ) */
#if defined( TRUST_TEST )
#include "reader/trust.h"
#endif /* defined( TRUST_TEST ) */
#if defined( EMV_TEST )
#include "reader/emv.h"
#endif /* defined( EMV_TEST ) */

#define DB_ERROR CONS
#define DB_TRACE CONS
#define DB_CALL NORM
#define DB_LOG CONS
#define DB_UNUSUAL EXCL
#include "reader/dprint.h"

#endif /* !defined( _READER_PCSC_TEST_PCSCTPRJ_H ) */
/* end of file:$Id: pcsctprj.h 54220 2009-05-02 02:33:10Z lse $ */
