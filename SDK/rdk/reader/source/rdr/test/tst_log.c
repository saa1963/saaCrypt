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
 * $Id: tst_log.c 54220 2009-05-02 02:33:10Z lse $
 *
 * ���������� ������ / ������ �������� ���������� (READER).
 *
 * ������� ������������ ������� ������������� ����������� � �����������
 *  ��������� �����������.
 *
 * �������:reader_test_login_*, reader_test_logout_*
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

/* ��������� / ������ ����� local machine. */
TSupErr reader_test_local_machine( 
    TSupSysEContext *context, 
    int flag )
{
    TSupErr code;

    if( flag )
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Setting local machine flag...")));
	code = rdr_local_machine( context, flag );
        if( code )
	   return test_error( context, code );
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Local machine flag set successfully.")));
    }
    else
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Setting current user flag...")));
	code = rdr_local_machine( context, flag );
        if( code )
	   return test_error( context, code );
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Current user flag set successfully.")));
    }

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������������� ��������.
 ++++*/
TSupErr reader_test_login( 
    TSupSysEContext *context,
    const TCHAR *passwd )
{
    TSupErr code;
    TRdrLoginInfoType type = RDR_LOGIN_INFO_UNKNOWN;
    size_t length = 0; /* ����� ������. */
    const TCHAR *cur_passwd = passwd;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Login test in progress...")));

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code )
	return test_error( context, code );

    if( type != RDR_LOGIN_INFO_PASSWD
	&& type != RDR_LOGIN_INFO_NO )
	return test_error( context, SUP_ERR_PARAM );
    code = rdr_login( context, cur_passwd, NULL );

    if( code )
	return test_error( context, code );
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Login successful.")));
    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������������� ��������.
 ++++*/
TSupErr reader_test_prime(
    TSupSysEContext *context,
    const TCHAR *passwd )
{
    TSupErr code;
    TRdrLoginInfoType type = RDR_LOGIN_INFO_PRIME_PASSWD;
    size_t length = 0; /* ����� ������. */
    const TCHAR *cur_passwd = passwd;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Login test in progress...")));    

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code )
	return test_error( context, code );
    if( type != RDR_LOGIN_INFO_PRIME_PASSWD )
	return test_error( context, SUP_ERR_PARAM );
    
    code = rdr_prime( context, cur_passwd, NULL );
    if( code )
	return test_error( context, code );

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Login successful.")));    
    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������������� ��������.
 ++++*/
TSupErr reader_test_logout( 
    TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
/*+ 
 * ����������:
 *
 * SUP_ERR_NO, ��� ������� login
 *
 * SUP_ERR_MEMORY, ��� �������� ������.
 *
 * ������ logout.
 +*/
{
    TSupErr code;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Logout test in progress...")));    
    
    code = rdr_logout( context );
    if( code )
	return test_error( context, code ); 

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Logout test successful.")));        
    return SUP_ERR_NO;
}

/* ������� ������������� � ��������. */
TSupErr reader_test_connect_carrier( 
    TSupSysEContext *context )
{
    TSupErr code;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Connecting to carrier...")));    
    code = rdr_connect_carrier( context );
    if( code == RDR_ERR_FORMAT )
    {
	test_error( context, code ); 
	code = rdr_format_carrier( context );
    }
    if( code )
	return test_error( context, code ); 
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Connected to carrier successfully.")));    
    return SUP_ERR_NO;
}

/* ������� ������������ �� ��������. */
TSupErr reader_test_disconnect_carrier( 
    TSupSysEContext *context )
{
    TSupErr code;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Disconnecting from carrier...")));    

    code = rdr_disconnect_carrier( context );
    if( code )
	return test_error( context, code ); 
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Disconnected from carrier successfully.")));    
    return SUP_ERR_NO;
}

TSupErr reader_test_folder_create(
    TSupSysEContext *context,
    const TCHAR *folder )
{
    TSupErr code;
    TCHAR *real_name;
    TRdrFolderEnumContext *enum_ctx;
    size_t folder_length;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Creating folder...")));    

    code = rdr_folder_enum_open( context, &enum_ctx, &folder_length,
	folder, RDR_FOLDER_ENUM_FLAG_NEW );
    if( code )
	return test_error( context, code );
    real_name = malloc( ( folder_length + 1 ) * sizeof( TCHAR ) );
    if( real_name == NULL )
    {
	rdr_folder_enum_close( enum_ctx );
	return test_error( context, code );
    }
    code = RDR_ERR_FILE_EXIST;
    while( code == RDR_ERR_FILE_EXIST )
    {
	code = rdr_folder_enum_next( enum_ctx, real_name );
	if( code )
	{
	    free( real_name );
	    rdr_folder_enum_close( enum_ctx );
	    return test_error( context, code );
	}
	code = rdr_folder_open( context, _TEXT( "w" ), real_name );
    }
    free( real_name );
    rdr_folder_enum_close( enum_ctx );
    if( code )
	return test_error( context, code ); 

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open folder success.")));    
    return SUP_ERR_NO;
}

/* ������� ������������ �������� �����. */
TSupErr reader_test_folder_open( 
    TSupSysEContext *context,
    const TCHAR *folder )
{
    TSupErr code;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Opening folder `%s'..."), folder));    
    
    code = rdr_folder_open( context, _TEXT( "r" ), folder );
    if( code )
	return test_error( context, code ); 
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open folder success.")));    
    return SUP_ERR_NO;
}

/* ������������ �������� �����. */
TSupErr reader_test_folder_close( 
    TSupSysEContext *context )
{
    TSupErr code;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Closing folder...")));    
    
    code = rdr_folder_close( context );
    if( code )
	return test_error( context, code ); 

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Folder closed successfully.")));    
    
    return SUP_ERR_NO;
}

TSupErr reader_test_folder_enum(
    const TSupSysEContext *context )
{
    TSupErr code;
    TRdrFolderEnumContext *enum_ctx;
    size_t folder_len;
    TCHAR *cur_folder;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Enumerating folders...")));    
    
    code = rdr_folder_enum_open( context, &enum_ctx, &folder_len, NULL, 0 );
    if( code )
	return test_error( context, code ); 
    cur_folder = malloc( ( folder_len + 1 ) * sizeof( TCHAR ) );
    if( !cur_folder )
    {
	rdr_folder_enum_close( enum_ctx );
	return test_error( context, SUP_ERR_MEMORY );
    }
    code = rdr_folder_enum_next( enum_ctx, cur_folder );
    while( !code )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "%s"), cur_folder));    	
	code = rdr_folder_enum_next( enum_ctx, cur_folder );
    }
    free( cur_folder );
    if( code != RDR_ERR_FILE_NOT_FOUND )
    {
	rdr_folder_enum_close( enum_ctx );
	return test_error( context, code );
    }
    code = rdr_folder_enum_close( enum_ctx );
    if( code )
	return test_error( context, code );

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Enumerating folders success.")));    	    
    return SUP_ERR_NO;
}

/* ������� ������������. */
TSupErr reader_test_lock( TSupSysEContext *context)
{
    TSupErr code;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Locking...")));    
    
    code = rdr_lock( context );
    if( code )
	return test_error( context, code ); 
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Lock success.")));    
    return SUP_ERR_NO;
}
/* ������� ���������������. */
TSupErr reader_test_unlock( TSupSysEContext *context)
{
    TSupErr code;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unlocking...")));    
    
    code = rdr_unlock( context );
    if( code )
	return test_error( context, code ); 
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unlock success.")));    
    return SUP_ERR_NO;
}
/*+ end of file:$Id: tst_log.c 54220 2009-05-02 02:33:10Z lse $ +*/
