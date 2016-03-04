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
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief Определение принадлежности карты к типу Магистра 
 *        (ОС Trust).
 */

#include "ric_prj.h"
#include "reader.kit/rdr_ver.h"

#define templTRUST_FUNS(x)		TRUS##x##_FUNCS
#define templTRUST_TABLE(x)		TRUS##x##_TABLE
#define templTrust_media_get_handle(x)	trus##x##_media_get_handle
#define templTrust_property_init(x)	trus##x##_property_init
#define templTrust_wizard_init(x)	trus##x##_wizard_init
#define templTrust_connect_remove(x)	trus##x##_connect_remove
#define templTrust_info_ids(x)		trus##x##_info_ids
#define templTRUST_PATH_CFG(x)		_TEXT("\\config\\KeyCarriers\\TRUS") _TEXT(#x) _TEXT("\\")
#define templTRUST_IDS_ITEMS(x)		TRUS##x##_IDS_ITEMS
#define templTRUST_IDS(x)		TRUS##x##_IDS
#define templTRUST_NAME(x)		_TEXT("TRUS") _TEXT(#x)

/* TODO: удалить debug в окончательной версии (TRUSTD) */
#define ATRUST_FUNS		templTRUST_FUNS(TD)
#define ATRUST_TABLE		templTRUST_TABLE(TD)
#define Atrust_media_get_handle	templTrust_media_get_handle(td)
#define Atrust_property_init	templTrust_property_init(td)
#define Atrust_wizard_init	templTrust_wizard_init(td)
#define Atrust_connect_remove	templTrust_connect_remove(td)
#define Atrust_info_ids		templTrust_info_ids(td)
#define ATRUST_PATH_CFG		templTRUST_PATH_CFG(TD)
#define ATRUST_NAME		templTRUST_NAME(TD)
#define ATRUST_ATR	    _TEXT("3B9800008031C072F7418107")
#define ATRUST_MASK	    _TEXT("FFFF0000FFFFFFFFFFFFFFFF")
#define ATRUST_FILE_NAMES    _TEXT("A\\B\\C\\D\\E\\F\\G\\H")
#define ATRUST_IDS_ITEMS		templTRUST_IDS_ITEMS(TD)
#define ATRUST_IDS		templTRUST_IDS(TD)
#define AIDS_TRUST_NAME		IDS_TRUSTD_NAME
#   include "mag_is_impl.h"
#undef ATRUST_FUNS
#undef ATRUST_TABLE
#undef Atrust_media_get_handle
#undef Atrust_property_init
#undef Atrust_wizard_init
#undef Atrust_connect_remove
#undef Atrust_info_ids
#undef ATRUST_PATH_CFG
#undef AIDS_TRUST_NAME
#undef ATRUST_NAME
#undef ATRUST_ATR
#undef ATRUST_MASK
#undef ATRUST_FILE_NAMES
#undef ATRUST_IDS_ITEMS
#undef ATRUST_IDS


/* Карта Магистра обычно выпускаемая TRUST */
#define ATRUST_FUNS		templTRUST_FUNS(T)
#define ATRUST_TABLE		templTRUST_TABLE(T)
#define Atrust_media_get_handle	templTrust_media_get_handle(t)
#define Atrust_property_init	templTrust_property_init(t)
#define Atrust_wizard_init	templTrust_wizard_init(t)
#define Atrust_connect_remove	templTrust_connect_remove(t)
#define Atrust_info_ids		templTrust_info_ids(t)
#define ATRUST_PATH_CFG		templTRUST_PATH_CFG(T)
#define ATRUST_NAME		templTRUST_NAME(T)
#define ATRUST_ATR		_TEXT("3B9E00008031C0654D4700000072F7418107")   // TODO: заменить на согласованные
#define ATRUST_MASK		_TEXT("FFFF0000FFFFFFFFFFFF300000FFFFFFFFFF")
#define ATRUST_FILE_NAMES	_TEXT("A\\B\\C\\D\\E\\F\\G\\H")		    // TODO: заменить на согласованные
#define ATRUST_IDS_ITEMS	templTRUST_IDS_ITEMS(T)
#define ATRUST_IDS		templTRUST_IDS(T)
#define AIDS_TRUST_NAME		IDS_TRUST_NAME
#   include "mag_is_impl.h"
#undef ATRUST_FUNS
#undef ATRUST_TABLE
#undef Atrust_media_get_handle
#undef Atrust_property_init
#undef Atrust_wizard_init
#undef Atrust_connect_remove
#undef Atrust_info_ids
#undef ATRUST_PATH_CFG
#undef AIDS_TRUST_NAME
#undef ATRUST_NAME
#undef ATRUST_ATR
#undef ATRUST_MASK
#undef ATRUST_FILE_NAMES
#undef ATRUST_IDS_ITEMS
#undef ATRUST_IDS
#undef AIDS_TRUST_NAME

/* Карта Магистра для Сбербанк/BGS - TRUSTS */
#define ATRUST_FUNS		templTRUST_FUNS(TS)
#define ATRUST_TABLE		templTRUST_TABLE(TS)
#define Atrust_media_get_handle	templTrust_media_get_handle(ts)
#define Atrust_property_init	templTrust_property_init(ts)
#define Atrust_wizard_init	templTrust_wizard_init(ts)
#define Atrust_connect_remove	templTrust_connect_remove(ts)
#define Atrust_info_ids		templTrust_info_ids(ts)
#define ATRUST_PATH_CFG		templTRUST_PATH_CFG(TS)
#define ATRUST_NAME		templTRUST_NAME(TS)
#define ATRUST_ATR		_TEXT("3B9A00008031C0610072F7418107")	    // TODO: заменить на согласованные
#define ATRUST_MASK		_TEXT("FFFF0000FFFFFFFF30FFFFFFFFFF")
#define ATRUST_FILE_NAMES	_TEXT("A\\B\\C\\D")				    // TODO: заменить на согласованные
#define ATRUST_IDS_ITEMS	templTRUST_IDS_ITEMS(TS)
#define ATRUST_IDS		templTRUST_IDS(TS)
#define AIDS_TRUST_NAME		IDS_TRUSTS_NAME
#   include "mag_is_impl.h"
#undef ATRUST_FUNS
#undef ATRUST_TABLE
#undef ATRUST_MEDIA_GET_HANDLE
#undef ATRUST_PROPERTY_INIT
#undef ATRUST_WIZARD_INIT
#undef ATRUST_INFO_IDS
#undef ATRUST_PATH_CFG
#undef AIDS_TRUST_NAME
#undef ATRUST_NAME
#undef ATRUST_ATR
#undef ATRUST_MASK
#undef ATRUST_FILE_NAMES
#undef ATRUST_IDS_ITEMS
#undef ATRUST_IDS
#undef AIDS_TRUST_NAME

/* end of file: $Id: mag_is.c 54220 2009-05-02 02:33:10Z lse $ */
