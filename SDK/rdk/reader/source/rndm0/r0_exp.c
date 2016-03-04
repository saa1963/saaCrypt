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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 *
 * \brief Полная реализация ДСЧ: 0 последовательность(RNDM0).
 *
 * Проект: Датчик случайных чисел: 0 последовательность(RNDM0).
 *
 * Константа: RNDM0_HANDLE
 *
 * Функция: rndm0_rndm_get_handle, rndm0_rndm
 */

#include "r0_prj.h" /*+ Project (READER/RNDM/RNDM0) include.
    Мастер include файл для проекта (READER/RNDM/RNDM0). +*/
#include "Win32/rndm0_rc.h"
#include "reader.kit/rdr_ver.h"

/* Функция получения случайного числа. */
static TSupErr rndm0_rndm(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция получения типа ДСЧ. */
static TSupErr rndm0_type(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция получения типа ДСЧ. */
static TSupErr fapsi0_type(
    TSupSysContext *context, TSupSysInfo *info );

/* Функция получения instance. */
static TSupErr rndm0_instance(
    TSupSysContext *context, TSupSysInfo *info );

#if defined( SUPPORT_RESOURCE_STD )
extern TSupResourceInstance RNDM0_RESOURCE;
#else
#define RNDM0_RESOURCE RNDM0_DLL_INSTANCE
#endif

static const TSupSysInfoIDSBaseItem FAPSI0_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_FAPSI0_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
};

/*!
 * \ingroup dallas_internal
 * \brief Описание смещений ресурса для DS1410E
 */
static const TSupSysInfoIDSBase FAPSI0_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( FAPSI0_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    FAPSI0_IDS_ITEMS
};

/*!
 * \ingroup dallas_fun_general
 * \brief Функция возвращающая смещение ресурса для DS1410E
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TSupSysInfoIDSBase
 * \return код ошибки.
 * \retval #SUP_ERR_NO смещение ресурса возвращено.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
 */
static TSupErr fapsi0_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &FAPSI0_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}

/*!
 * \ingroup rndm0_internal
 * \brief Массив интерфейсных функций считывателя с номерами.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #rndm0_funs
 */
static const TSupSysFunctionTableItem RNDM0_FUNS[] = 
{
    { SUPSYS_FUN_INSTANCE, rndm0_instance },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},

    { RANDOM_FUN_TYPE, rndm0_type },
    { RANDOM_FUN_RNDM, rndm0_rndm }
};

/*!
 * \ingroup rndm0_internal
 * \brief Массив интерфейсных функций считывателя с номерами.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #rndm0_funs
 */
static const TSupSysFunctionTableItem FAPSI0_FUNS[] = 
{
    { SUPSYS_FUN_INSTANCE, rndm0_instance },
    { SUPSYS_FUN_IDS_BLOCK, fapsi0_info_ids },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
    { RANDOM_FUN_TYPE, fapsi0_type },
    { RANDOM_FUN_RNDM, rndm0_rndm }
};

/*!
 * \ingroup acr_internal
 * \brief Точка входа в ДСЧ.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #rndm0_funs
 */
static const TSupSysFunctionTable RNDM0_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( RNDM0_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    RNDM0_FUNS
};

/*!
 * \ingroup acr_internal
 * \brief Точка входа в ДСЧ.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #rndm0_funs
 */
static const TSupSysFunctionTable FAPSI0_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( FAPSI0_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    FAPSI0_FUNS
};


/*! \ingroup rndm0_exp
 * \brief Получение точки входа в библиотеку.
 *
 * Функция возвращающая точку входа в библиотеку.
 * \return Точку входа в библиотеку.
 */
const TRndmHandle* rndm0_rndm_get_handle( void )
{
    return (const TRndmHandle*)&RNDM0_TABLE;
}

/*! \ingroup rndm0_exp
 * \brief Получение точки входа в библиотеку.
 *
 * Функция возвращающая точку входа в библиотеку.
 * \return Точку входа в библиотеку.
 */
const TRndmHandle* fapsi0_rndm_get_handle( void )
{
    return (const TRndmHandle*)&FAPSI0_TABLE;
}

/*! \ingroup rndm0_sys
 * \brief Функция получения случайного числа.
 *
 * Функция заполнения буфера, переданного для случайного числа, константной
 * последовательностью.
 * \param context [in/out] контекст ДСЧ (не используется)
 * \param info [out] 
 * \return всегда SUP_ERR_NO
 */
static TSupErr rndm0_rndm( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TRandomInfoRndm *inf = (TRandomInfoRndm*)info;
    UNUSED( context );
    SUPSYS_PRE_INFO( info, TRandomInfoRndm );
    inf->size_of = sizeof( TRandomInfoRndm );
    memset( inf->info.info, 0xaa, inf->info.length );
    return SUP_ERR_NO;
}

/*!
 * \brief Функция получения типа ДСЧ.
 *
 * Функция возвращает в качестве типа ДСЧ, поддержку всевозможных типов.
 * \param type [out] тип ДСЧ.
 * \return всегда SUP_ERR_NO
 */
static TSupErr rndm0_type( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TRandomInfoType *inf = (TRandomInfoType*)info;
    UNUSED( context );
    SUPSYS_PRE_INFO( info, TRandomInfoType );
    inf->type = RNDM_TYPE_WINDOWLESS | RNDM_TYPE_CERTIFIED | RNDM_TYPE_HARD;
    inf->size_of = sizeof( TRandomInfoType );
    return SUP_ERR_NO;
}

/*!
 * \brief Функция получения типа ДСЧ.
 *
 * Функция возвращает в качестве типа ДСЧ, поддержку всевозможных типов.
 * \param type [out] тип ДСЧ.
 * \return всегда SUP_ERR_NO
 */
static TSupErr fapsi0_type( 
    TSupSysContext *context, TSupSysInfo *info )
{
    TRandomInfoType *inf = (TRandomInfoType*)info;
    UNUSED( context );
    SUPSYS_PRE_INFO( info, TRandomInfoType );
    inf->type = RNDM_TYPE_WINDOWLESS | RNDM_TYPE_CERTIFIED | 
	RNDM_TYPE_FAPSI_GAMMA;
    inf->size_of = sizeof( TRandomInfoType );
    return SUP_ERR_NO;
}

/* Функция получения instance. */
static TSupErr rndm0_instance(
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoInstance *inf = (TSupSysInfoInstance*)info;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoInstance );
    inf->size_of = sizeof( TSupSysInfoInstance );
    inf->instance = RNDM0_RESOURCE;
    return SUP_ERR_NO;
}
/* end of file: $Id: r0_exp.c 54220 2009-05-02 02:33:10Z lse $ */
