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

/****
 * $Id: test.c 68807 2011-06-27 07:16:13Z dim $
 *
 * Интерфейс к датчикам случайных чисел (RNDM).
 *
 * Программа тестирования ДСЧ (TEST).
 *
 * Основной модуль тестов.
 *
 * Функция:main
 *
 * Прямая зависимость от define:
 *
 * Стандартных: 
 ****/

#include "rndm_tst.h" /*+ Project (RNDM/TEST) include.
    Мастер include файл для проекта (RNDM/TEST). +*/

/*!
 * \brief Размер тестируемого буфера.
 */
#if !defined( BUFFER_LEN )
#define BUFFER_LEN 32
#endif /* !defined( BUFFER_LEN ) */

TSupportDbContext *support_context_io=NULL;

static int std_main( int argc, TCHAR *argv[] );

static TSupErr rndm_test_info( TSupSysEContext *context );
#if defined( SUPPORT_RESOURCE_STD )
extern TSupResourceInstance RNDM_TST_RESOURCE;
#else
#define RNDM_TST_RESOURCE NULL
#endif

static TSupErr test_all( TSupSysEContext *context, size_t buffer_len );

int main( int argc, TCHAR *argv[] )
{
    int ret;
    support_context_io = support_print_init( NULL, DTEXT( "rndm_tst" ),
	SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );

    ret = std_main( argc, argv );
    support_print_done( support_context_io );
    return ret;
}

/*!
 * Функция полного тестирования ДСЧ.
 */
int std_main( int argc, TCHAR *argv[] )
{
    TSupErr code = SUP_ERR_NO;
    TSupSysEContext *context;
    TSupSysEList *list = NULL;
    int buffer_len = BUFFER_LEN;
    
    if( support_load_library() )
	return SUP_ERR_VERIFY;
    if( rndm_load_library() )
	return SUP_ERR_VERIFY;
    code = rndm_register_start( &list );
    if( code ) goto exit;
    code = supsys_register_all( list );
    if( code ) goto exit;
    code = rndm_find_mask( list, &context, RNDM_TYPE_ANY_MASK, 
	RNDM_TYPE_ANY_VALUE );
    if( code ) goto exit;
    if( argc > 1 ) buffer_len = _ttoi( argv[1] );
    code = test_all( context, buffer_len );
    rndm_context_free( context );
    supsys_unregister_all( list, 1 );
 exit:
    rndm_unload_library();
    support_unload_library();
#if defined VALGRIND_CHECK
    VALGRIND_PRINTF("\n\nRequired \"4 bytes in 1 blocks are definitely lost, supsys_register_start (slreg.c:373)\"");
#endif	/* VALGRIND_CHECK */

    return code;
}

TSupErr test_all( TSupSysEContext *context, size_t buffer_len )
{
    TSupErr code = SUP_ERR_NO;
    unsigned char *buf;

    buf = malloc( buffer_len );
    if( !buf )
	return SUP_ERR_MEMORY;
    memset( buf, 0, buffer_len );
    code = rndm_test_info( context );
    if( code )
    {
	free( buf );
	return code;
    }
    code = rndm( context, buffer_len, buf );
    if( code )
    {
	free( buf );
	return code;
    }

    DbTraceHex(DB_TRACE, support_context_io, NULL, buf, buffer_len);
    free( buf );
    return SUP_ERR_NO;
}

static TSupErr rndm_test_info( TSupSysEContext *context )
{
    TSupErr code;
    TCHAR *text;
    size_t length = 0;
    
#if !defined (UNIX)
    HICON icon;
#endif /* !defined( UNIX ) */

    /* Имя. */
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Getting rndm name...\n")));
    code = supsys_name( context, &length, NULL );
    if( code )
	return code;
    text = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( text == NULL )
	return SUP_ERR_MEMORY;
    code = supsys_name( context, &length, text );
    if( code )
    {
	free( text );
	return code;
    }
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Rndm name:%s\n"),text));
    free( text );

    /* Copyright. */
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Getting copyright info...\n")));
    code = supsys_copyright( context, &length, NULL );
    if( code )
	return code;
    text = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( text == NULL )
	return SUP_ERR_MEMORY;
    code = supsys_copyright( context, &length, text );
    if( code )
    {
	free( text );
	return code;
    }
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Copyright info:%s\n"),text));
    free( text );

    /* icon */
#if !defined (UNIX)
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Getting icon...\n")));
    code = supsys_icon( context, &icon );
    if( code )
	return code;
    DbTrace(DB_TRACE,(FTEXT(support_context_io, "Get icon success...\n")));
#endif /* !defined( UNIX ) */
    return SUP_ERR_NO;
}

/* end of file: $Id: test.c 68807 2011-06-27 07:16:13Z dim $ */
