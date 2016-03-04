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
 * \version $Revision: 4395 $
 * \date $Date: 2001-10-31 18:44:33 +0400 (Wed, 31 Oct 2001) $
 * \author $Author: cav $
 * \brief ������� �������� �����.
 */

#include "fat12prj.h"

/*!
 * \ingroup fat12_fun_reader
 * \brief ������� �������� �����.
 * \param context [in] �������� �����������. 
 * \param info [in/out] ��������� #TReaderInfoFolderClose
 * \sa TReaderInfoFolderClose, READER_FUN_FOLDER_CLOSE, 
 *  TSupSysFunction, fat12_folder_open, #rdr_folder_open, #rdr_folder_close
 */
TSupErr fat12_folder_close( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoFolderClose );

    if( ctx->folder )
    {
	free( ctx->folder );
	ctx->folder = NULL;
    }
    return SUP_ERR_NO;
}
/* end of file: $Id: f12dcl.c 4395 2001-10-31 14:44:33Z cav $ */
