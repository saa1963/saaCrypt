/*
 * Copyright(C) 2000 Проект ИОК
 *
 * Этот файл содержит информацию, являющуюся
 * собственностью компании Крипто Про.
 *
 * Любая часть этого файла не может быть скопирована,
 * исправлена, переведена на другие языки,
 * локализована или модифицирована любым способом,
 * откомпилирована, передана по сети с или на
 * любую компьютерную систему без предварительного
 * заключения соглашения с компанией Крипто Про.
 */

#include "fat12prj.h" /* Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). */

/*!
 * \ingroup fat12_info_same_media
 * \brief Функция проверки идентичности вставленного носителя контексту.
 * \param context [in] контекст считывателя. 
 * \param info [out] структура #TReaderInfoSameMedia
 * \sa #TReaderInfoSameMedia, #TReaderInfoSetRetry, READER_FUN_SAME_MEDIA, 
 *  #rdr_retry_proc_set
 * \return код ошибки.
 * \retval #SUP_ERR_NO носитель идентичен.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
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
