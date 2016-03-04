/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

/*!
 * \file $RCSfile$
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 *
 * \brief ������ ���������� ���: 0 ������������������(RNDM0).
 *
 * ������: ������ ��������� �����: 0 ������������������(RNDM0).
 *
 * ���������: RNDM0_HANDLE
 *
 * �������: rndm0_rndm_get_handle, rndm0_rndm
 */

#include "r0_prj.h" /*+ Project (READER/RNDM/RNDM0) include.
    ������ include ���� ��� ������� (READER/RNDM/RNDM0). +*/
#include "Win32/rndm0_rc.h"
#include "reader.kit/rdr_ver.h"

/* ������� ��������� ���������� �����. */
static TSupErr rndm0_rndm(
    TSupSysContext *context, TSupSysInfo *info );

/* ������� ��������� ���� ���. */
static TSupErr rndm0_type(
    TSupSysContext *context, TSupSysInfo *info );

/* ������� ��������� ���� ���. */
static TSupErr fapsi0_type(
    TSupSysContext *context, TSupSysInfo *info );

/* ������� ��������� instance. */
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
 * \brief �������� �������� ������� ��� DS1410E
 */
static const TSupSysInfoIDSBase FAPSI0_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( FAPSI0_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    FAPSI0_IDS_ITEMS
};

/*!
 * \ingroup dallas_fun_general
 * \brief ������� ������������ �������� ������� ��� DS1410E
 * \param context [in] �������� �����������. �� ������������.
 * \param info [out] ��������� #TSupSysInfoIDSBase
 * \return ��� ������.
 * \retval #SUP_ERR_NO �������� ������� ����������.
 * \retval #SUP_ERR_PARAM ��� ������������ ����������
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
 * \brief ������ ������������ ������� ����������� � ��������.
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
 * \brief ������ ������������ ������� ����������� � ��������.
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
 * \brief ����� ����� � ���.
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
 * \brief ����� ����� � ���.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #rndm0_funs
 */
static const TSupSysFunctionTable FAPSI0_TABLE =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( FAPSI0_FUNS ) / sizeof( TSupSysFunctionTableItem ),
    FAPSI0_FUNS
};


/*! \ingroup rndm0_exp
 * \brief ��������� ����� ����� � ����������.
 *
 * ������� ������������ ����� ����� � ����������.
 * \return ����� ����� � ����������.
 */
const TRndmHandle* rndm0_rndm_get_handle( void )
{
    return (const TRndmHandle*)&RNDM0_TABLE;
}

/*! \ingroup rndm0_exp
 * \brief ��������� ����� ����� � ����������.
 *
 * ������� ������������ ����� ����� � ����������.
 * \return ����� ����� � ����������.
 */
const TRndmHandle* fapsi0_rndm_get_handle( void )
{
    return (const TRndmHandle*)&FAPSI0_TABLE;
}

/*! \ingroup rndm0_sys
 * \brief ������� ��������� ���������� �����.
 *
 * ������� ���������� ������, ����������� ��� ���������� �����, �����������
 * �������������������.
 * \param context [in/out] �������� ��� (�� ������������)
 * \param info [out] 
 * \return ������ SUP_ERR_NO
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
 * \brief ������� ��������� ���� ���.
 *
 * ������� ���������� � �������� ���� ���, ��������� ������������ �����.
 * \param type [out] ��� ���.
 * \return ������ SUP_ERR_NO
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
 * \brief ������� ��������� ���� ���.
 *
 * ������� ���������� � �������� ���� ���, ��������� ������������ �����.
 * \param type [out] ��� ���.
 * \return ������ SUP_ERR_NO
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

/* ������� ��������� instance. */
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
