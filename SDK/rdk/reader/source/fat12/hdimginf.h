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

#if !defined( _READER_HDIMAGE_HDIMGINF_H )
#define _READER_HDIMAGE_HDIMGINF_H

#include "reader/rdr.h" /* Project (READER) include.
    Внешний интерфейс работы со считывателями. */
#include "reader.kit/reader.h" /* Project (READER/RDR) include.
    Определения необходимые для создания библиотеки считывателя. */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

/* Функция очистки. */
/* Функция получения системных флагов. */
TSupErr hdimage_info_system_flag(
    TSupSysContext *context, TSupSysInfo* );

/* Функция регистрирования считывателя. */
TSupErr hdimage_register(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция установки флага local_machine. */
TSupErr hdimage_local_machine(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr hdimage_info_ids(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ehdimage_info_system_flag(
    TSupSysContext *context, TSupSysInfo* );

TSupErr ehdimage_info_ids(
    TSupSysContext *context, TSupSysInfo *info );

TSupErr ehdimage_setup_encryption(
    TSupSysContext *context, TSupSysInfo *info );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _READER_HDIMAGE_HDINF_H ) */
/* end of file:$Id: hdimginf.h 42536 2007-09-26 13:50:44Z chudov $ */
