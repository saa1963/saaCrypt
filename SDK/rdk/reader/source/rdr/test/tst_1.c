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

/*
 * ���� ���� �������� ����� �������� ������� ��� ������� ����.
 * ��� ������� ����� �� ������ �� ����������, �� ��� �����.
 */

#include "tst_prj.h" /*+ Project (READER/RDR/TEST) include.
    include ���� ��� ������� (READER/RDR/TEST). +*/

/*++++
 * ������� ������������ ��������� ���������� login � ��� �������������
 * ��������� ������� ������ � ������� ������.
 ++++*/
TSupErr reader_test_info_passwd( 
    const TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
{
    TSupErr code; /* ��� ��������. */
    size_t min_length = 0; /* ����� ������. */
    size_t max_length = 0; /* ����� ������. */
    size_t length;
    TCHAR *term; /* ������ ������. */
    TRdrLoginInfoType type = RDR_LOGIN_INFO_UNKNOWN;
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting login information...")));

    /*+ ��������� ���� ���������� ������������ ��� login. +*/
    code = rdr_passwd_length( context, &type, &max_length, &min_length );
    if( code )
	return test_error( context, code );
    /*+ ���� ���. ���������� �� ���������, �� ������ ����� ����������. +*/
    if( type == RDR_LOGIN_INFO_NO )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "No additional info needed at login.")));
        return SUP_ERR_NO;
    }
    /*+ � ��������� �������. +*/
    /*+ ����� �� ����� ���� ��������� ����������. +*/
    if( type == RDR_LOGIN_INFO_PASSWD )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password (%d chars max %d chars min) will be used at login."), max_length, min_length));
    }
    else
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Public information (%d chars max %d chanrs min) will be used at login."), max_length, min_length ));

    /*+ ��������� ������� ������. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting password term.")));
    
    code = rdr_passwd_term( context, &length, NULL );
    if( code )
	return test_error( context, code );
    term = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( term == NULL )
	return test_error( context, SUP_ERR_MEMORY );
    code = rdr_passwd_term( context, &length, term );
    if( code )
    {
	free( term );
	return test_error( context, code );
    }

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password term:%s."),term));

    free( term );
    return SUP_ERR_NO;
}

#define STD_LENGTH_MAX 20


/*++++
 * ������� ������������ ������� ��������.
 ++++*/
