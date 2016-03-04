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
 * $Id: tst_file.c 64365 2010-10-07 13:04:10Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека тестирования (TEST).
 *
 * Функции тестирования операций с файлами.
 *
 * Функции: test_file_*
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

/*++++
 * Функция тестирования операции работы с файлами.
 ++++*/
TSupErr reader_test_file_all( 
    TSupSysEContext *context, /*+ (i) Handle считывателя. +*/
    size_t length )
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, тест успешен.
 *
 * Код ошибки, при ошибках.
 +*/
{
    TSupErr code;
    static const TCHAR *test_file_name = _TEXT( "filename" );
    const TCHAR *file_name = test_file_name;
    TRdrFileNumber filno = 1;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File function test in progress...")));
    /*+ Тест на создание, запись, закрытие файла. +*/
    code = reader_test_file_create( context, file_name, filno, length );
    if( code )
	return code;

    /*+ Тест на открытие, чтение, закрытие файла. +*/
    code = reader_test_file_read( context, file_name, filno, length );
    if( code )
	return code;
    /*+ Тест на удаление файла. +*/
    code = reader_test_file_unlink( context, file_name, filno );
    if( code && code != SUP_ERR_UNSUPPORTED )
	return code;
    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File function test successful.")));

    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования операции создания, записи, закрытия файла.
 ++++*/
TSupErr reader_test_file_create( 
    TSupSysEContext *context, /*+ (i) Handle считывателя. +*/
    const TCHAR *file_name, /*+ (i) Имя создаваемого файла. +*/
    TRdrFileNumber filno, /*+ (i) Номер создаваемого файла. +*/
    size_t test_length )
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, тест успешен.
 *
 * Код ошибки, при ошибках.
 +*/
{
    TSupErr code;
    unsigned char *buffer;
    size_t i;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Create/write/close function test in progress...")));

    /*+ Создать файл. +*/
    code = rdr_file_open( context, _TEXT( "wp" ), file_name, filno );
    if( code == SUP_ERR_UNSUPPORTED )
    {
	test_error( context, SUP_ERR_UNSUPPORTED );
	code = rdr_file_open( context, _TEXT( "r+p" ), file_name, filno );
    }
    if( code )
	return test_error( context, code );

    /* Тест на ограничение файла. +*/
    code = reader_test_file_chsize( context, test_length );
    if( code )
	return code;

    buffer = malloc( test_length );
    if( buffer == NULL )
    {
	rdr_file_close( context );
	return test_error( context, SUP_ERR_MEMORY );
    }

    /*+ Записать информацию в файл. +*/
    for( i = 0; i < test_length; i++ )
	buffer[i] = (unsigned char)i;
    code = rdr_file_write( context, 0, test_length, buffer, NULL );
    free( buffer );
    if( code )
	return test_error( context, code );

    /*+ Закрыть файл. +*/
    code = rdr_file_close( context );
    if( code )
	return test_error( context, code );

    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Create/write/close function test successful.")));    
    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования операции открытия, чтения, закрытия файла.
 ++++*/
TSupErr reader_test_file_read( 
    TSupSysEContext *context, /*+ (i) Handle считывателя. +*/
    const TCHAR *file_name, /*+ (i) Имя открываемого файла. +*/
    TRdrFileNumber filno, /*+ (i) Номер открываемого файла. +*/
    size_t test_length )
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, тест успешен.
 *
 * Код ошибки, при ошибках.
 +*/
{
    TSupErr code;
    unsigned char *buffer;
    size_t i;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open/read/close function test in progress...")));    

    /*+ Открыть файл. +*/
    code = rdr_file_open( context, _TEXT( "r" ), file_name, filno );
    if( code )
	return test_error( context, code );

    /*+ Тест на длину файла. +*/
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

    /*+ Прочитать записанную информацию. +*/
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
    /*+ Закрыть файл. +*/
    code = rdr_file_close( context );
    if( code )
	return test_error( context, code );

    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Open/read/close function test successful.")));    
    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования операции получения длины файла.
 ++++*/
TSupErr reader_test_file_length( 
    TSupSysEContext *context ) /*+ (i) Handle считывателя. +*/
{
    TSupErr code;
    size_t length = 0;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File length test in progress...")));    

    /*+ Получить длину файла. +*/
    code = rdr_file_length( context, &length );
    if( code )
	return test_error( context, code );

    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File length:%u."),length));        
    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования операции получения ограничения на длину файла.
 ++++*/
TSupErr reader_test_file_chsize( 
    TSupSysEContext *context, /*+ (i) Handle считывателя. +*/
    size_t test_length )
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, тест успешен.
 *
 * Код ошибки, при ошибках.
 +*/
{
    TSupErr code;
    size_t length = test_length;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File limit test in progress...")));        
    
    /*+ Получить длину файла. +*/
    code = rdr_file_chsize( context, &length );
    if( code )
	return test_error( context, code );

    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File limit:%u."),length));        

    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования операции удаления файла.
 ++++*/
TSupErr reader_test_file_unlink( 
    TSupSysEContext *context, /*+ (i) Handle считывателя. +*/
    const TCHAR *file_name, /*+ (i) Имя удаляемого файла. +*/
    TRdrFileNumber filno ) /*+ (i) Номер удаляемого файла. +*/
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, тест успешен.
 *
 * Код ошибки, при ошибках.
 +*/
{
    TSupErr code;

    /*+ Сообщить о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unlink test in progress...")));        

    /*+ Удалить файл. +*/
    code = rdr_file_unlink( context, file_name, filno );
    if( code )
	return test_error( context, code );

    /*+ Сообщить об успешном окончании теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "File unlink successful.")));        
    return SUP_ERR_NO;
}

/*+ end of file:$Id: tst_file.c 64365 2010-10-07 13:04:10Z dim $ +*/
