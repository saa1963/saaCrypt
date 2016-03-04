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
 * \version $Revision: 4475 $
 * \date $Date: 2001-11-06 14:54:11 +0400 (Tue, 06 Nov 2001) $
 * \author $Author: cav $
 * \brief ������� ��������� ������� ������.
 */

#include "ric_prj.h"

/*! 
 * \ingroup ric_fun_general
 * \brief ������� ��������� ������� ������.
 * \param context [in] �������� �����������. �� ������������.
 * \param info [in/out] ��������� #TReaderInfoPasswdInfo
 * \sa #TReaderInfoPasswdType, #READER_FUN_PASSWD_TYPE, 
 *  #TSupSysFunction, #TReaderInfoPasswdInfo, #READER_FUN_PASSWD_LENGTH
 */
TSupErr ric_passwd_length(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoPasswdInfo *inf = (TReaderInfoPasswdInfo*)info;

    SUPSYS_PRE_INFO( info, TReaderInfoPasswdInfo );
    SUPSYS_PRE( inf->type == RDR_LOGIN_INFO_PASSWD )

    UNUSED( context );
    inf->size_of = sizeof( TReaderInfoPasswdInfo );
    inf->max_length = 8;
    inf->min_length = 0;
    return SUP_ERR_NO;
}
/* end of file:$Id: ric_plen.c 4475 2001-11-06 10:54:11Z cav $ */
