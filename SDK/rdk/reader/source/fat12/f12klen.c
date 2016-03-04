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

/****
 * $Id: f12klen.c 75486 2012-03-04 12:45:49Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ������� ������ / ������ �������� ���������� � �������� �������.
 * ������� ��������� ����� �����.
 *
 * �������: fat12_length
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). +*/

/*++++
 * ������� ��������� ����� �����.
 ++++*/
TSupErr fat12_length(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoLength *inf = (TReaderInfoLength*)info;
    TFat12Context *ctx = (TFat12Context*)context;
#ifndef UNIX
    BY_HANDLE_FILE_INFORMATION finfo;
#endif /* UNIX */

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoLength );

    inf->size_of = sizeof( TReaderInfoLength );
#ifdef UNIX
    if( fseek( ctx->handle, 0, SEEK_END ) )
	return fat12_os_error();
    inf->length = (size_t)ftell( ctx->handle );
    if( inf->length == (size_t)-1 )
	return fat12_os_error();
#else /* UNIX */
    if( !GetFileInformationByHandle( ctx->handle, &finfo ) )
	return fat12_os_error();
    inf->length = finfo.nFileSizeLow;
    if( finfo.nFileSizeHigh )
	inf->length = (size_t)-1;
#endif /* UNIX */
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12klen.c 75486 2012-03-04 12:45:49Z dim $ +*/
