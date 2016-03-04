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

/*!
* \file $RCSfile$
* \version $Revision: 75486 $
* \date $Date: 2012-03-04 16:45:49 +0400 (Sun, 04 Mar 2012) $
* \author $Author: dim $
* \brief Функция открытия контекста перечисления считывателей в группе fat12.
*/

#include "fat12prj.h" /* Project (READER/FAT12) include.
Мастер include файл для проекта (READER/FAT12). */

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static TSupErr enum_strings(TFat12ConnectEnum **enum_ptr)
{
    TSupErr code=SUP_ERR_NO;

    int i;
#ifdef SOLARIS
    if(!volmgt_running())
	return SUP_ERR_CONNECT;
#else
    struct stat st;
#endif /* SOLARIS */

    *enum_ptr = malloc( sizeof( TFat12ConnectEnum ) );
    if( *enum_ptr == NULL )
	return SUP_ERR_MEMORY;
    (*enum_ptr)->drive_strings=(*enum_ptr)->cur=NULL;

    for(i=0;i<8;i++){
	char buf[20];
	TFat12ConnectEnumElem *el;

#ifdef SOLARIS
	sprintf(buf,"/dev/rdiskette%d",i);
	if(volmgt_inuse(buf))
#else /* SOLARIS */
	sprintf(buf,"/dev/fd%d",i);
	if(stat(buf, &st)!=ENOENT)
#endif /* SOLARIS */
	{
	    el=(TFat12ConnectEnumElem*)malloc(sizeof(TFat12ConnectEnumElem));
	    if(!el){
		code=SUP_ERR_MEMORY;
		goto exit;
	    }
	    el->next=NULL;
	    el->connect_str=(TCHAR*)malloc(2*sizeof(TCHAR)); /* floppy#+'\0' */
	    if(!el->connect_str){
		code=SUP_ERR_MEMORY;
		free(el);
		goto exit;
	    }
	    _stprintf(el->connect_str,"%d",i);
	    if((*enum_ptr)->cur){
		(*enum_ptr)->cur->next=el;
		(*enum_ptr)->cur=el;
	    } else
		(*enum_ptr)->drive_strings=(*enum_ptr)->cur=el;
	}
    }
exit:
    if(code){
	free(*enum_ptr); /* Correct freeing of what we've allocated previously
			 is needed */
	*enum_ptr = NULL;
    }
    return code;
}
#endif /* UNIX */

/*! 
* \ingroup fat12_fun_reader
* \brief Функция открытия контекста поиска следующего типа подключения.
*
* Функция вычисляет максимальную длину имени соединения.
* \param context [in] контекст считывателя. 
* \param info [in/out] структура #TReaderInfoConnectOpen
* \sa #TReaderInfoConnectOpen, #READER_FUN_CONNECT_NEXT, 
*  #TSupSysFunction, #fat12_info_connect_close, #fat12_info_connect_next,
* \return Код ошибки
* \retval #SUP_ERR_NO структура перечисления считывателей успешно
*  открыта
* \retval #SUP_ERR_PARAM при неправильных параметрах
* \retval #SUP_ERR_MEMORY при нехватке памяти
* \retval "Ошибки ОС" \ref fat12_os_error. 
*/
#ifndef UNDER_CE
TSupErr fat12_group_enum_open(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoGroupEnumOpen *inf = (TSupSysInfoGroupEnumOpen*)info;
    TFat12ConnectEnum *enum_ptr;
#ifdef UNIX
    TSupErr code;
#else /* UNIX */
    DWORD length;
#endif /* UNIX */

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoGroupEnumOpen );
    inf->size_of = sizeof( TSupSysInfoGroupEnumOpen );
    inf->add_info.length = 0;
    inf->add_info.info = NULL;
#ifdef UNIX
    if((code=enum_strings(&enum_ptr)))
	return code;
    inf->name.length=8;
#else /* UNIX */
    enum_ptr = malloc( sizeof( TFat12ConnectEnum ) );
    if( enum_ptr == NULL )
	return SUP_ERR_MEMORY;
    length = GetLogicalDriveStrings( 0, NULL );
    if( length < 40 ) /* Windows 95 некорректно возвращает значение. */
	length = 40;
    enum_ptr->drive_strings = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( enum_ptr->drive_strings == NULL )
    {
	free( enum_ptr );
	return SUP_ERR_MEMORY;
    }
    enum_ptr->cur = enum_ptr->drive_strings;
    GetLogicalDriveStrings( length, enum_ptr->drive_strings );
    /* Получить максимальную длину. */
    inf->name.length = 0;
    while( *( enum_ptr->cur ) )
    {
	size_t l = _tcslen( enum_ptr->cur );
	if( GetDriveType( enum_ptr->cur ) == DRIVE_REMOVABLE )
	{
	    if( inf->name.length < l )
		inf->name.length = l;
	}
	enum_ptr->cur += l + 1;
    }
    if( !inf->name.length )
    {
	free( enum_ptr->drive_strings );
	free( enum_ptr );
	return SUP_ERR_CONNECT;
    }
#endif /* UNIX */
    // TODO: improve
    inf->name.length += _tcslen(_TEXT("FAT12_"));
    enum_ptr->cur = enum_ptr->drive_strings;
    inf->add_info.length = sizeof( TFat12ConnectEnum );
    inf->add_info.info = (unsigned char*)enum_ptr;

    return SUP_ERR_NO;
}
#else
TSupErr fat12_group_enum_open(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoGroupEnumOpen *inf = (TSupSysInfoGroupEnumOpen*)info;
    TFat12ConnectEnum *enum_ptr;
    TSupErr code = SUP_ERR_NO;
    // Name that matches all storage cards
    TCHAR *szFname = TEXT("\\*");

    UNUSED( context );
    SUPSYS_PRE_INFO( info, TSupSysInfoGroupEnumOpen );
    inf->size_of = sizeof( TSupSysInfoGroupEnumOpen );
    inf->add_info.length = 0;
    inf->add_info.info = NULL;

    enum_ptr=(TFat12ConnectEnum*)malloc(sizeof(TFat12ConnectEnum));
    if(!enum_ptr){
	code=SUP_ERR_MEMORY;
	goto exit;
    }

    // Ensure that Storage Card exists.
    enum_ptr->hSearch = FindFirstFile(szFname, &enum_ptr->fd);
    if (enum_ptr->hSearch == INVALID_HANDLE_VALUE)
	return SUP_ERR_NO;
    inf->add_info.length = sizeof(TFat12ConnectEnum);
    inf->add_info.info = (unsigned char *)enum_ptr;
    inf->name.length = SUPSYSE_NICKNAME_LENGTH + 1;
exit:
    if(code){
	free(enum_ptr); /* Correct freeing of what we've allocated previously
			is needed */
	return code;
    }
    return code;
}
#endif
