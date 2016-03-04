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
 * \brief Дублирование контекста карты.
 */

#include "ric_prj.h"

/*!
 * \brief Дублирование контекста карты.
 */
TSupErr trust_context_dup( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoContextDup *inf = (TSupSysInfoContextDup*)info;
    TRICContext *dest;
    TRICContext *src = (TRICContext*)context;

    SUPSYS_PRE_INFO( info, TSupSysInfoContextDup );
    SUPSYS_PRE_CONTEXT( context, TRICContext );

    inf->size_of = sizeof( TSupSysInfoContextDup );
    dest = malloc( sizeof( TRICContext ) );
    if( dest == NULL )
	return SUP_ERR_MEMORY;
    dest->reader = NULL;
    dest->apdu = NULL;
    dest->status = RIC_STATUS_START;
    dest->file = 0;
    dest->file_length = 0;
    dest->connect = 0;
    dest->unique_length = src->unique_length;
    dest->unique_form = src->unique_form;
    dest->change_pin = src->change_pin;
    dest->block_len_read = src->block_len_read;
    dest->block_len_write = src->block_len_write;
    dest->class_tag = src->class_tag;
    dest->application = src->application;
    //TODO: dest->folder == dest->application; dest->folder = NULL
    memset( dest->pin, 0, 8 );
    memset( dest->unique, 0, 4 );
    inf->dest_context = (TSupSysEContext*)dest;
    return SUP_ERR_NO;
}
/* end of file: $Id: mag_dup.c 54220 2009-05-02 02:33:10Z lse $ */
