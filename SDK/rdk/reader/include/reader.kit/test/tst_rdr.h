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

#include <tchar.h>
#include <stdlib.h>

#if !defined( _READER_TEST_TST_RDR_H )
#define _READER_TEST_TST_RDR_H

#include "reader/rdr.h" /*+ Project (READER/RDR) include.
    Внешний интерфейс работы со считывателями. +*/

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

#if !defined( _READER_TEST_DECL )
#if defined( READER_TEST_IMPORTS )
#define _READER_TEST_DECL _STDIMPL_DECL
#else /* defined( READER_TEST_IMPORTS ) */
#define _READER_TEST_DECL
#endif /* !defined( READER_TEST_IMPORTS ) */
#endif /* !defined( _READER_TEST_DECL ) */

/* **** Функции тестирования списков считывателей. **** */

/* Функция тестирования загрузки библиотек. */
_READER_TEST_DECL TSupErr reader_test_list_load( void );

/* Функция тестирования прохода по списку библиотек. */
_READER_TEST_DECL TSupErr reader_test_list_out( void );

/* Функция тестирования выгрузки библиотек. */
_READER_TEST_DECL TSupErr reader_test_list_unload( void );

/* Основная функция тестирования поддержки нескольких библиотек. */
_READER_TEST_DECL TSupErr reader_test_list_all( void );

/* **** Функции тестирования информационных функций. **** */

/* Получение информации о пароле. */
_READER_TEST_DECL TSupErr reader_test_info_passwd( 
    const TSupSysEContext *context );

/* Получение системных флагов. */
_READER_TEST_DECL TSupErr reader_test_info_system_flag(
    const TSupSysEContext *context );

/* Все информационные тесты. */
_READER_TEST_DECL TSupErr reader_test_info( 
    const TSupSysEContext *context );

/* **** Функции тестирования, требующие соединения со считывателем. **** */

/* Соединение с носителем. */
_READER_TEST_DECL TSupErr reader_test_connect_carrier( 
    TSupSysEContext *context );

/* Отсоединение от носителя. */
_READER_TEST_DECL TSupErr reader_test_disconnect_carrier( 
    TSupSysEContext *context );

/* Установка / снятие флага local machine. */
_READER_TEST_DECL TSupErr reader_test_local_machine( 
    TSupSysEContext *context, int flag );

/* **** Функции тестирования носителя. **** */

/* Функция тестирования уникального номера носителя. */
_READER_TEST_DECL TSupErr reader_test_unique(
    TSupSysEContext *context );

/* **** Функции тестирования приложения. **** */

/* Функция тестирования очистки пароля. */
_READER_TEST_DECL TSupErr reader_test_info_passwd_clear(
    TSupSysEContext *context,
    const TCHAR *master_passwd );

/* Вход в приложение. */
_READER_TEST_DECL TSupErr reader_test_prime( 
    TSupSysEContext *context,
    const TCHAR *passwd );

/* Вход в приложение. */
_READER_TEST_DECL TSupErr reader_test_login( 
    TSupSysEContext *context,
    const TCHAR *passwd );

/* Выход из приложения. */
_READER_TEST_DECL TSupErr reader_test_logout( 
    TSupSysEContext *context );

/* Блокирование. */
_READER_TEST_DECL TSupErr reader_test_lock( 
    TSupSysEContext *context );

/* Разблокирование. */
_READER_TEST_DECL TSupErr reader_test_unlock( 
    TSupSysEContext *context );

/* Функция тестирования изменения пароля. */
_READER_TEST_DECL TSupErr reader_test_passwd_change(
    TSupSysEContext *context,
    const TCHAR *old_passwd,
    const TCHAR *new_passwd );

/* Функция перечисления папок. */
_READER_TEST_DECL TSupErr reader_test_folder_enum( 
    const TSupSysEContext *context );

/* Функция тестирования открытия папки. */
_READER_TEST_DECL TSupErr reader_test_folder_open( 
    TSupSysEContext *context,
    const TCHAR *folder );

/* Функция тестирования создания папки. */
_READER_TEST_DECL TSupErr reader_test_folder_create( 
    TSupSysEContext *context,
    const TCHAR *folder );

/* Функция тестирования очистки папки. */
_READER_TEST_DECL TSupErr reader_test_folder_clear( 
    TSupSysEContext *context );

/* Тестирование закрытия папки. */
_READER_TEST_DECL TSupErr reader_test_folder_close( 
    TSupSysEContext *context );
/* **** Функции тестирования файлов. **** */

/* Функция тестирования операции создания, записи, закрытия файла. */
_READER_TEST_DECL TSupErr reader_test_file_create( 
    TSupSysEContext*, /*+ (i) Handle считывателя. +*/
    const TCHAR*, /*+ (i) Имя создаваемого файла. +*/
    TRdrFileNumber, /*+ (i) Номер создаваемого файла. +*/
    size_t );

/* Функция тестирования операции открытия, чтения, закрытия файла. */
_READER_TEST_DECL TSupErr reader_test_file_read( 
    TSupSysEContext*, /*+ (i) Handle считывателя. +*/
    const TCHAR*, /*+ (i) Имя создаваемого файла. +*/
    TRdrFileNumber, /*+ (i) Номер создаваемого файла. +*/
    size_t length );

/* Функция тестирования операции получения длины файла. */
_READER_TEST_DECL TSupErr reader_test_file_length( 
    TSupSysEContext* ); /*+ (i) Handle считывателя. +*/

/* Функция тестирования операции получения ограничения на длину файла. */
_READER_TEST_DECL TSupErr reader_test_file_chsize( 
    TSupSysEContext*, /*+ (i) Handle считывателя. +*/
    size_t test_length );

/* Функция тестирования операции удаления файла. */
_READER_TEST_DECL TSupErr reader_test_file_unlink( 
    TSupSysEContext*, /*+ (i) Handle считывателя. +*/
    const TCHAR*, /*+ (i) Имя удаляемого файла. +*/
    TRdrFileNumber ); /*+ (i) Номер удаляемого файла. +*/

/* Функция тестирования операции работы с файлами. */
_READER_TEST_DECL TSupErr reader_test_file_all( 
    TSupSysEContext*,
    size_t length );

/*+ **** Функции тестирования кодов ошибок. **** +*/

/* Функция тестирования и вывода сообщения об ошибке. */
_READER_TEST_DECL TSupErr test_error( 
    const TSupSysEContext *context,
    TSupErr );

/*+ **** Тестовый индикатор процесса выполнения. **** +*/

_READER_TEST_DECL TSupErr reader_test_retry_function_set(
    TSupSysEContext *context ); /*+ (i) HANDLE считывателя. +*/

_READER_TEST_DECL TSupErr reader_test_retry_wait_function_set(
    TSupSysEContext *context ); /*+ (i) HANDLE считывателя. +*/

#if defined( SUPPORT_RESOURCE_STD )
extern TSupResourceInstance READER_TEST_RESOURCE;
#else
extern HINSTANCE READER_TEST_RESOURCE;
#endif

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_TEST_TST_RDR_H ) */
/*+ end of file: $Id: tst_rdr.h 68640 2011-06-17 20:29:25Z dim $ +*/
