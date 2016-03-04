/*
 * Copyright(C) 2008 Проект ИОК
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
 * \version $Revision: 62630 $
 * \date $Date: 2010-06-24 15:33:16 +0400 (Thu, 24 Jun 2010) $
 * \author $Author: dim $
 * \brief Функции перечисления папок
 */

#include "ric_prj.h" 

static TSupErr trust_check_file_presence( 
    TSupSysContext *context, 
    TCHAR folder )
{
    TRICContext *ctx = (TRICContext*)context;
    TSCardInfoAPDU apdu = { 0x00, 0xa4, 0x00, 0x0c };
    unsigned char data[2];
    TSupErr code;
    unsigned short id;

    SUPSYS_PRE_CONTEXT( context, TRICContext );

	id = (unsigned short)(0x101+(unsigned char)((folder-'A')*0x10));

    data[0]=(unsigned char)(id>>8);
    data[1]=(unsigned char)(id&0x00ff);
	
    apdu.input.length = 2;
    apdu.input.info = data;
    apdu.output.length = 0;
    apdu.output.info = NULL;

    code = ctx->apdu( ctx->reader, &apdu );
    if( code )
	return code;

    if( apdu.sw1 == 0x6a && apdu.sw2 == 0x82 ){
	return RDR_ERR_FILE_NOT_FOUND;
	}

    if( apdu.sw1 != 0x90 || apdu.sw2 != 0x00 )
	return RDR_ERR_INVALID_MEDIA;

    return SUP_ERR_NO;
}

typedef struct TTrustFolderEnumOwn_ {
    const TCHAR *folders; /*!< Строка всех folders */
    const TCHAR *cur_folder; /*!< Указатель на еще не перечисленный folder. */
} TTrustFolderEnumOwn;

typedef struct TTrustFolderEnumCtx_ {
    int own; /*!< !0, папки перечисляются PCSC, 0 папки перечисляются
	носителем. */
    union {
	TSupSysInfoUntyped untyped;  /*!< Собсвенно контекст перечисления
	при перечислении носителем. */
	TTrustFolderEnumOwn own; /*!< Собсвенно контекст перечисления
	при перечислении PC/SC. */
    } both;
} TTrustFolderEnumCtx;

/*! 
 * \ingroup trust_fun_general
 * \brief Функция открытия перечисления папок
 * \param context [in] контекст считывателя. 
 * \param info [out] структура #TReaderInfoFolderEnumOpen
 * \sa TReaderInfoFolderEnumOpen, READER_FUN_FOLDER_ENUM_OPEN, 
 * \return код ошибки.
 */
TSupErr trust_folder_enum_open(
    TSupSysContext *context, 
    TSupSysInfo *info)
{
    TTrustFolderEnumCtx *both;
    TReaderInfoFolderEnumOpen *inf = (TReaderInfoFolderEnumOpen*)info;
    TRICContext *ctx = (TRICContext*)context;

    SUPSYS_PRE_INFO(info, TReaderInfoFolderEnumOpen);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);
    inf->add_info.length = 0;
    inf->add_info.info = NULL;

	// Данные для заглушки, которая выдаст "A" и "B" TODO: опрос карты для получения количества контейнеров
    both = malloc(sizeof(*both));
    if(!both)
	return SUP_ERR_MEMORY;
    both->own = 1;
    both->both.own.folders = _TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    both->both.own.cur_folder = both->both.own.folders;
    inf->name.length = 2;   // максимальная длина папки

    inf->add_info.length = sizeof(*both);
    inf->add_info.info = (unsigned char*)both;

    return trust_select_application(ctx);
}

/*! 
 * \ingroup pcsc_fun_general
 * \brief Функция получения следующей папки перечисления
 * \param context [in] контекст считывателя. 
 * \param info [out] структура #TReaderInfoFolderEnumNext
 * \sa TReaderInfoFolderEnumNext, READER_FUN_FOLDER_ENUM_NEXT, 
 * \return код ошибки.
 */
TSupErr trust_folder_enum_next(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupErr code;
    TReaderInfoFolderEnumNext *inf = (TReaderInfoFolderEnumNext*)info;
    TTrustFolderEnumCtx *both;

    SUPSYS_PRE_INFO(info, TReaderInfoFolderEnumNext);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);
    SUPSYS_PRE(inf->add_info.info);
    SUPSYS_PRE_READ_PTRS(inf->add_info.info, inf->add_info.length);

    both = (TTrustFolderEnumCtx*)inf->add_info.info;

	// Заглушка, которая выдаёт "A" и "B" TODO: опрос карты для получения количества контейнеров
    if (!both->own || 0 == *both->both.own.cur_folder) {
	return RDR_ERR_FILE_NOT_FOUND;
    }
    code = SUP_ERR_NO;
    if (NULL != inf->name.text) {
	if (1 <= inf->name.length) {
	    inf->name.text[0] = *both->both.own.cur_folder++;
	    inf->name.text[1] = 0;
	    inf->name.length = 2;
		code = trust_check_file_presence(context,inf->name.text[0]);
	} else {
	    inf->name.text[0] = 0;
	    inf->name.length = 0;
	    code = SUP_ERR_PARAM;	// TODO: MORE_DATA
	}
    } else {
	inf->name.length = 2;	// длинна для опроса данных неопределённой длинны
    }
    return code;
}

/*! 
 * \ingroup pcsc_fun_general
 * \brief Функция закрытия перечисления папок
 * \param context [in] контекст считывателя. 
 * \param info [out] структура #TReaderInfoFolderEnumClose
 * \sa TReaderInfoFolderEnumClose, READER_FUN_FOLDER_ENUM_CLOSE, 
 * \return код ошибки.
 */
TSupErr trust_folder_enum_close(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TTrustFolderEnumCtx *both;
    TReaderInfoFolderEnumClose *inf = (TReaderInfoFolderEnumClose*)info;

    SUPSYS_PRE_INFO(info, TReaderInfoFolderEnumClose);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);
    SUPSYS_PRE(inf->add_info.info);
    SUPSYS_PRE_READ_PTRS(inf->add_info.info, inf->add_info.length);

    both = (TTrustFolderEnumCtx*)inf->add_info.info;
    both->own = 0;
    free(both);
    return SUP_ERR_NO;
}
/* end of file:$Id: mag_fld.c 62630 2010-06-24 11:33:16Z dim $ */
