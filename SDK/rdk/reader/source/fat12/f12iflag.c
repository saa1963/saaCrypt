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
 * $Id: f12iflag.c 60080 2010-02-16 08:37:02Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции поддержки информации с ключевой дискеты.
 * Функция получения системных флагов.
 *
 * Функция: fat12_info_system_flag
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

/*!
 * \ingroup fat12_internal
 * \brief Флаги считывателя.
 */
static const unsigned char FAT12_FLAGS_GENERAL[] = 
{ 
    0 << 7 
};

static const unsigned char FAT12_FLAGS_CONNECT[] = 
{
      0 << 7 /* SUPSYS_FLAG_CONNECT_MANY */
    | 0 << 6 /* SUPSYS_FLAG_CONNECT_COM */
    | 0 << 5 /* SUPSYS_FLAG_CONNECT_LPT */
    | 0 << 4 /* SUPSYS_FLAG_CONNECT_AUTO */
    | 0 << 3 /* SUPSYS_FLAG_CONNECT_MULTY */
    | 0 << 2 /* SUPSYS_FLAG_CONNECT_PNPGROUP */
};

static const unsigned char FAT12_FLAGS_RDR[] = 
{
      0 << 7 /* RDR_FLAG_RDR_LONG_NAMES */
    | 1 << 6 /* RDR_FLAG_RDR_REMOVABLE */
#ifdef UNIX
    | 0 << 5 /* RDR_FLAG_RDR_UNIQUE */
#else /* UNIX */
    | 1 << 5 /* RDR_FLAG_RDR_UNIQUE */
#endif /* UNIX */
    | 1 << 4 /* RDR_FLAG_RDR_FOLDERS */
#ifndef UNDER_CE
    | 1 << 3, /* RDR_FLAG_RDR_FOLDER_ROOT */
#endif
};

static const unsigned char FAT12_FLAGS_GRP_CONNECT[] = 
{
      0 << 7 /* SUPSYS_FLAG_CONNECT_MANY */
    | 0 << 6 /* SUPSYS_FLAG_CONNECT_COM */
    | 0 << 5 /* SUPSYS_FLAG_CONNECT_LPT */
    | 0 << 4 /* SUPSYS_FLAG_CONNECT_AUTO */
    | 0 << 3 /* SUPSYS_FLAG_CONNECT_MULTY */
    | 1 << 2 /* SUPSYS_FLAG_CONNECT_PNPGROUP */
};

/*++++
 * Функция получения системных флагов.
 ++++*/
TSupErr fat12_info_system_flag(
    TSupSysContext *context, 
    TSupSysInfo *info ) /*+ (io) Запрашиваемая информация. +*/
/*+
 * Возвращает:
 *
 * SUP_ERR_NO, при успешном получении имени дисковода.
 *
 * SUP_ERR_PARAM, при неправильно переданных параметрах.
 +*/
{
    TSupSysInfoFlags *flags = (TSupSysInfoFlags*)info;
    size_t length;
    const unsigned char *ptr = NULL;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoFlags );
    SUPSYS_PRE( flags->type == RDR_FLAG_BLOCK_RDR
      || flags->type == SUPSYSE_FLAG_BLOCK_GENERAL
      || flags->type == SUPSYS_FLAG_BLOCK_CONNECT );
    length = flags->string.bits;
    if( flags->type == SUPSYSE_FLAG_BLOCK_GENERAL )
    {
	length = SUPSYSE_FLAG_GENERAL_VERSION_1;
	ptr = FAT12_FLAGS_GENERAL;
    }
    if( flags->type == SUPSYS_FLAG_BLOCK_CONNECT )
    {
	length = SUPSYS_FLAG_CONNECT_VERSION_3;
	ptr = FAT12_FLAGS_CONNECT;
    }
    if( flags->type == RDR_FLAG_BLOCK_RDR )
    {
	length = RDR_FLAG_RDR_VERSION_1;
	ptr = FAT12_FLAGS_RDR;
    }
    if( flags->string.string == NULL )
    {
	flags->string.bits = length;
	return SUP_ERR_NO;
    }
    if( flags->string.bits > length )
	flags->string.bits = length;
    memcpy( flags->string.string, ptr, 
	flags->string.bits / 8 + ( flags->string.bits % 8 ? 1 : 0 ) );
    flags->string.bits = length;
    return SUP_ERR_NO;
}

TSupErr fat12_group_info_system_flag(
				     TSupSysContext *context, 
				     TSupSysInfo *info )
{
    TSupSysInfoFlags *flags = (TSupSysInfoFlags*)info;
    size_t length;
    const unsigned char *ptr = NULL;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoFlags );
    SUPSYS_PRE( flags->type == RDR_FLAG_BLOCK_RDR
	|| flags->type == SUPSYSE_FLAG_BLOCK_GENERAL
	|| flags->type == SUPSYS_FLAG_BLOCK_CONNECT );
    length = flags->string.bits;
    if( flags->type == SUPSYSE_FLAG_BLOCK_GENERAL )
    {
	length = SUPSYSE_FLAG_GENERAL_VERSION_1;
	ptr = FAT12_FLAGS_GENERAL;
    }
    if( flags->type == SUPSYS_FLAG_BLOCK_CONNECT )
    {
	length = SUPSYS_FLAG_CONNECT_VERSION_3;
	ptr = FAT12_FLAGS_GRP_CONNECT;
    }
    if( flags->type == RDR_FLAG_BLOCK_RDR )
    {
	length = RDR_FLAG_RDR_VERSION_1;
	ptr = FAT12_FLAGS_RDR;
    }
    if( flags->string.string == NULL )
    {
	flags->string.bits = length;
	return SUP_ERR_NO;
    }
    if( flags->string.bits > length )
	flags->string.bits = length;
    memcpy( flags->string.string, ptr, 
	flags->string.bits / 8 + ( flags->string.bits % 8 ? 1 : 0 ) );
    flags->string.bits = length;
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12iflag.c 60080 2010-02-16 08:37:02Z dim $ +*/
