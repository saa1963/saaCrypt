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
 * \brief Функции дублирующие ресурсы RIC.DLL
 *	  для Магистра (ОС Trust)
 */

#include "ric_prj.h"

//#if !defined(UNIX) //TODO: под Windows должно быть в ресусрсах DLL??
TSupErr trust_info_version(
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoVersion *inf = (TSupSysInfoVersion*)info;

    SUPSYS_PRE_INFO(info, TSupSysInfoVersion);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);

    //
    //TODO: установить версию READER/CSP
    //CSP\src\include\csp_product.h
    //	VER_PRODUCTVERSION_MAJOR
    //	VER_PRODUCTVERSION_MINOR
    //  CVS: $Revision: 54220 $
    //	VER_PRODUCTBUILD
    //
    inf->version[0] = 3;    
    inf->version[1] = 6;
    inf->version[2] = 0;
    inf->version[3] = 5087;
    inf->size_of = sizeof(TSupSysInfoNickname);
    return SUP_ERR_NO;
}

TSupErr trust_info_copyright(
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoCopyright *inf = (TSupSysInfoCopyright*)info;
    static const TCHAR copyright[] = _TEXT("(c) 2008, PKI project");
    static const size_t cr_len = sizeof(copyright)/sizeof(copyright[0]);

    SUPSYS_PRE_INFO(info, TSupSysInfoCopyright);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);

    if (NULL == inf->copyright.text) {
	inf->copyright.length = cr_len;
    } else if (cr_len > inf->copyright.length) {
	inf->copyright.length = cr_len;
        return SUP_ERR_PARAM;	// TODO: MORE_DATA
    } else {
	_tcsncpy(inf->copyright.text, copyright, inf->copyright.length);
    }
    inf->size_of = sizeof(TSupSysInfoNickname);
    return SUP_ERR_NO;
}

TSupErr trust_info_company(
    TSupSysContext *context, TSupSysInfo *info )
{
    TSupSysInfoCompany *inf = (TSupSysInfoCompany*)info;
    static const TCHAR company[] = _TEXT("Crypto-Pro Ltd.");
    static const size_t co_len = sizeof(company)/sizeof(company[0]);

    SUPSYS_PRE_INFO(info, TSupSysInfoCompany);
    SUPSYS_PRE_CCONTEXT(context, TRICContext);

    if (NULL == inf->company.text) {
	inf->company.length = co_len;
    } else if (co_len > inf->company.length) {
	inf->company.length = co_len;
        return SUP_ERR_MEMORY;
    } else {
	_tcsncpy(inf->company.text, company, inf->company.length);
    }
    inf->size_of = sizeof( TSupSysInfoNickname );
    return SUP_ERR_NO;
}

//#endif /* !UNIX */
