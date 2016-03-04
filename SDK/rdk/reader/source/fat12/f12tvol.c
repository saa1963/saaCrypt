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

#if defined UNIX || defined WINCE
TSupErr fat12_set_volume_label(TFat12Context *ctx, TCHAR *label)
{
    UNUSED(ctx);
    UNUSED(label);
    return SUP_ERR_NO;
}

/*
 * ������� ��������� ��������� ������ � ����� ���� �� UNIX.
 */
TSupErr fat12_volume_name(
    TFat12Context *ctx,
    const TCHAR *path, 
    TCHAR **name, 
    unsigned long *serial )
{
    UNUSED(ctx);
    UNUSED(path);

    *serial = (unsigned long)-1;
    *name = malloc(12 * sizeof(TCHAR));
    if(!*name)
	return SUP_ERR_MEMORY;

    (*name)[0]=_TEXT('\0');
    return SUP_ERR_NO;
}

#else // !UNIX && !WINCE = Desktop Windows Win32

TSupErr fat12_set_volume_label(TFat12Context *ctx, TCHAR *label)
{
    if( !SetVolumeLabel( ctx->path, label ) )
	return fat12_os_error();
    return SUP_ERR_NO;
}

/*
 * ������� ��������� ��������� ������ � ����� ���� �� Windows Desktop.
 */
TSupErr fat12_volume_name(
    TFat12Context *ctx,
    const TCHAR *path, 
    TCHAR **name, 
    unsigned long *serial )
{
    TSupErr code = SUP_ERR_NO;
    DWORD maximumComponentLength; /* ������������ ������ �������� ����. */
    DWORD fileSystemFlags; /* ����� �������� �������. */

    UNUSED(ctx);
    *serial = (unsigned long)-1;
    *name = NULL;
    if( !GetVolumeInformation( path, NULL, 0, 
	serial, &maximumComponentLength, &fileSystemFlags,
	NULL, 0 ) ) {
	    code=fat12_os_error();
	    goto done;
    }
    *name = malloc( ( maximumComponentLength + 1 ) * sizeof( TCHAR ) );
    if( *name == NULL ){
	code=SUP_ERR_MEMORY;
	goto done;
    }
    if( !GetVolumeInformation( path, *name, 
	maximumComponentLength + 1 /*volume_name_size */, 
	serial, &maximumComponentLength, &fileSystemFlags,
	NULL, 0 ) ) {
	    code=fat12_os_error();
	    goto done;
    }

done:
    if(code!=SUP_ERR_NO){
	*serial = (unsigned long)-1;
	if(*name) free( *name );
	*name = NULL;
    }

    return code;
}
#endif // !UNIX && !WINCE = Desktop Windows Win32
