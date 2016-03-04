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

/*
 * Этот файл содержит набор полезных функций для отладки кода.
 * Эти функции могут не откуда не вызываться, но они нужны.
 */

#include "tst_prj.h" /*+ Project (READER/RDR/TEST) include.
    include файл для проекта (READER/RDR/TEST). +*/

/*++++
 * Функция тестирования получения параметров login и при необходимости
 * получения термина пароль и размера пароля.
 ++++*/
TSupErr reader_test_info_passwd( 
    const TSupSysEContext *context ) /*+ (i) HANDLE считывателя. +*/
{
    TSupErr code; /* Код возврата. */
    size_t min_length = 0; /* Длина пароля. */
    size_t max_length = 0; /* Длина пароля. */
    size_t length;
    TCHAR *term; /* Термин пароль. */
    TRdrLoginInfoType type = RDR_LOGIN_INFO_UNKNOWN;
    
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting login information...")));

    /*+ Получение типа информации необходимого для login. +*/
    code = rdr_passwd_length( context, &type, &max_length, &min_length );
    if( code )
	return test_error( context, code );
    /*+ Если доп. информация не требуется, то только вывод результата. +*/
    if( type == RDR_LOGIN_INFO_NO )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "No additional info needed at login.")));
        return SUP_ERR_NO;
    }
    /*+ В остальных случаях. +*/
    /*+ Вывод на экран типа требуемой информации. +*/
    if( type == RDR_LOGIN_INFO_PASSWD )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password (%d chars max %d chars min) will be used at login."), max_length, min_length));
    }
    else
	DbTrace(DB_TRACE, (FTEXT(support_context_io, "Public information (%d chars max %d chanrs min) will be used at login."), max_length, min_length ));

    /*+ Получение термина пароль. +*/
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
 * Функция тестирования очистки носителя.
 ++++*/
TSupErr reader_test_folder_clear( 
    TSupSysEContext *context ) /*+ (i) HANDLE считывателя. +*/
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
 * Функция тестирования изменения пароля.
 ++++*/
TSupErr reader_test_passwd_change(
    TSupSysEContext *context, /*+ (i) HANDLE считывателя. +*/
    const TCHAR *old_passwd, /*+ (i) Новый пароль. +*/
    const TCHAR *new_passwd ) /*+ (i) Новый пароль. +*/
{
    TSupErr code; /* Код возврата. */
    TSupErr newcode; /* Код возврата при смене пароля. */
    TSupErr newret = SUP_ERR_UNSUPPORTED;
    TRdrLoginInfoType type = RDR_LOGIN_INFO_PASSWD;
    size_t length = 0; /* Длина пароля. */
    const TCHAR *cur_old_passwd = old_passwd;
    const TCHAR *cur_new_passwd = new_passwd;
 
    /*+ Вывод сообщения о начале теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Changing password in progress...")));

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED || (!code && !length) )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ Собственно тест смены пароля. +*/
    code = newcode = rdr_passwd_change( context, cur_new_passwd );
    if( code )
    {
	newret = test_error( context, newcode ); // Наверное, лучше здесь не останавливаться
    }

    /*+ Возврат пароля в прежнее состояние. +*/
    code = rdr_passwd_change( context, cur_old_passwd ); 
    if( code )
    {
	return test_error( context, code ); // А здесь уже всё равно
    }
    if( newcode )
    {
	return newret;
    }

    /*+ Вывод сообщения о конце теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password change successful.")));

    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования очистки пароля.
 ++++*/
TSupErr reader_test_info_passwd_clear(
    TSupSysEContext *context, /*+ (i) HANDLE считывателя. +*/
    const TCHAR *master_passwd ) /*+ (i) Мастер пароль. +*/
{
    TSupErr code; /* Код возврата. */
    TRdrLoginInfoType type = RDR_LOGIN_INFO_PASSWD_MASTER;
    size_t length = 0; /* Длина пароля. */
    const TCHAR *cur_passwd = master_passwd;

    /*+ Вывод сообщения о начеле теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Clearing password in progress...")));

    code = rdr_passwd_length( context, &type, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED || (!code && !length) )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ Собственно тест очистки пароля. +*/
    code = rdr_passwd_clear( context, cur_passwd );

    if( code == SUP_ERR_UNSUPPORTED )
    {
	test_error( context, code );
	return SUP_ERR_NO;
    }

    /*+ Вывод сообщения о конце теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Password clear successfull.")));

    return SUP_ERR_NO;
}

/*++++
 * Функция тестирования получения системных флагов.
 ++++*/
TSupErr reader_test_info_system_flag(
    const TSupSysEContext *context ) /*+ (i) HANDLE считывателя. +*/
{
    TSupErr code; /* Код возврата. */
    int flag; /* Флаг считывателя. */
    
    /*+ Вывод сообщения о начеле теста. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting reader flags...")));
    
    /*+ Вывод сообщения о поддержке файловой системы. +*/
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

    /*+ Вывод сообщения о сменности носителя. +*/
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

    /*+ Вывод сообщения о поддержке уникального номера носителя. +*/
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
 * Функция тестирования уникального номера носителя.
 ++++*/
TSupErr reader_test_unique(
    TSupSysEContext *context ) /*+ (i) HANDLE считывателя. +*/
{
    TSupErr code; /* Код возврата. */
    size_t length; /* Реальная длина уникального номера. */
    TCHAR *unique; /* Уникальный номер. */
    TCHAR *new_unique; /* Новый уникальный номер. */
    static const TCHAR *new_unique_prototype = _TEXT( "This is a Prototype of the New Unique" );

    /*+ Вывод сообщенияя о начале теста чтения уникального номера. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting unique identifier in progress...")));

    /*+ Если уникальный номер может быть очень длинным, то получить реальную длину 
	номера. +*/
    code = rdr_unique_get( context, &length, NULL );
    if( code == SUP_ERR_UNSUPPORTED )
    {
        DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identificator is not support.")));
	return SUP_ERR_NO;
    }
    if( code )
	return test_error( context, code );

    /*+ Получить уникальное имя. +*/
    unique = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( unique == NULL )
	return test_error( context, SUP_ERR_MEMORY );
    code = rdr_unique_get( context, &length, unique );
    if( code )
    {
	free( unique );
	return test_error( context, code );
    }

    /*+ Вывести уникальное имя. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identifier is:%s"),unique));

    /*+ Вывести сообщение о начале установки уникального номера. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Setting unique identifier in progress...")));

    code = rdr_unique_set( context, (TCHAR*)new_unique_prototype );
    /*+ Если функция не поддерживается, вывести сообщение о неподдержке, записи
	уникального номера на носитель. +*/
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

    /*+ Получить уникальное имя. +*/
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

    /*+ Вывести уникальное имя. +*/
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

    /*+ Вывести сообщение о конце теста на запись. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unique identifier set successful to %s."), unique));
    free( unique );
    return SUP_ERR_NO;
}

/*++++
 * Функция получения всей информации о считывателе, которой не требуется
 * носитель.
 +*/
TSupErr reader_test_info( 
    const TSupSysEContext *context ) /*+ (i) HANDLE считывателя. +*/
{
    TSupErr code; /* Код возврата. */

    /*+ Тестирование параметров Login и при необходимости получения термина пароль,
    и размера пароля. +*/
    code = reader_test_info_passwd( context );
    if( code )
	return code;
    
    /*+ Тестирование функции получения системных флагов. +*/
    code = reader_test_info_system_flag( context );
    if( code )
	return code;

    return SUP_ERR_NO;
}
/*+ end of file: $Id: tst_1.c 68640 2011-06-17 20:29:25Z dim $ +*/
