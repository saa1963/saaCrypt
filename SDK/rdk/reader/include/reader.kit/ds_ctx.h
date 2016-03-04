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
 * \version $Revision: 36705 $
 * \date $Date: 2006-10-23 20:46:50 +0400 (Mon, 23 Oct 2006) $
 * \author $Author: cav $
 * \brief Интерфейс таблеток DS19xx.
 */

#if !defined( _READER_DS_DS_CTX_H )
#define _READER_DS_DS_CTX_H

#include "reader.kit/reader.h"

#define DALLAS_FUN_IS_SAME_SER_NUM 0x2500

#define DALLAS_RESTORE_FLAG_HARD 0x1

typedef TSupErr (*DSReadFun)( TSupSysContext *context, size_t from, 
    size_t *length, unsigned char *buffer );

typedef TSupErr (*DSWriteFun)( TSupSysContext *context, size_t from, 
    size_t *length, const unsigned char *buffer );

typedef struct TDSInfoIsSame_
{
    size_t size_of;
    unsigned char unique[8];
} TDSInfoIsSame;

typedef struct TDSInfoConnectCarrier_
{
    size_t size_of;
    unsigned char unique[8];
    TSupSysContext *context;
    DSReadFun read_proc;
    DSWriteFun write_proc;
    unsigned flags;
} TDSInfoConnectCarrier;

typedef struct TDSInfoUnique_
{
    size_t size_of;
    unsigned char unique[8];
    TSupSysInfoText unique_str;
} TDSInfoUnique;

typedef TSupSysInfoDummy TDSInfoDisconnectCarrier;

typedef struct TDSInfoRestoreContext_
{
    TReaderInfoRestoreContext *base;
    unsigned flags;
} TDSInfoRestoreContext;

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_DS_DS_H ) */
/* end of file: $Id: ds_ctx.h 36705 2006-10-23 16:46:50Z cav $ */
