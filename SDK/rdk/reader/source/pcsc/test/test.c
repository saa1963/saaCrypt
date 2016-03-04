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

#include "pcsctprj.h"
TSupportDbContext *support_context_io=NULL;

/* ������� �������������� �����������. */
static TSupErr register_all( void );

/* ������� �������������� �����������. */
static void unregister_all( void );

/* ����� ������ ����� �� login */
static TSupErr test_unlogged( TSupSysEContext* context );

/* ����� ��������� login. */
static TSupErr test_in_log(
    TSupSysEContext* context,
    const TCHAR *passwd, const TCHAR *newpasswd);

/* ������� ������������. */
static TSupErr test_all( 
    TSupSysENickname name, 
    const TCHAR *folder, const TCHAR *passwd, const TCHAR *newpasswd);

/* ������ ������������������� ������������. */
static TSupSysEList *reader_list = NULL;

int main( int argc, TCHAR *argv[] );

static const TCHAR std_passwd[9] = _TEXT( "11111111" );
static const TCHAR new_passwd[9] = _TEXT( "22222222" );

/*
    "AKS ifdh 0"
    "GEMPLUS GCR410P 0"
    "ArtSystemz UniFlex User 0"
*/

/*!
 * \brief ������� ������� ������������ ���������� ��������� �����������.
 */
