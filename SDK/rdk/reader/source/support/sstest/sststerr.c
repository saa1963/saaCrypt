#include "sststprj.h"

/*++++
 * Функция вывода сообщения об ошибке.
 ++++*/
TSupErr supsys_test_error( 
    const TSupSysEContext *context, /*+ Handle элемента, может быть NULL. +*/
    TSupErr code ) /*+ (i) Код ошибки. +*/
{
    static const TCHAR con_error_memory[] = _TEXT( "**Fatal error** Not enought memory." );
    static const TCHAR con_error_verify[] = _TEXT( "**Fatal error** Verification failed." );
    TCHAR *text;
    size_t size = 0;
    TSupErr error;

    if( code == SUP_ERR_CANCEL )
	return SUP_ERR_CANCEL;
    error = supsys_error( context, code, &size, NULL, NULL, 0, 0 );
    if( error == SUP_ERR_MEMORY )
    {
	DbTrace(DB_ERROR, (FTEXT(support_context_io, "%s"),con_error_memory));
	return SUP_ERR_MEMORY;
    }
    if( error == SUP_ERR_VERIFY )
    {
        DbTrace(DB_ERROR, (FTEXT(support_context_io, "%s"),con_error_verify)); 
	return SUP_ERR_VERIFY;
    }
    text = malloc( ( size + 1 ) * sizeof( TCHAR ) );
    if( text == NULL )
    {
        DbTrace(DB_ERROR, (FTEXT(support_context_io, "%s"),con_error_memory)); 
	return SUP_ERR_MEMORY;
    }
    error = supsys_error( context, code, &size, text, NULL, 0, 0 );
    if( error == SUP_ERR_MEMORY )
    {
	free( text );
        DbTrace(DB_ERROR, (FTEXT(support_context_io, "%s"),con_error_memory)); 
	return SUP_ERR_MEMORY;
    }
    if( error == SUP_ERR_VERIFY )
    {
	free( text );
        DbTrace(DB_ERROR, (FTEXT(support_context_io, "%s"),con_error_verify)); 	
	return SUP_ERR_VERIFY;
    }

    DbTrace(DB_ERROR, (FTEXT(support_context_io, "Error: %s"),text)); 	 
    free( text );
    return code;
}
/* end of file:$Id: sststerr.c 36818 2006-10-25 14:46:27Z cav $ */
