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

#include "ric_prj.h"
#include "Win32/ric_rc.h"

static const TSupSysInfoIDSBaseItem OSCAR_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_OSCAR_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_OSCAR, SUPSYS_IDS_ICON_QUANT_V1 },
};

/*!
 * \ingroup oscar_internal
 * \brief �������� �������� ������� ��� �����
 */
static const TSupSysInfoIDSBase OSCAR_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( OSCAR_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    OSCAR_IDS_ITEMS
};

/*!
 * \ingroup oscar_fun_general
 * \brief ������� ������������ �������� ������� ��� �����
 * \param context [in] �������� �����������. �� ������������.
 * \param info [out] ��������� #TSupSysInfoIDSBase
 * \return ��� ������.
 * \retval #SUP_ERR_NO �������� ������� ����������.
 * \retval #SUP_ERR_PARAM ��� ������������ ����������
 */
TSupErr oscar_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &OSCAR_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: oscids.c 37543 2006-11-27 16:07:59Z chudov $ */
