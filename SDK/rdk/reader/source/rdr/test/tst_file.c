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
 * $Id: tst_file.c 64365 2010-10-07 13:04:10Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������������ (TEST).
 *
 * ������� ������������ �������� � �������.
 *
 * �������: test_file_*
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
 * ������� ������������ �������� ������ � �������.
 ++++*/
TSupErr reader_test_file_all( 
    TSupSysEContext *context, /*+ (i) Handle �����������. +*/
    size_t length )
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� �������.
 *
 * ��� ������, ��� �������.
 +*/
{
    TSupErr code;
    static const TCHAR *test_file_name = _TEXT( "filename" );
    const TCHAR *file_name = test_file_name;
    TRdrFileNumber filno = 1;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File function test in progress...")));
    /*+ ���� �� ��������, ������, �������� �����. +*/
    code = reader_test_file_create( context, file_name, filno, length );
    if( code )
	return code;

    /*+ ���� �� ��������, ������, �������� �����. +*/
    code = reader_test_file_read( context, file_name, filno, length );
    if( code )
	return code;
    /*+ ���� �� �������� �����. +*/
    code = reader_test_file_unlink( context, file_name, filno );
    if( code && code != SUP_ERR_UNSUPPORTED )
	return code;
    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File function test successful.")));

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ �������� ��������, ������, �������� �����.
 ++++*/
TSupErr reader_test_file_create( 
    TSupSysEContext *context, /*+ (i) Handle �����������. +*/
    const TCHAR *file_name, /*+ (i) ��� ������������ �����. +*/
    TRdrFileNumber filno, /*+ (i) ����� ������������ �����. +*/
    size_t test_length )
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� �������.
 *
 * ��� ������, ��� �������.
 +*/
{
    TSupErr code;
    unsigned char *buffer;
    size_t i;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Create/write/close function test in progress...")));

    /*+ ������� ����. +*/
    code = rdr_file_open( context, _TEXT( "wp" ), file_name, filno );
    if( code == SUP_ERR_UNSUPPORTED )
    {
	test_error( context, SUP_ERR_UNSUPPORTED );
	code = rdr_file_open( context, _TEXT( "r+p" ), file_name, filno );
    }
    if( code )
	return test_error( context, code );

    /* ���� �� ����������� �����. +*/
    code = reader_test_file_chsize( context, test_length );
    if( code )
	return code;

    buffer = malloc( test_length );
    if( buffer == NULL )
    {
	rdr_file_close( context );
	return test_error( context, SUP_ERR_MEMORY );
    }

    /*+ �������� ���������� � ����. +*/
    for( i = 0; i < test_length; i++ )
	buffer[i] = (unsigned char)i;
    code = rdr_file_write( context, 0, test_length, buffer, NULL );
    free( buffer );
    if( code )
	return test_error( context, code );

    /*+ ������� ����. +*/
    code = rdr_file_close( context );
    if( code )
	return test_error( context, code );

    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Create/write/close function test successful.")));    
    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ �������� ��������, ������, �������� �����.
 ++++*/
TSupErr reader_test_file_read( 
    TSupSysEContext *context, /*+ (i) Handle �����������. +*/
    const TCHAR *file_name, /*+ (i) ��� ������������ �����. +*/
    TRdrFileNumber filno, /*+ (i) ����� ������������ �����. +*/
    size_t test_length )
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� �������.
 *
 * ��� ������, ��� �������.
 +*/
{
    TSupErr code;
    unsigned char *buffer;
    size_t i;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open/read/close function test in progress...")));    

    /*+ ������� ����. +*/
    code = rdr_file_open( context, _TEXT( "r" ), file_name, filno );
    if( code )
	return test_error( context, code );

    /*+ ���� �� ����� �����. +*/
    code = reader_test_file_length( context );
    if( code )
    {
	rdr_file_close( context );
	return code;
    }

    buffer = malloc( test_length );
    if( buffer == NULL )
    {
	rdr_file_close( context );
	return test_error( context, SUP_ERR_MEMORY );
    }

    /*+ ��������� ���������� ����������. +*/
    code = rdr_file_read( context, 0, test_length, buffer, NULL );
    if( code )
	return test_error( context, code );
    for( i = 0; i < test_length; i++ )
	if( buffer[i] != ( i & 0xff ) )
	{
	    free( buffer );
	    return test_error( context, SUP_ERR_VERIFY );
	}

    free( buffer );
    /*+ ������� ����. +*/
    code = rdr_file_close( context );
    if( code )
	return test_error( context, code );

    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open/read/close function test successful.")));    
    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ �������� ��������� ����� �����.
 ++++*/
TSupErr reader_test_file_length( 
    TSupSysEContext *context ) /*+ (i) Handle �����������. +*/
{
    TSupErr code;
    size_t length = 0;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File length test in progress...")));    

    /*+ �������� ����� �����. +*/
    code = rdr_file_length( context, &length );
    if( code )
	return test_error( context, code );

    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File length:%u."),length));        
    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ �������� ��������� ����������� �� ����� �����.
 ++++*/
TSupErr reader_test_file_chsize( 
    TSupSysEContext *context, /*+ (i) Handle �����������. +*/
    size_t test_length )
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� �������.
 *
 * ��� ������, ��� �������.
 +*/
{
    TSupErr code;
    size_t length = test_length;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File limit test in progress...")));        
    
    /*+ �������� ����� �����. +*/
    code = rdr_file_chsize( context, &length );
    if( code )
	return test_error( context, code );

    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File limit:%u."),length));        

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ �������� �������� �����.
 ++++*/
TSupErr reader_test_file_unlink( 
    TSupSysEContext *context, /*+ (i) Handle �����������. +*/
    const TCHAR *file_name, /*+ (i) ��� ���������� �����. +*/
    TRdrFileNumber filno ) /*+ (i) ����� ���������� �����. +*/
/*+
 * ����������:
 *
 * SUP_ERR_NO, ���� �������.
 *
 * ��� ������, ��� �������.
 +*/
{
    TSupErr code;

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unlink test in progress...")));        

    /*+ ������� ����. +*/
    code = rdr_file_unlink( context, file_name, filno );
    if( code )
	return test_error( context, code );

    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File unlink successful.")));        
    return SUP_ERR_NO;
}

/*+ end of file:$Id: tst_file.c 64365 2010-10-07 13:04:10Z dim $ +*/
