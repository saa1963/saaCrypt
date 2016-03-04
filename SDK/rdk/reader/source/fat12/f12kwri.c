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
 * $Id: f12kwri.c 44147 2008-01-10 14:18:38Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ������� ������ / ������ �������� ���������� � �������� �������.
 * ������� ������ � ����.
 *
 * �������: fat12_write
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). +*/
#include "limits.h" 
/*++++ 
 * ������� ������ � ����. 
 ++++*/
TSupErr fat12_write( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoWrite *inf = (TReaderInfoWrite*)info;
    TFat12Context *ctx = (TFat12Context*)context;

#ifdef UNIX
    size_t loc_writen;
#else /* UNIX */
    DWORD loc_writen = 0;
    void *buffer = (void*)inf->info.info;
#endif /* UNIX */

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoWrite );
    SUPSYS_PRE( inf->from <= LONG_MAX );
    SUPSYS_PRE( inf->info.length <= LONG_MAX );

    SUPSYS_PRE_READ_PTRS( inf->info.info, inf->info.length );

    inf->size_of = sizeof( TReaderInfoWrite );
#ifdef UNIX
    if( ctx->handle == NULL )
#else
    if( ctx->handle == INVALID_HANDLE_VALUE )
#endif
	return RDR_ERR_MEDIA_CHANGED;
#ifdef UNIX
    if( fseek( ctx->handle, inf->from, SEEK_SET ) )
	return fat12_os_error();
    loc_writen = fwrite( inf->info.info, 1, inf->info.length, ctx->handle );
    if( loc_writen != inf->info.length )
	return fat12_os_error();
#else /* UNIX */
    if( SetFilePointer( ctx->handle, (LONG)inf->from, NULL, 
	FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	return fat12_os_error();
    if( !WriteFile( ctx->handle, buffer, (LONG)inf->info.length, &loc_writen, 
	    NULL ) )
	return fat12_os_error();
#endif /* UNIX */
    inf->info.length -= loc_writen;
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kwri.c 44147 2008-01-10 14:18:38Z dim $ +*/
