/*
 * Copyright(C) 2000-2006 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� �����oi�����������
 * ���������� ���������� � ��������� ������ ���.
 */

/*!
 * \file $RCSfile$
 * \version $Revision: 44029 $
 * \date $Date: 2007-12-25 17:37:31 +0400 (Tue, 25 Dec 2007) $
 * \author $Author: maxdm $
 *
 * \brief ������� ����������� ��� ������ � cpdrvlib
 *
 * �������� ����������� IOCTL, �������������� cpdrvlib, � ����� �����������
 * ���� ��������� ���������.
 */
#ifndef _CPDRVLIB_H_INCLUDED
#define _CPDRVLIB_H_INCLUDED

#ifndef CTL_CODE
#include <winioctl.h>
#endif // CTL_CODE

#define IOCTL_GETCPCCREATEPROVIDER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0810, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_GETCPCGETDEFAULTCONFIG CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0811, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_GETCPCINITMEMORYLF CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0812, METHOD_BUFFERED, FILE_READ_ACCESS)

#define CPDRVLIB_NAME L"CPDrvLib"
#define CPDRVLIB_NT_DEVICE_NAME L"\\Device\\" CPDRVLIB_NAME
#define CPDRVLIB_DOS_DEVICE_NAME L"\\DosDevices\\Global\\" CPDRVLIB_NAME

#define CPKSP_NAME L"CPKSP"
#define CPKSP_NT_DEVICE_NAME L"\\Device\\" CPKSP_NAME
#define CPKSP_DOS_DEVICE_NAME L"\\DosDevices\\Global\\" CPKSP_NAME


#endif // _CPDRVLIB_H_INCLUDED
