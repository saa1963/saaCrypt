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

#include "ric_prj.h"
#include "Win32/ric_rc.h"

static const TSupSysInfoIDSBaseItem OSCAR_IDS_ITEMS2[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_OSCAR2_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_OSCAR, SUPSYS_IDS_ICON_QUANT_V1 },
};

/*!
 * \ingroup oscar_internal
 * \brief Описание смещений ресурса для Оскра
 */
static const TSupSysInfoIDSBase OSCAR2_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( OSCAR_IDS_ITEMS2 ) / sizeof( TSupSysInfoIDSBaseItem ),
    OSCAR_IDS_ITEMS2
};

/*!
 * \ingroup oscar_fun_general
 * \brief Функция возвращающая смещение ресурса для Оскар
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TSupSysInfoIDSBase
 * \return код ошибки.
 * \retval #SUP_ERR_NO смещение ресурса возвращено.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
 */
TSupErr oscar2_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &OSCAR2_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
/* end of file: $Id: osc2ids.c 37543 2006-11-27 16:07:59Z chudov $ */
