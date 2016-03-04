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


#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

TSupErr fat12_info_instance(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoInstance *inf = (TSupSysInfoInstance*)info;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoInstance );
    inf->instance = FAT12_RESOURCE;
    inf->size_of = sizeof( TSupSysInfoInstance );
    return SUP_ERR_NO;
}
/*+ end of file: $Id: f12iinst.c 4025 2001-10-03 10:40:40Z cav $ +*/
