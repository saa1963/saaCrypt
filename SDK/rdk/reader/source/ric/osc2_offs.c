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

TSupErr oscar2_file_offset(
    unsigned char file_id, size_t *offset, TCHAR *connect)
{
    const unsigned char *c;

    *offset = 0;
    if( file_id == 0 || file_id > 6 )
	return SUP_ERR_VERIFY;
    c = OSCAR2_FILE_NUMBERS[OSCAR2_FILE_INDEX[file_id - 1]];
    for( ; file_id != *c; c++ )
    {
	size_t len = 0;
	TSupErr err = oscar2_file_length( *c, &len, connect);
	if( err )
	    return err;
	*offset += len;
    }
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2_offs.c 23182 2005-01-24 12:39:11Z cav $ */
