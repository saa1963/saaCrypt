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
 * \version $Revision: 44146 $
 * \date $Date: 2008-01-10 18:15:59 +0400 (Thu, 10 Jan 2008) $
 * \author $Author: dim $
 * \brief ����� ����� � �����������.
 */

#include "fat12prj.h"
#include "reader.kit/rdr_ver.h"

/*!
 * \ingroup hdimage_internal
 * \brief ������ ������������ ������� ����������� � ��������.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #hdimage_funs
 */
static const TSupSysFunctionTableItem ehdimage_funs[] =
{
    { SUPSYS_FUN_INSTANCE, fat12_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, fat12_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, fat12_context_free },
    { SUPSYS_FUN_IDS_BLOCK, ehdimage_info_ids },
    { SUPSYS_FUN_FLAGS, ehdimage_info_system_flag },
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},

    { SUPSYS_FUN_REGISTER, hdimage_register },
    { SUPSYS_FUN_UNREGISTER, fat12_unregister },

    { READER_FUN_LOCAL_MACHINE, hdimage_local_machine },

    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_folder_enum_close },
    { READER_FUN_FOLDER_OPEN, fat12_folder_open },
    { READER_FUN_FOLDER_CLOSE, fat12_folder_close },

    { READER_FUN_FOLDER_CLEAR, fat12_folder_clear },
    { READER_FUN_OPEN, fat12_open },
    { READER_FUN_CLOSE, fat12_close },
    { READER_FUN_UNLINK, fat12_unlink },

    { READER_FUN_READ, fat12_read },
    { READER_FUN_WRITE, fat12_write },
    { READER_FUN_CHSIZE, fat12_chsize },
    { READER_FUN_LENGTH, fat12_length },
};

/*!
 * \ingroup hdimage_internal
 * \brief ����� ����� � �����������.
 * \sa #TSupSysFunctionTable, #TSupSysFunctionTableItem, #hdimage_funs
 */
static const TSupSysFunctionTable ehdimage_table =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( ehdimage_funs ) / sizeof( TSupSysFunctionTableItem ),
    ehdimage_funs
};

/*!
 * \ingroup hdimage_external
 * \brief ������� ������������ ����� ����� � �����������.
 * \sa #rdr_register, #rdr_register_all, #TSupSysExportFunction
 * \return ��������� �� ����� ����� � �����������.
 * \internal ������������ ��������� �� #hdimage_table
 */
const TSupSysEHandle *
    hsm_reader_get_handle(
    void )
{
    return (const TSupSysEHandle*)&ehdimage_table;
}

/* end of file: $Id: ehdis.c 44146 2008-01-10 14:15:59Z dim $ */
