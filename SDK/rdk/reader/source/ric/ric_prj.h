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
 * \version $Revision: 75355 $
 * \date $Date: 2012-03-01 17:24:46 +0400 (Thu, 01 Mar 2012) $
 * \author $Author: dim $
 * \brief Precompile header.
 */

#if !defined( _READER_RIC_RIC_PRJ_H )
#define _READER_RIC_RIC_PRJ_H

#include "reader.kit/gen_prj.h"

#if defined( _USRDLL )
#define _RIC_DECL _STDEXP_DECL
#define _OSCAR_DECL _STDEXP_DECL
#define _TRUST_DECL _STDEXP_DECL
#endif /* defined( _USRDLL ) */

/* Project (READER/SUPPORT). */
#include "reader/sup_syse.h"

/* Project (READER/RDR). */
#include "reader/rdr.h"
#include "reader.kit/reader.h" 

/* Project (READER/SCARD) */
#include "reader.kit/scard.h"

#include "reader/ric.h"
#include "reader/oscar.h"
#include "reader/trust.h"
#include "ric_int.h"

#if defined _WIN32
#include "reader.kit/supwsys.h"
#else
#include "ric_rc.h"
#endif /* _WIN32 */

#endif /* !defined( _READER_RIC_RIC_PRJ_H ) */
/* end of file:$Id: ric_prj.h 75355 2012-03-01 13:24:46Z dim $ */
