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

#include "fat12prj.h" /* Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). */

/*!
 * \ingroup fat12_info_general
 * \brief ������� ����������� ��������� �����������.
 * \return ��� ������
 * \retval SUP_ERR_NO �������� ���������.
 * \retval SUP_ERR_PARAM �������� ������������ ���������.
 */
TSupErr fat12_context_free( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoContextFree *inf = (TSupSysInfoContextFree*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    /* ������� ������� ����������. */
    SUPSYS_PRE_INFO( info, TSupSysInfoContextFree );

    inf->size_of = sizeof( TSupSysInfoContextFree );
    if( ctx == NULL )
	return SUP_ERR_NO;
    free(ctx->path);
    free(ctx->path_to_item);
    free(ctx);
    return SUP_ERR_NO;
}
/* end of file: $Id: f12cfree.c 54220 2009-05-02 02:33:10Z lse $ */
