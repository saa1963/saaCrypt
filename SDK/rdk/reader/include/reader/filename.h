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
 * \file $RCSfile: filename.h,v $
 * \version $Revision: 1.13 $
 * \date $Date: 2009/05/02 02:30:57 $
 * \author $Author: lse $
 *
 * \brief ������� �������� ������.
 *
 * ������: Filenames
 *
 */

#ifndef RDR_FILENAME_H
#define RDR_FILENAME_H

#ifdef SOLARIS
#define CSP_FLOPPY_ROOT "/floppy/floppy"
#else
#define CSP_FLOPPY_ROOT CSP_VOLATILE_ROOT "/mnt/"
#endif

#define CSP_USERS_DIR CSP_VOLATILE_ROOT "/users"
#define CSP_PROTECTED_DIR CSP_VOLATILE_ROOT "/keys"
#define CSP_DSRF_DIR CSP_VOLATILE_ROOT "/dsrf"

#endif /* RDR_FILENAME_H */
/* end of file:$Id: filename.h,v 1.13 2009/05/02 02:30:57 lse Exp $ */
