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
 * $Id: version.c 61198 2010-04-10 08:41:58Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции поддержки информации с ключевой дискеты.
 * Получение номера версии.
 *
 * Константа: FAT12_VERSION
 *
 * Функция: fat12_info_version
 *
 * Прямая зависимость от define: 
 *
 * Стандартных: 
 *
 * Определенных пользователем: SUPPORT_ASSERT
 *
 ****/
/*+ Project (READER/SUPPORT) +*/
#include "reader.kit/gen_prj.h"
#ifndef UNIX
#include <windows.h>
#endif
#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"
#include "reader.kit/rdr_ver.h"


/* ------------------------------------------ */
/*+ Текущий номер версии. +*/
static const TSupSysInfoVersion _VERSION = 
{
 sizeof(TSupSysInfoVersion),{3,0,0,0}
};


/*++++
 * Получение номера версии.
 ++++*/
TSupErr info_versionsupport(
    TSupSysContext *context, /*+ (io) Размер передаваемой информации на выходе размер 
	возвращаемой информации (и если ее не достаточно, то размер необходимой 
	информации). +*/
    TSupSysInfo *info ) /*+ (io) Запрашиваемая информация. +*/
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, если функция вернула номер версии
 * SUP_ERR_PARAM, при неправильно переданных параметрах.
 +*/
{
    UNUSED( context );
    if(info == NULL) return SUP_ERR_PARAM;
    if((*(TSupSysInfoVersion*)info).size_of < sizeof( TSupSysInfoVersion ) ) return SUP_ERR_PARAM;

    *(TSupSysInfoVersion*)info = _VERSION;
    return SUP_ERR_NO;
}
/*+ end of file: $Id: version.c 61198 2010-04-10 08:41:58Z dim $ +*/
