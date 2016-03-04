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

/*!
 * \file $RCSfile$
 * \version $Revision: 42536 $
 * \date $Date: 2007-09-26 17:50:44 +0400 (Wed, 26 Sep 2007) $
 * \author $Author: chudov $
 * \brief Функция получения флагов считывателя.
 */

#include "fat12prj.h"

/*!
 * \ingroup hdimage_internal
 * \brief Флаги считывателя.
 */
static unsigned char HDIMAGE_FLAGS_GENERAL[] = 
{ 
    0 << 7 
};

static unsigned char HDIMAGE_FLAGS_CONNECT[] = 
{
    0 << 7 /* SUPSYS_FLAG_CONNECT_MANY */
    | 0 << 6 /* SUPSYS_FLAG_CONNECT_COM */
    | 0 << 5 /* SUPSYS_FLAG_CONNECT_LPT */
    | 0 << 4 /* SUPSYS_FLAG_CONNECT_AUTO */
    | 0 << 3 /* SUPSYS_FLAG_CONNECT_MULTY */
    | 0 << 2 /* SUPSYS_FLAG_CONNECT_PNPGROUP */
};

static unsigned char EHDIMAGE_FLAGS_RDR[] = 
{
    0 << 7 /* RDR_FLAG_RDR_LONG_NAMES */
    | 0 << 6 /* RDR_FLAG_RDR_REMOVABLE */
    | 0 << 5 /* RDR_FLAG_RDR_UNIQUE */
    | 1 << 4 /* RDR_FLAG_RDR_FOLDERS */
    | 1 << 3 /* RDR_FLAG_RDR_FOLDER_ROOT */
    | 1 << 2, /* RDR_FLAG_RDR_PROTECTED */
};

/*! 
 * \ingroup hdimage_fun_general
 * \brief Функция получения флагов считывателя.
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TReaderInfoSystemFlag
 * \sa #TReaderInfoSystemFlag, #READER_FUN_SYSTEM_FLAG, 
 *  #supsys_flag
 * \return Код ошибки
 */
TSupErr ehdimage_info_system_flag(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoFlags *flags = (TSupSysInfoFlags*)info;
    size_t length;
    unsigned char *ptr = NULL;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoFlags );
    SUPSYS_PRE( flags->type <= RDR_FLAG_BLOCK_RDR );
    SUPSYS_PRE( flags->type == RDR_FLAG_BLOCK_RDR
      || flags->type == SUPSYSE_FLAG_BLOCK_GENERAL
      || flags->type == SUPSYS_FLAG_BLOCK_CONNECT );
    length = flags->string.bits;
    if( flags->type == SUPSYSE_FLAG_BLOCK_GENERAL )
    {
	length = SUPSYSE_FLAG_GENERAL_VERSION_1;
	ptr = HDIMAGE_FLAGS_GENERAL;
    }
    if( flags->type == SUPSYS_FLAG_BLOCK_CONNECT )
    {
	length = SUPSYS_FLAG_CONNECT_VERSION_3;
	ptr = HDIMAGE_FLAGS_CONNECT;
    }
    if( flags->type == RDR_FLAG_BLOCK_RDR )
    {
	length = RDR_FLAG_RDR_VERSION_2;
	ptr = EHDIMAGE_FLAGS_RDR;
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

/* end of file: $Id: ehdiflag.c 42536 2007-09-26 13:50:44Z chudov $ */
