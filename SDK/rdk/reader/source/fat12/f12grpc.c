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

/****
* $Id: f12grpc.c 54220 2009-05-02 02:33:10Z lse $
*
* Работа со считывателями и ключевой информацией (READER).
*
* Библиотека чтения / записи ключевой информации на дискеты (FAT12).
*
* Функции поддержки информации с ключевой дискеты.
* Функция получения следующего типа подключения.
*
* Функция: fat12_group_enum_close
****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
Мастер include файл для проекта (READER/FAT12). +*/

TSupErr fat12_group_enum_close(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoGroupEnumClose *inf = (TSupSysInfoGroupEnumClose*)info;
    TFat12ConnectEnum *enum_ptr;

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoGroupEnumClose );
    inf->size_of = sizeof( TSupSysInfoGroupEnumClose );
    enum_ptr = (TFat12ConnectEnum*)inf->add_info.info;
    if( enum_ptr == NULL )
    {
	inf->add_info.length = 0;
	return SUP_ERR_NO;
    }
#ifdef UNIX
    if( enum_ptr->drive_strings ){
	TFat12ConnectEnumElem *el=enum_ptr->drive_strings;

	for(enum_ptr->cur=el;enum_ptr->cur;enum_ptr->cur=el){
	    el=enum_ptr->cur->next;
	    free(enum_ptr->cur->connect_str);
	    free(enum_ptr->cur);
	}
    }
#elif UNDER_CE
    if( enum_ptr->hSearch && enum_ptr->hSearch != INVALID_HANDLE_VALUE )
	CloseHandle(enum_ptr->hSearch);
#else /* UNIX */
    if( enum_ptr->drive_strings )
	free( enum_ptr->drive_strings );
#endif
    free( enum_ptr );
    inf->add_info.length = 0;
    inf->add_info.info = NULL;
    return SUP_ERR_NO;
}

/*+ end of file: $Id: f12grpc.c 54220 2009-05-02 02:33:10Z lse $ +*/
