#if !defined( _SUPPORT_SUP_SYSW_H )
#define _SUPPORT_SUP_SYSW_H

#include "reader/support.h"
#include "reader/sup_wnd.h"

#ifndef NO_SUP_PROPERTIES
_SUPPORT_DECL TSupErr supsys_properties( HWND hwnd, 
    TSupSysEContext *ctx );
#endif // !NO_SUP_PROPERTIES
#ifndef NO_SUP_WIZARD
_SUPPORT_DECL TSupErr supsys_add2_item( HWND hwnd, 
    TSupSysEList *list, TSupSysEList *installed_list, 
    TSupSysEContext **context );
#endif // NO_SUP_WIZARD

#endif /* defined( _SUPPORT_SUP_SYSW_H ) */
/* end of file: $Id: sup_sysw.h 54220 2009-05-02 02:33:10Z lse $ */
