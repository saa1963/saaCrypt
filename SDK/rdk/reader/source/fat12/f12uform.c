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

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*++++
 * Функция формирования уникального номера.
 ++++*/
TSupErr fat12_unique_form(
    unsigned long volume_serial,
    const TCHAR *vname,
    size_t *length,
    TCHAR *unique )
{
    size_t l = 0;
    int i;
    TCHAR volume_name[11 + 2]; /* Метка тома. */
    size_t len; /* Требуемая длина для получения уникального идентификатора. */

    _tcsncpy( volume_name, vname, 11 );
    volume_name[11] = 0;
    if( vname == NULL )
    {
	*length = 1 + 8 + 1 + 11;
	return SUP_ERR_NO;
    }
    len = 8 + 1 + _tcslen( volume_name ) + 1;
    if( unique == NULL )
    {
	*length = len;
	return SUP_ERR_NO;
    }
    for( i = 4 * 7; i >= 0 && l < *length; i -= 4, l++, unique++ )
    {
	unsigned char j = (unsigned char)( ( volume_serial >> i ) & 0xf );
	if( j > 9 )
	    *unique = (TCHAR)( 'A' + j - 10 );
	else
	    *unique = (TCHAR)( '0' + j );
    }
    if( *length <= l )
    {
	*length = len;
	return SUP_ERR_NO;
    }
    if( !*volume_name )
    {
	*unique = 0;
	*length = len;
	return SUP_ERR_NO;
    }
    *unique++ = '_'; l++; 
    if( *length <= l )
    {
	*length = len;
	return SUP_ERR_NO;
    }
    _tcsncpy( unique, volume_name, *length - l );
    unique[*length - l] = 0;
    *length = len;
    return SUP_ERR_NO;
}
/* end of file: $Id: f12uform.c 44149 2008-01-10 14:25:34Z dim $ */