int _tmain( int argc, TCHAR *argv[] )
{
    TSupErr code;
    const TCHAR *folder = _TEXT( "0B00" ); // _TEXT( "CPCSP" );
    const TCHAR *passwd = std_passwd;
    const TCHAR *newpasswd = NULL;
    support_context_io = support_print_init( NULL, DTEXT( "pcsc_tst" ),
	SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );

    if( argc < 2 )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, " Use %s <reader_name> [<folder> [<pwd> [<newpwd>]]]"), argv[0]));
        support_print_done( support_context_io );
        return SUP_ERR_PARAM;
    }

    if( argc > 2)
    {
	folder = argv[2];
    }
    if( argc > 3)
    {
	passwd = argv[3];
	newpasswd = NULL;
    }
    if( argc > 4)
    {
	newpasswd = argv[4];
    }
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader:`%s', Folder:`%s', Password:`%s'."), argv[1], folder, passwd));
    if(newpasswd)
    {
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Test New Password:`%s'."), newpasswd));
    }

    code = support_load_library ();
    if ( code )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Support library failed: %d."), code));
        support_print_done( support_context_io );
        return SUP_ERR_PARAM;
    }

    code = rdr_load_library ();
    if ( code )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Reader library failed: %d."), code));
        support_print_done( support_context_io );
        return SUP_ERR_PARAM;
    }
    code = register_all();
    if( !code )
    {
	code = test_all( argv[1], folder, passwd, newpasswd );
        unregister_all();
    }
    support_print_done( support_context_io );
    return code;
}

/*!
 * \brief ������� ����������� �����������.
 */
static TSupErr register_all()
{
    TSupErr code;

    code = rdr_register_start( &reader_list );
    if( code )
    {
        supsys_unregister_all( reader_list, 1 );
        return code;
    }
    code = supsys_register_all( reader_list );
    if( code )
    {
        supsys_unregister_all( reader_list, 1 );
        return test_error( NULL, code );
    }
    return SUP_ERR_NO;
}

/*!
 * ������� �������������� ����������� � �������� �������.
 */
static void unregister_all( void )
{
    supsys_unregister_all( reader_list, 1 );
}

/*!
 * ����� ��������� login.
 * \param context [in] �������� �����������, �� ������� �������� login.
 * \return ��������� �����
 */
static TSupErr test_in_log(
    TSupSysEContext* context,
    const TCHAR *passwd, const TCHAR *newpasswd)
{
    TSupErr code;
 
    // � ����� � ���������� ������������ ����� ��� ���������� ������
    // ����������������. ��� ������� ���������� ����� ����������
    // ����������� �����. PWD
    UNUSED(passwd); UNUSED(newpasswd); 
    if (newpasswd) {
	code = reader_test_passwd_change(context, passwd, newpasswd);
	if( code )
	    return code;
    }
    code = reader_test_file_all( context, 13 );
    if( code )
        return code;
    code = reader_test_unique( context );
    if( code )
        return code;
    return code;
}

TSupErr test_unlogged( TSupSysEContext* context )
{
    TSupErr code;
    static const TCHAR *test_file_name = _TEXT( "filename" );
    const TCHAR *file_name = test_file_name;
    const TRdrFileNumber filno = 6;
    unsigned char buffer[10];
    const size_t length = sizeof( buffer );

    /*+ �������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nUnlogged read function test in progress...\n")));
    code = rdr_file_open( context, _TEXT( "r" ), file_name, filno );
    if( code )
	return test_error( context, code );
    code = rdr_file_read( context, 0, length, buffer, NULL );
    if( code )
	return test_error( context, code );
    code = rdr_file_close( context );
    if( code )
	return test_error( context, code );
    /*+ �������� �� �������� ��������� �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nUnlogged read test successful.\n")));

    return SUP_ERR_NO;
}

/*!
 * ������� ������������.
 * \return ��������� �����.
 */
static TSupErr test_all( TSupSysENickname name, const TCHAR *folder, const TCHAR *passwd, const TCHAR *newpasswd )
{
    TSupErr code;
    TSupSysEContext *context;

    code = supsys_find_nickname( reader_list, name, NULL, &context );
    if( code )
        return test_error( NULL, code );
    code = reader_test_retry_wait_function_set( context );
    if( code )
    {
        supsys_context_free( context );
        return code;
    }
    code = supsys_test_info( context );
    if( code )
    {
        supsys_context_free( context );
        return code;
    }
    code = reader_test_info( context );
    if( code )
        return code;
    code = rdr_lock( context );
    if( code )
        return code;
    code = supsys_test_connect( context );
    if( code )
    {
        supsys_context_free( context );
        return code;
    }
    code = reader_test_connect_carrier( context );
    if( code )
    {
        supsys_context_free( context );
        return code;
    }
    code = reader_test_folder_enum( context );
    if( code )
    {
        supsys_context_free( context );
        return code;
    }
    code = reader_test_folder_open( context, folder );
    if( code == RDR_ERR_PRIME )
    {
        code = reader_test_prime( context, passwd );
        if( code )
            return test_error( NULL, code );
        code = reader_test_folder_create( context, NULL );
        if( code )
        {
            supsys_context_free( context );
            return code;
        }
        code = rdr_prime_login( context );
    }
    else if( code )
    {
        supsys_context_free( context );
        return code;
    }
    else
    {
	/* �� ��� ����� ����� ������������ ��� ����������������.
	���������������� ���� �����, ���� ���� ����� ������������ ������
	�� ���������� ����� ��� �����������. */
	code = test_unlogged( context );
	if( code )
	{
	    supsys_context_free( context );
	    return code;
	}
        code = reader_test_login( context, passwd );
    }
    if( code )
    {
        supsys_context_free( context );
        return code;
    }

    // � ����� � ���������� ������������ ����� ��� ���������� ������
    // newpasswd ���������������. ��� ������� ���������� ����� ����������
    // ����������� �����.
    code = test_in_log( context, passwd, /*NULL*/ newpasswd );
    if( code )
    {
        reader_test_logout( context );
        supsys_context_free( context );
        return code;
    }
    code = reader_test_folder_clear( context );
    supsys_context_free( context );
    return code;
    /* �� ��� ����� ������� ������������ ������� ������, �� ��� ��� ����,
    ������� �� ������������ ��� ���������������� ������ ���� ��������������!

    code = reader_test_info_passwd_clear( context, _TEXT( "12345678" ) );
    if( code == SUP_ERR_UNSUPPORTED )
        code = SUP_ERR_NO;
    if( code )
    {
        if( info != std )
            free( info );
        supsys_context_free( context );
        return code;
    }
    */
}
/* end of file:$Id: test.c 68807 2011-06-27 07:16:13Z dim $ */
