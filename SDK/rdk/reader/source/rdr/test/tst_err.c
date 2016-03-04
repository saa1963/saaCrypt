/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

/****
 * $Id: tst_err.c 36825 2006-10-25 15:30:04Z cav $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������������ (TEST).
 *
 * ������� ������ ��������� �� ������.
 *
 * �������: test_error
 *
 * ������ ����������� �� define: 
 *
 * �����������: 
 *
 * ������������ �������������:
 *
 ****/

#include "tst_prj.h" /*+ Project (READER/RDR/TEST) include.
    include ���� ��� ������� (READER/RDR/TEST). +*/

/*++++
 * ������� ������ ��������� �� ������.
 ++++*/
TSupErr test_error( 
    const TSupSysEContext *context, /*+ Handle �����������, ����� ���� NULL. +*/
    TSupErr code ) /*+ (i) ��� ������. +*/
{
#if 0
    static const TCHAR con_error_memory[] = _TEXT( "**Fatal error** Not enough memory." );
    static const TCHAR con_error_verify[] = _TEXT( "**Fatal error** Verification failed." );
#endif
    TCHAR *text;
    size_t size = 0;
    TSupErr error;

    if( code == SUP_ERR_CANCEL )
	return SUP_ERR_CANCEL;
    error = rdr_error( context, code, &size, NULL );
    if( error == SUP_ERR_MEMORY )
    {
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "**Fatal error** Not enought memory.")));
	return SUP_ERR_MEMORY;
    }
    if( error == SUP_ERR_VERIFY )
    {
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "**Fatal error** Verification failed.")));
	return SUP_ERR_VERIFY;
    }
    text = malloc( ( size + 1 ) * sizeof( TCHAR ) );
    if( text == NULL )
    {
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "**Fatal error** Not enought memory.")));
	return SUP_ERR_MEMORY;
    }
    error = rdr_error( context, code, &size, text );
    if( error == SUP_ERR_MEMORY )
    {
	free( text );
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "**Fatal error** Not enought memory.")));
	return SUP_ERR_MEMORY;
    }
    if( error == SUP_ERR_VERIFY )
    {
	free( text );
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "**Fatal error** Verification failed.")));
	return SUP_ERR_VERIFY;
    }
    
    DbTrace(DB_ERROR, (FTEXT(support_context_io, "Error: %s"), text));    
    free( text );
    return code;
}

/*+ end of file:$Id: tst_err.c 36825 2006-10-25 15:30:04Z cav $ +*/
