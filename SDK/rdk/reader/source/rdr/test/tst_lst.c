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
 * $Id: tst_lst.c 27858 2005-08-16 18:38:11Z pooh $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Интерфейс ко всем считывателям (RDR).
 *
 * Библиотека тестирования (TEST).
 *
 * Функции тестирования поддержки нескольких библиотек.
 *
 * Функции:reader_test_list_*
 *
 * Прямая зависимость от define: 
 *
 * Стандартных: 
 *
 * Определенных пользователем:
 *
 ****/

#include "tst_prj.h" /*+ Project (READER/RDR/TEST) include.
    include файл для проекта (READER/RDR/TEST). +*/

static TSupSysEList *reader_list = NULL;

/*++++
 * Функция тестирования загрузки библиотек.
 ++++*/
TSupErr reader_test_list_load( void )
{
    TSupErr code;

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nAll readers registration in progress...\n")));    
    
    code = rdr_register_start( &reader_list );
    if( code )
    {
	supsys_unregister_all( reader_list, 1 );
	return code;
    }
    code = supsys_register_all( reader_list );
    if( code )
    {
	test_error( NULL, SUP_ERR_VERIFY );
	return code;
    }
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nRegistration completed successfully.\n")));    
    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования прохода по списку библиотек.
 ++++*/
TSupErr reader_test_list_out( void )
{
    TSupSysEFind *context;
    TSupSysEContext *rdr_context;
    TSupErr code;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nReaders test in progress...\n")));    
    
    code = supsys_find_open( reader_list, &context );
    if( code )
	return test_error( NULL, SUP_ERR_MEMORY );
    code = supsys_find_next( context, &rdr_context );
    while( code == SUP_ERR_NO )
    {
	supsys_test_info_nickname( rdr_context );
	supsys_context_free( rdr_context );
	code = supsys_find_next( context, &rdr_context );
    }
    supsys_find_close( context );
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nReaders test completed successfully.\n")));        
    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования выгрузки библиотек.
 ++++*/
TSupErr reader_test_list_unload( void )
{
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nAll readers unregistration in progress...\n")));        
    
    supsys_unregister_all( reader_list, 1 );

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\nUnregistration completed successfully.\n")));            
    return SUP_ERR_NO;
}

/*++++
 * Основная функция тестирования поддержки нескольких библиотек.
 ++++*/
TSupErr reader_test_list_all( void )
{
    TSupErr code;
    code = reader_test_list_load();
    if( code )
	return code;
    
    code = reader_test_list_out();
    if( code )
	return code;
    
    code = reader_test_list_unload();
    if( code )
	return code;
    
    return SUP_ERR_NO;
}

/*+ end of file:$Id: tst_lst.c 27858 2005-08-16 18:38:11Z pooh $ +*/
