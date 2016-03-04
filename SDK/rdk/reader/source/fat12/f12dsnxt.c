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
 * \version $Revision: 12092 $
 * \date $Date: 2003-10-01 20:04:37 +0400 (Wed, 01 Oct 2003) $
 * \author $Author: chudov $
 * \brief ������� ������ ��������� �����.
 */

#include "fat12prj.h"

/*! 
 * \ingroup fat12_fun_reader
 * \brief ������� ������ ��������� �����.
 *
 * ������� ������ ��������� ����������.
 * \param context [in] �������� �����������. �� ������������.
 * \param info [in/out] ��������� #TReaderInfoFolderEnumNext
 * \sa #TReaderInfoFolderEnumNext, #READER_FUN_FOLDER_ENUM_NEXT,
 *  #TSupSysFunction, #fat12_info_locked_folder_enum_open, 
 *  #fat12_info_locked_folder_enum_close,
 * \return ��� ������
 */
TSupErr fat12_info_locked_folder_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumNext *inf = (TReaderInfoFolderEnumNext*)info;
    char * info_ptr;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumNext );
//    SUPSYS_PRE_STRING_PTRS( inf->name.text, inf->name.length );
    UNUSED( context );

    if( ( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
	return fat12_info_folder_enum_next (context, info);

    inf->size_of = sizeof( TReaderInfoFolderEnumNext );
    info_ptr = (char*)inf->add_info.info;

    if (* (size_t*) info_ptr >= inf->add_info.length)
	return RDR_ERR_FILE_NOT_FOUND;
    // TODO: check, that 0 is before end;
    _tcscpy ( inf->name.text, info_ptr + * (size_t*) info_ptr );
    inf->name.length = _tcslen( inf->name.text );
    * (size_t*) info_ptr += sizeof (TCHAR) * (inf->name.length + 1);
    return SUP_ERR_NO;
}

/* end of file: $Id: f12dsnxt.c 12092 2003-10-01 16:04:37Z chudov $ */
