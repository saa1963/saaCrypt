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

TSupErr ric_error( unsigned char sw1, unsigned char sw2 )
{
    unsigned sw = sw1 << 8 | sw2;
    if( ( sw & 0xFFF0 ) == 0x63C0 )
    {
	if( sw & 0xf )
	    return RDR_ERR_INVALID_PASSWD;
	else
	    return RDR_ERR_PASSWD_LOCKED;
    }
    switch( sw )
    {
    case 0x65F0: return RDR_ERR_INVALID_MEDIA;
    case 0x6982: return RDR_ERR_NEED_PASSWD;
    case 0x6a82: return RDR_ERR_FILE_NOT_FOUND;
    }
    return SUP_ERR_VERIFY;
}
