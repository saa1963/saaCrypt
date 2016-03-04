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

static const TSupSysInfoIDSBaseItem EHDIMAGE_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_EHDIMAGE_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_HDIMAGE, SUPSYS_IDS_ICON_QUANT_V1 },
    { READER_IDS_BLOCK, IDS_EHDIMAGE_INSERT_TEXT, READER_IDS_QUANT_V1 }
};

/*!
 * \ingroup fat12_internal
 * \brief �������� �������� ������� ��� HDIMAGE
 */
static const TSupSysInfoIDSBase EHDIMAGE_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( EHDIMAGE_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    EHDIMAGE_IDS_ITEMS
};

/*!
 * \ingroup hdimage_fun_general
 * \brief ������� ������������ �������� ������� ��� EHDIMAGE
 * \param context [in] �������� �����������. �� ������������.
 * \param info [out] ��������� #TSupSysInfoIDSBase
 * \return ��� ������.
 * \retval #SUP_ERR_NO �������� ������� ����������.
 * \retval #SUP_ERR_PARAM ��� ������������ ����������
 */
TSupErr ehdimage_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &EHDIMAGE_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: ehdids.c 42536 2007-09-26 13:50:44Z chudov $ */
