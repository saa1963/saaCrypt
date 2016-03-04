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
 * $Id: f12kunl.c 75486 2012-03-04 12:45:49Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ������� ������ / ������ �������� ���������� � �������� �������.
 * ������� �������� �����.
 *
 * �������: fat12_unlink
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). +*/

/*++++ 
 * ������� �������� �����.
 ++++*/
TSupErr fat12_unlink( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TCHAR *file_name;
    TCHAR *fname;
    TReaderInfoUnlink *inf = (TReaderInfoUnlink*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoUnlink );
    SUPSYS_PRE_STRING_PTRS( inf->name.text, inf->name.length );
#ifdef UNIX
    SUPSYS_PRE( ctx->handle == NULL );
#else
    SUPSYS_PRE( ctx->handle == INVALID_HANDLE_VALUE );
#endif
    SUPSYS_PRE( ctx->path != NULL );

    inf->size_of = sizeof( TReaderInfoUnlink );
    fname = inf->name.text;
    file_name = malloc( ( _tcslen( fname )
	+ 2 + _tcslen( ctx->path )
	+ 1 + ( ctx->folder ? _tcslen( ctx->folder ) : 0 )
	+ 1 /* '\0' */ ) * sizeof( TCHAR ) );

    if( file_name == NULL )
	return SUP_ERR_MEMORY;
    _tcscpy( file_name, ctx->path );
    if( ctx->folder )
    {
	_tcscat( file_name, ctx->folder );
	_tcscat( file_name, _TEXT( "/" ) );
    }
    _tcscat( file_name, fname );
#ifdef UNIX
    if( remove( file_name ) )
#else
    if( !DeleteFile( file_name ) )
#endif /* UNIX*/
    {
	TSupErr code = fat12_os_error();
	free( file_name );
	return code;
    }
    free( file_name );
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12kunl.c 75486 2012-03-04 12:45:49Z dim $ +*/
