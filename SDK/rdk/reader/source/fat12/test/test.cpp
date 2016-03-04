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

#include "tst12prj.h" /*+ Project (READER/FAT12/TEST) include.
    Мастер include файл для проекта (READER/FAT12/TEST). +*/
TSupportDbContext *support_context_io=NULL;

/* Функция регистрации считывателя. */
static TSupErr register_all( void );

/* Функция разрегистрации считывателя. */
static void unregister_all( void );

/* Функция тестирования. */
static TSupErr test_all( const TCHAR *reader, TCHAR *arg );

/* Список зарегистрированных считывателей. */
static TSupSysEList *reader_list = NULL;

static int std_main( const TCHAR *reader, TCHAR *arg );

int _tmain( int argc, TCHAR *argv[] )
{
    int ret;
    support_context_io = support_print_init( NULL, DTEXT( "fat12tst" ),
	SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );
    
    switch(argc){
      case 1:
        ret = std_main( _TEXT("hdimage"), NULL);
        if(ret) break; 
#ifdef UNIX        
        ret = std_main( _TEXT("fat12_0"), NULL);
#else 
        ret = std_main( _TEXT("fat12_a"), NULL);
#endif        
	break;
      case 2: case 3:
        ret = std_main( argv[1], argv[2] );
        break;
      default:
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "\nUsage: %s [hdimage|fat12_X]"),argv[0]));
	ret=SUP_ERR_PARAM;
        break;
    }
    support_print_done( support_context_io );

    return ret;
}

int std_main( const TCHAR *reader, TCHAR *arg )
{
    TSupErr code;

    code = support_load_library();
    if( code ) return code;
    code = rdr_load_library();
    if( code ) return code;
    code = register_all();
    if( code ) return code;
    code = test_all( reader, arg );

    unregister_all();
    rdr_unload_library();
    support_unload_library();

    return code;
}

/*!
 * \brief Функция регистрации ключевой системы и считывателя.
 */
static TSupErr register_all( void )
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
 * Функция разрегистрации считывателя и ключевой системы.
 */
static void unregister_all( void )
{
    supsys_unregister_all( reader_list, 1 );
}

/*!
 * Функция тестирования.
 * \return Результат теста.
 */
static TSupErr test_all( const TCHAR *reader, TCHAR *arg )
{
    TSupErr code;
    TSupSysEContext *context;

    code = supsys_find_nickname( reader_list, (TCHAR*)reader, arg, &context );
    if( code ) return test_error( NULL, code );
#ifndef UNIX
    code = reader_test_retry_wait_function_set( context ); if( code ) goto exit;
#endif
    code = supsys_test_info( context ); if( code ) goto exit;
    code = reader_test_info( context ); if( code ) goto exit;
    code = supsys_test_connect( context ); if( code ) goto exit;
    code = reader_test_local_machine( context, 1 ); if( code ) goto exit;
    code = reader_test_connect_carrier( context ); if( code ) goto exit;
    code = reader_test_folder_enum( context ); if( code ) goto exit;
    code = reader_test_lock( context ); if( code ) goto exit;
    code = reader_test_folder_create( context, _TEXT( "FOLDER" ) ); if( code ) goto exit;
    code = reader_test_login( context, NULL ); if( code ) goto exit;
    code = reader_test_file_all( context, 13 ); if( code ) goto exit;
    code = reader_test_unique( context ); if( code ) goto exit;
    /* code = reader_test_logout( context ); if( code ) goto exit; */
    code = reader_test_folder_clear( context ); if( code ) goto exit;
    code = reader_test_unlock( context ); if( code ) goto exit;
    code = reader_test_disconnect_carrier( context ); if( code ) goto exit;
    code = supsys_test_disconnect( context ); if( code ) goto exit;
exit:
    supsys_context_free( context );

    return code;
}

/*+ end of file: test.c +*/
