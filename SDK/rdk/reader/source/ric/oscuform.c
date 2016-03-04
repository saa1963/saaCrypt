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

#include "ric_prj.h"

static const TCHAR UNIQUE_PREFIX[] = _TEXT( "OSCAR_%02X%02X%02X%02X" );

/* Функция получения уникального номера. */
TSupErr oscar_unique_form( unsigned char unique_num[4], TCHAR *unique )
{
    _sntprintf( unique, MAX_UNIQUE_LENGTH + 1, UNIQUE_PREFIX, unique_num[0], unique_num[1], 
	unique_num[2], unique_num[3] );
    return SUP_ERR_NO;
}
/* end of file: $Id: oscuform.c 66111 2011-02-03 15:03:44Z maxdm $ */