TSupErr reader_test_folder_clear( 
    TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
{
    TSupErr code;
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Clearing folder in progress...")));

    code = rdr_folder_clear( context );

    if( code )
	return test_error( context, code );
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Clearing folder successful.")));

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ��������� ������.
 ++++*/
TSupErr reader_test_passwd_change(
    TSupSysEContext *context, /*+ (i) HANDLE �����������. +*/
    const TCHAR *old_passwd, /*+ (i) ����� ������. +*/
    const TCHAR *new_passwd ) /*+ (i) ����� ������. +*/
{
    TSupErr code; /* ��� ��������. */
    TSupErr newcode; /* ��� �������� ��� ����� ������. */
    TSupErr newret = SUP_ERR_UNSUPPORTED;
    TRdrLoginInfoType type = RDR_LOGIN_INFO_PASSWD;
    size_t length = 0; /* ����� ������. */
    const TCHAR *cur_old_passwd = old_passwd;
    const TCHAR *cur_new_passwd = new_passwd;
 
    /*+ ����� ��������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Changing password in progress...")));

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED || (!code && !length) )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ ���������� ���� ����� ������. +*/
    code = newcode = rdr_passwd_change( context, cur_new_passwd );
    if( code )
    {
	newret = test_error( context, newcode ); // ��������, ����� ����� �� ���������������
    }

    /*+ ������� ������ � ������� ���������. +*/
    code = rdr_passwd_change( context, cur_old_passwd ); 
    if( code )
    {
	return test_error( context, code ); // � ����� ��� �� �����
    }
    if( newcode )
    {
	return newret;
    }

    /*+ ����� ��������� � ����� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password change successful.")));

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������� ������.
 ++++*/
TSupErr reader_test_info_passwd_clear(
    TSupSysEContext *context, /*+ (i) HANDLE �����������. +*/
    const TCHAR *master_passwd ) /*+ (i) ������ ������. +*/
{
    TSupErr code; /* ��� ��������. */
    TRdrLoginInfoType type = RDR_LOGIN_INFO_PASSWD_MASTER;
    size_t length = 0; /* ����� ������. */
    const TCHAR *cur_passwd = master_passwd;

    /*+ ����� ��������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Clearing password in progress...")));

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED || (!code && !length) )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ ���������� ���� ������� ������. +*/
    code = rdr_passwd_clear( context, cur_passwd );

    if( code == SUP_ERR_UNSUPPORTED )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }

    /*+ ����� ��������� � ����� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password clear successfull.")));

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ��������� ��������� ������.
 ++++*/
TSupErr reader_test_info_system_flag(
    const TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
{
    TSupErr code; /* ��� ��������. */
    int flag; /* ���� �����������. */
    
    /*+ ����� ��������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting reader flags...")));
    
    /*+ ����� ��������� � ��������� �������� �������. +*/
    code = supsys_flag( context, RDR_FLAG_BLOCK_RDR, RDR_FLAG_RDR_LONG_NAMES,
	&flag );

    if( code )
	return test_error( context, code );
    if( flag )
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader support long names.")));
    }else{
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader is not support long names.")));
    }   

    /*+ ����� ��������� � ��������� ��������. +*/
    code = supsys_flag( context, RDR_FLAG_BLOCK_RDR, RDR_FLAG_RDR_REMOVABLE,
	&flag );
    if( code )
	return test_error( context, code );
    if( flag )
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader is removable.")));
    }else{
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader is not removable.")));
    }

    /*+ ����� ��������� � ��������� ����������� ������ ��������. +*/
    code = supsys_flag( context, RDR_FLAG_BLOCK_RDR, RDR_FLAG_RDR_UNIQUE,
	&flag );
    if( code )
	return test_error( context, code );
    if( flag )
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader supports unique identifier.")));
    }else{
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader does not support unique identifier.")));
    }

    return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ����������� ������ ��������.
 ++++*/
TSupErr reader_test_unique(
    TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
{
    TSupErr code; /* ��� ��������. */
    size_t length; /* �������� ����� ����������� ������. */
    TCHAR *unique; /* ���������� �����. */
    TCHAR *new_unique; /* ����� ���������� �����. */
    static const TCHAR *new_unique_prototype = _TEXT( "This is a Prototype of the New Unique" );

    /*+ ����� ���������� � ������ ����� ������ ����������� ������. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting unique identifier in progress...")));

    /*+ ���� ���������� ����� ����� ���� ����� �������, �� �������� �������� ����� 
	������. +*/
    code = rdr_unique_get( context, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identificator is not support.")));
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ �������� ���������� ���. +*/
    unique = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( unique == NULL )
	return test_error( context, SUP_ERR_MEMORY );
    code = rdr_unique_get( context, &length, unique );
    if( code )
    {
	free( unique );
	return test_error( context, code );
    }

    /*+ ������� ���������� ���. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identifier is:%s"),unique));

    /*+ ������� ��������� � ������ ��������� ����������� ������. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Setting unique identifier in progress...")));

    code = rdr_unique_set( context, (TCHAR*)new_unique_prototype );
    /*+ ���� ������� �� ��������������, ������� ��������� � �����������, ������
	����������� ������ �� ��������. +*/
    if( code == SUP_ERR_UNSUPPORTED )
    {
	free( unique );
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Setting unique identifier unsupported.")));
	return SUP_ERR_NO;
    }

    code = rdr_unique_get( context, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED )
    {
	free( unique );
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identificator is not support.")));
	return SUP_ERR_NO;
    }
    if( code )
    {
	free( unique );
	return test_error( context, code );
    }

    /*+ �������� ���������� ���. +*/
    new_unique = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( new_unique == NULL )
    {
	free( unique );
	return test_error( context, SUP_ERR_MEMORY );
    }
    code = rdr_unique_get( context, &length, new_unique );
    if( code )
    {
	free( unique ); free( new_unique );
	return test_error( context, code );
    }

    /*+ ������� ���������� ���. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identifier set successful to %s."), new_unique));
    free( new_unique );

    code = rdr_unique_set( context, unique );
    if( code )
    {
	free( unique );
	return test_error( context, code );
    }
    code = rdr_unique_get( context, &length, NULL );
    if( code )
    {
	free( unique ); 
	return test_error( context, code );
    }
    new_unique = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( !new_unique )
    {
	free( unique ); 
	return test_error( context, SUP_ERR_MEMORY );
    }
    code = rdr_unique_get( context, &length, new_unique );
    if( code )
    {
	free( unique ); free( new_unique );
	return test_error( context, code );
    }
    if( _tcsicmp( unique, new_unique ) )
    {
	free( unique );
	free( new_unique );
	return test_error( context, SUP_ERR_VERIFY );
    }
    free( new_unique );

    /*+ ������� ��������� � ����� ����� �� ������. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identifier set successful to %s."), unique));
    free( unique );
    return SUP_ERR_NO;
}

/*++++
 * ������� ��������� ���� ���������� � �����������, ������� �� ���������
 * ��������.
 +*/
TSupErr reader_test_info( 
    const TSupSysEContext *context ) /*+ (i) HANDLE �����������. +*/
{
    TSupErr code; /* ��� ��������. */

    /*+ ������������ ���������� Login � ��� ������������� ��������� ������� ������,
    � ������� ������. +*/
    code = reader_test_info_passwd( context );
    if( code )
	return code;
    
    /*+ ������������ ������� ��������� ��������� ������. +*/
    code = reader_test_info_system_flag( context );
    if( code )
	return code;

    return SUP_ERR_NO;
}
/*+ end of file: $Id: tst_1.c 68640 2011-06-17 20:29:25Z dim $ +*/
