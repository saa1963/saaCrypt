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
 * \version $Revision: 23182 $
 * \date $Date: 2005-01-24 16:39:11 +0400 (Mon, 24 Jan 2005) $
 * \author $Author: cav $
 * \brief Функция вычисления длины файла.
 */

#include "ric_prj.h"

static TSupErr get_flen( const TCHAR *path, unsigned char file_id, size_t *len );

/*!
 * \brief Функция вычисления длины файла.
 */
TSupErr oscar2_file_length( 
    unsigned char file_id, 
    size_t *length, TCHAR* sconnect )
{
    TCHAR *connect = sconnect ? sconnect : DEFAULT_CONNECT;
    TSupErr err;
    TCHAR *path; 

    if( file_id == 0 || file_id > 6 )
	return SUP_ERR_VERIFY;

    path = malloc((_tcslen(OSCAR2_PATH_CFG) + 
	_tcslen(connect) + _tcslen(BASE_FILE_CFG) + 1) * sizeof(TCHAR));
    if(!path)
	return SUP_ERR_MEMORY;
    _tcscpy( path, OSCAR2_PATH_CFG );
    _tcscat( path, connect );
    _tcscat( path, BASE_FILE_CFG );

    // Только для этого файла
    if( file_id != 3 )
    {
	err = get_flen( path, file_id, length );
	free( path ); 
	if( err ) return err;
	return SUP_ERR_NO;
    }

    // Если файл - последний, то вычисляем длину, вычитая все остальные длины
    {
	size_t cur;
	err = get_flen( path, OSCAR2_FILE_NUMBERS[1][0], &cur );
	if( err ) { free( path ); return err; }
	*length -= cur;
    }
    free(path);
    return SUP_ERR_NO;
}

TSupErr get_flen( const TCHAR *path, unsigned char file_id, size_t *len )
{
    long param = 0;
    TCHAR *tst;
    TSupErr code;

    tst = malloc((_tcslen(path) + 3) * sizeof(TCHAR));
    if( !tst )
	return SUP_ERR_MEMORY;
    _sntprintf( tst, _tcslen( path ) + 3, _TEXT( "%s%1d" ), path, file_id );
    code = support_registry_get_long( tst, &param );
    free( tst );
    if( code )
	return code;
    if( param < 0 )
	return SUP_ERR_VERIFY;
    *len = (size_t)param;
    return SUP_ERR_NO;
}

/* end of file: $Id: osc2_flen.c 23182 2005-01-24 12:39:11Z cav $ */
