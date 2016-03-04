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
 * \ingroup fat12_info_general
 * \brief Функция уничтожения контекста считывателя.
 * \return Код ошибки
 * \retval SUP_ERR_NO контекст уничтожен.
 * \retval SUP_ERR_PARAM переданы неправильные параметры.
 */
TSupErr fat12_context_free( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoContextFree *inf = (TSupSysInfoContextFree*)info;
    TFat12Context *ctx = (TFat12Context*)context;

    /* Провека входных параметров. */
    SUPSYS_PRE_INFO( info, TSupSysInfoContextFree );

    inf->size_of = sizeof( TSupSysInfoContextFree );
    if( ctx == NULL )
	return SUP_ERR_NO;
    free(ctx->path);
    free(ctx->path_to_item);
    free(ctx);
    return SUP_ERR_NO;
}
/* end of file: $Id: f12cfree.c 54220 2009-05-02 02:33:10Z lse $ */
