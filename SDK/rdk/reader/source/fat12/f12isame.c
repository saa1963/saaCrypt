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

/*!
 * \ingroup fat12_info_same_media
 * \brief ������� �������� ������������ ������������ �������� ���������.
 * \param context [in] �������� �����������. 
 * \param info [out] ��������� #TReaderInfoSameMedia
 * \sa #TReaderInfoSameMedia, #TReaderInfoSetRetry, READER_FUN_SAME_MEDIA, 
 *  #rdr_retry_proc_set
 * \return ��� ������.
 * \retval #SUP_ERR_NO �������� ���������.
 * \retval #SUP_ERR_PARAM ��� ������������ ����������
 */
TSupErr fat12_info_same_media( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoSameMedia *inf = (TReaderInfoSameMedia*)info;
    TFat12Context *ctx = (TFat12Context*)context;
    TSupErr code;
    TCHAR *cur_volume_name;
    unsigned long cur_serial;

    SUPSYS_PRE_INFO( info, TReaderInfoSameMedia );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );

    inf->size_of = sizeof( TReaderInfoSameMedia );
    if( ctx->path == NULL )
	return SUP_ERR_NO;
    code = fat12_volume_name( ctx, ctx->path, &cur_volume_name, &cur_serial );
    if( code )
	return code;
    code = SUP_ERR_NO;
    if( ctx->volume_label != NULL
	|| ctx->volume_serial != (unsigned long)-1 )
    {
	if( (ctx->volume_serial != cur_serial)
	    || (ctx->volume_label && _tcsicmp( ctx->volume_label, cur_volume_name ) ) )
	    code = RDR_ERR_ANOTHER_MEDIA;
    }
    if( inf->unique.text == NULL || inf->unique.length == 0 )
    {
	fat12_unique_form( cur_serial, cur_volume_name,
	    &inf->unique.length, NULL );
	free( cur_volume_name );
	return code;
    }
    fat12_unique_form( cur_serial, cur_volume_name,
	&inf->unique.length, inf->unique.text );
    if( inf->flags & RDR_SAME_WRITE_AVAIL )
    {
        code = fat12_set_volume_label(ctx, cur_volume_name);
        if (!code){
	    free( cur_volume_name );
	    return RDR_ERR_WRITE_PROTECT; 
	}
    }
    free( cur_volume_name );
    return code;
}
/* end of file: $Id: f12isame.c 44148 2008-01-10 14:22:33Z dim $ */
