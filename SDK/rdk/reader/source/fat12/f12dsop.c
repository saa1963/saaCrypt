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
 * \version $Revision: 56183 $
 * \date $Date: 2009-08-08 20:10:04 +0400 (Sat, 08 Aug 2009) $
 * \author $Author: dim $
 * \brief Функция открытия контекста поиска следующей папки.
 */

#include "fat12prj.h" 

/*! 
 * \ingroup hdimage_fun_reader
 * \brief Функция открытия контекста поиска следующей папки.
 *
 * Функция находит первую папку, но не выдает ее.
 * \param context [in] контекст считывателя. 
 * \param info [in/out] структура #TReaderInfoConnectOpen
 * \sa #TReaderInfoConnectOpen, #READER_FUN_CONNECT_NEXT, 
 *  #TSupSysFunction, #fat12_info_locked_folder_enum_close, #fat12_info_locked_folder_enum_next,
 * \return Код ошибки
 */
TSupErr fat12_info_locked_folder_enum_open(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoFolderEnumOpen *inf = (TReaderInfoFolderEnumOpen*)info;
    TReaderInfoLock info_lock = { sizeof( TReaderInfoLock ) };
    TReaderInfoFolderEnumOpen info_enum;
    TSupErr code;
    TSupErr prev_code;
    TCHAR * list_ptr = NULL;
    size_t list_len = 0;
    size_t list_offs = 0;
    size_t list_num = 0;
    char * info_ptr; /* NOT a TCHAR! */
    size_t enum_len = 0;

    SUPSYS_PRE_INFO( info, TReaderInfoFolderEnumOpen );
    SUPSYS_PRE_CONTEXT( context, TFat12Context );

    if( ( inf->flags & RDR_FOLDER_ENUM_FLAG_NEW ) )
	return fat12_info_folder_enum_open (context, info);

    inf->size_of = sizeof( TReaderInfoFolderEnumOpen );

    code = fat12_lock (context, &info_lock);
    if (code)
	return code; /* TODO: maybe other code? */

    memcpy (&info_enum, inf, sizeof (info_enum));
    code = fat12_info_folder_enum_open (context, &info_enum);
    if (code)
	goto unlock_return;
    info_enum.name.text = malloc (sizeof (TCHAR) * (info_enum.name.length+1));
    enum_len = info_enum.name.length;

    while (1)
    {
	info_enum.name.length = enum_len;
	code = fat12_info_folder_enum_next (context, &info_enum);
	if (code == RDR_ERR_FILE_NOT_FOUND)
	    break;
	if (code)
	    goto close_return;
	if (!list_ptr)
	{
	    list_len = 1024;
	    if (list_len < info_enum.name.length + 1)
		list_len = info_enum.name.length + 1; 
	    /* allocate space for string and termiation zero TCHAR */
	    list_ptr = (TCHAR*) calloc (list_len, sizeof(TCHAR));
	    if (!list_ptr)
	    {
		code = SUP_ERR_MEMORY;
		goto close_return;
	    }
	} else
	if (list_len < list_offs + (info_enum.name.length+1))
	{
	    size_t new_list_len = list_len + 1024;
	    TCHAR * new_list_ptr = NULL;
	    if (new_list_len < list_offs + (info_enum.name.length+1))
		new_list_len = list_offs + (info_enum.name.length+1);
	    /* allocate space for string and TWO termiation zero TCHARs */
	    new_list_ptr = (TCHAR*) calloc (new_list_len, sizeof(TCHAR));
	    memcpy (new_list_ptr, list_ptr, sizeof(TCHAR)*list_len);
	    free (list_ptr);
	    list_ptr = new_list_ptr;
	    list_len = new_list_len;
	}
	memcpy (list_ptr + list_offs, info_enum.name.text, 
	    sizeof (TCHAR)*(info_enum.name.length+1));
	list_offs += info_enum.name.length+1;
	list_num ++;
    };

    code = SUP_ERR_NO;
close_return:
    free (info_enum.name.text);
    prev_code = code;
    code = fat12_info_folder_enum_close (context, &info_enum);    
    if (prev_code)
	code = prev_code;
    if (code)
	goto unlock_return;

    inf->add_info.length = sizeof (size_t) /* current element offset */
	+ sizeof (TCHAR)*list_offs; /* strings */
    inf->add_info.info = malloc (inf->add_info.length);
    inf->name.length = enum_len;

    info_ptr = (char*) inf->add_info.info;
    * (size_t*) info_ptr = sizeof (size_t);
    info_ptr += sizeof (size_t);
    if(list_ptr) /* в зависимости от реализации 2-й аргумент NULL может оказаться недопустимым */
	memcpy (info_ptr, list_ptr, sizeof(TCHAR)*list_offs);

unlock_return:
    free(list_ptr);
    prev_code = code;
    code = fat12_unlock (context, &info_lock);
    if (prev_code)
	code = prev_code;
    return code;
}
/* end of file: $Id: f12dsop.c 56183 2009-08-08 16:10:04Z dim $ */
  
