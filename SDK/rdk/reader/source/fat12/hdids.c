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

#include "fat12prj.h"

static const TSupSysInfoIDSBaseItem HDIMAGE_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_HDIMAGE_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_HDIMAGE, SUPSYS_IDS_ICON_QUANT_V1 },
    { READER_IDS_BLOCK, IDS_HDIMAGE_INSERT_TEXT, READER_IDS_QUANT_V1 }
};

/*!
 * \ingroup fat12_internal
 * \brief Описание смещений ресурса для HDIMAGE
 */
static const TSupSysInfoIDSBase HDIMAGE_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( HDIMAGE_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    HDIMAGE_IDS_ITEMS
};

/*!
 * \ingroup hdimage_fun_general
 * \brief Функция возвращающая смещение ресурса для HDIMAGE
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TSupSysInfoIDSBase
 * \return код ошибки.
 * \retval #SUP_ERR_NO смещение ресурса возвращено.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
 */
TSupErr hdimage_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &HDIMAGE_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: hdids.c 37543 2006-11-27 16:07:59Z chudov $ */
