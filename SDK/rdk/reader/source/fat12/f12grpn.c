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
* $Id: f12grpn.c 75486 2012-03-04 12:45:49Z dim $
*
* Работа со считывателями и ключевой информацией (READER).
*
* Библиотека чтения / записи ключевой информации на дискеты (FAT12).
*
* Функции поддержки информации с ключевой дискеты.
* Функция получения следующего типа подключения.
*
* Функция: fat12_group_enum_next
****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
Мастер include файл для проекта (READER/FAT12). +*/

#ifndef UNDER_CE
/*++++
* Функция получения следующего типа подключения.
++++*/
TSupErr fat12_group_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoGroupEnumNext *inf = (TSupSysInfoGroupEnumNext*)info;
    TFat12ConnectEnum *enum_ptr;
    size_t pref_l = _tcslen(_TEXT("FAT12_"));

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoGroupEnumNext );
    SUPSYS_PRE_READ_PTRS( inf->add_info.info, sizeof( TFat12ConnectEnum ) );

    inf->size_of = sizeof( TSupSysInfoGroupEnumNext );
#if !defined( UNIX )
    enum_ptr = (TFat12ConnectEnum*)(inf->add_info.info);
    SUPSYS_PRE_STRING_PTR( enum_ptr->drive_strings );
    SUPSYS_PRE_STRING_PTR( enum_ptr->cur );

    while( *( enum_ptr->cur ) )
    {
	size_t l = _tcslen( enum_ptr->cur );
	if( GetDriveType( enum_ptr->cur ) == DRIVE_REMOVABLE )
	{
	    if( l + pref_l > inf->name.length )
		return SUP_ERR_PARAM;
	    _tcscpy( inf->name.text, _TEXT("FAT12_") );
	    _tcscat( inf->name.text, enum_ptr->cur );
	    inf->name.length = l + pref_l;
	    if( inf->name.text[inf->name.length - 1] == '\\' )
	    {
		inf->name.text[inf->name.length - 1] = 0;
		inf->name.length--;
	    }
	    if( inf->name.text[inf->name.length - 1] == ':' )
	    {
		inf->name.text[inf->name.length - 1] = 0;
		inf->name.length--;
	    }
	    enum_ptr->cur += l + 1;
	    return SUP_ERR_NO;
	}
	enum_ptr->cur += l + 1;
    }
    enum_ptr->cur = NULL;
    return SUP_ERR_CONNECT;
#else
    enum_ptr = (TFat12ConnectEnum*)(inf->add_info.info);

    while( enum_ptr->cur )
    {
	const size_t l = _tcslen( enum_ptr->cur->connect_str );
	if( 1 /* GetDriveType( enum_ptr->cur ) == DRIVE_REMOVABLE */ )
	{
	    if( l + pref_l > inf->name.length )
		return SUP_ERR_PARAM;
	    _tcscpy( inf->name.text, _TEXT("FAT12_") );
	    _tcscat( inf->name.text, enum_ptr->cur->connect_str );
	    inf->name.length = l + pref_l;
	    if( inf->name.text[l + pref_l - 1] == '\\' )
	    {
		inf->name.text[l + pref_l - 1] = 0;
		inf->name.length--;
	    }
	    enum_ptr->cur = enum_ptr->cur->next;
	    return SUP_ERR_NO;
	}
//	enum_ptr->cur = enum_ptr->cur->next;
    }
    return SUP_ERR_CONNECT;
#endif	/* !UNIX */
}

#else

TSupErr fat12_group_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoGroupEnumNext *inf = (TSupSysInfoGroupEnumNext*)info;
    TFat12ConnectEnum *enum_ptr;
    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoGroupEnumNext );
    if( inf->add_info.info == NULL )
	return SUP_ERR_CONNECT;

    enum_ptr = (TFat12ConnectEnum*)inf->add_info.info;

    for(;;) {
	// Test whether the file is really on a storage card, and
	// not just a directory in the root directory.
	// It must have both the directory attribute and
	// the temporary attribute.
	if (  (enum_ptr->fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
	    && (enum_ptr->fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	{
	    // TODO: improves
	    _tcscpy( inf->name.text, _TEXT("FAT12_") );
	    _tcsncat( inf->name.text, enum_ptr->fd.cFileName, inf->name.length - 6);
	    if( !FindNextFile (enum_ptr->hSearch, &enum_ptr->fd) ) {
		FindClose (enum_ptr->hSearch);
		enum_ptr->hSearch = INVALID_HANDLE_VALUE;
		inf->add_info.info = NULL;
	    }            
	    break;
	}
	if (!FindNextFile (enum_ptr->hSearch, &enum_ptr->fd))
	{
	    FindClose (enum_ptr->hSearch);
	    enum_ptr->hSearch = INVALID_HANDLE_VALUE;
	    inf->add_info.info = NULL;
	    return SUP_ERR_CONNECT;
	}
    }
    inf->size_of = sizeof( TSupSysInfoGroupEnumNext );
    return SUP_ERR_NO;
}
#endif	/* !UNDER_CE */
/*+ end of file: $Id: f12grpn.c 75486 2012-03-04 12:45:49Z dim $ +*/
