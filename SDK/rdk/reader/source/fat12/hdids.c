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

#include "fat12prj.h"

static const TSupSysInfoIDSBaseItem HDIMAGE_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_HDIMAGE_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_HDIMAGE, SUPSYS_IDS_ICON_QUANT_V1 },
    { READER_IDS_BLOCK, IDS_HDIMAGE_INSERT_TEXT, READER_IDS_QUANT_V1 }
};

/*!
 * \ingroup fat12_internal
 * \brief �������� �������� ������� ��� HDIMAGE
 */
static const TSupSysInfoIDSBase HDIMAGE_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( HDIMAGE_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    HDIMAGE_IDS_ITEMS
};

/*!
 * \ingroup hdimage_fun_general
 * \brief ������� ������������ �������� ������� ��� HDIMAGE
 * \param context [in] �������� �����������. �� ������������.
 * \param info [out] ��������� #TSupSysInfoIDSBase
 * \return ��� ������.
 * \retval #SUP_ERR_NO �������� ������� ����������.
 * \retval #SUP_ERR_PARAM ��� ������������ ����������
 */
TSupErr hdimage_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &HDIMAGE_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: hdids.c 37543 2006-11-27 16:07:59Z chudov $ */
