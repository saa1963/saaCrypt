/*!
 * \ingroup GroupName
 * \brief Короткое описание функции.
 *
 * Подробное описание функции.
 *
 * \param lparam [in/out] описание параметра.
 * \return описание типа возвращаемых значение
 * \retval ERROR_SUCCESS, описание возвращаемого значения
 */

/*!
 * \file $RCSfile$
 * \version $Revision: 54220 $
 * \date $Date: 2009-05-02 06:33:10 +0400 (Sat, 02 May 2009) $
 * \author $Author: lse $
 * \brief Функции установки повтора операции.
 */

#include "tst_prj.h" /* Project (READER/RDR/TEST) include.
    include файл для проекта (READER/RDR/TEST). */

#if defined( UNIX )
#include <unistd.h>
#endif

typedef struct TReaderTestRetryInfo_
{
    TSupSysEContext *context;
} TReaderTestRetryInfo;

typedef struct TReaderTestRetryWaitInfo_
{
    TSupSysEContext *context;
} TReaderTestRetryWaitInfo;

static TSupErr reader_test_retry_function( void *info, TSupErr code );
static TSupErr reader_test_retry_wait_function( void *info, TSupErr code );

TSupErr reader_test_retry_function_set(
    TSupSysEContext *context )
{
    TReaderTestRetryInfo *info;
    TSupErr code;
    info = malloc( sizeof( TReaderTestRetryInfo ) );
    if( info == NULL )
	return SUP_ERR_MEMORY;
    info->context = context;
    code = rdr_retry_proc_set( context, reader_test_retry_function, info );
    if( code == SUP_ERR_UNSUPPORTED )
    {
	free( info );
	return SUP_ERR_NO;
    }
    if( code )
    {
	free( info );
	return code;
    }
    return SUP_ERR_NO;
}

TSupErr reader_test_retry_wait_function_set(
    TSupSysEContext *context )
{
    TReaderTestRetryWaitInfo *info;
    TSupErr code;
    info = malloc( sizeof( TReaderTestRetryWaitInfo ) );
    if( info == NULL )
	return SUP_ERR_MEMORY;
    info->context = context;
    code = rdr_retry_proc_set( context, reader_test_retry_wait_function, 
	info );
    if( code == SUP_ERR_UNSUPPORTED )
    {
	free( info );
	return SUP_ERR_NO;
    }
    if( code )
    {
	free( info );
	return code;
    }
    return SUP_ERR_NO;
}

static TSupErr reader_test_retry_function(
    void *info, 
    TSupErr code )
{
    TReaderTestRetryInfo *inf = (TReaderTestRetryInfo*)info;
    TSupErr ret;
    TCHAR unique[256] = _TEXT( "" );
    if( code == SUP_ERR_NO )
	return SUP_ERR_NO;
    if( code == RDR_ERR_RETRY_DUP )
    {
	TRdrRetryContextDup *dup_info = (TRdrRetryContextDup*)info;
	TReaderTestRetryInfo *dest_info;
	dest_info = malloc( sizeof( TReaderTestRetryInfo ) );
	if( dest_info == NULL )
	    return SUP_ERR_MEMORY;
	dest_info->context = dup_info->dest_context;
	dup_info->dest_info = dest_info;
	return SUP_ERR_NO;
    }
    if( code == RDR_ERR_RETRY_FREE )
    {
	if( info != NULL )
	    free( info );
	return SUP_ERR_NO;
    }
    if( code == RDR_ERR_RETRY_ATTACH )
    {
	TRdrRetryContextAttach *attach_info = (TRdrRetryContextAttach*)info;
	inf = (TReaderTestRetryInfo*)attach_info->info;
	inf->context = attach_info->context;
	return SUP_ERR_NO;
    }
    do
    {
	size_t length = 255;
	TCHAR cur_unique[256] = _TEXT( "" );
#ifdef UNIX
	sleep(1);
#else /* UNIX */
	Sleep( 1000 );
#endif /* UNIX */
	test_error( inf->context, code );

	ret = rdr_same_media( inf->context, &length, cur_unique,
	    code == RDR_ERR_WRITE_PROTECT ? RDR_SAME_WRITE_AVAIL : 0 );
	if( ret == RDR_ERR_ANOTHER_MEDIA && _tcsicmp( cur_unique, unique ) )
	{
            DbTrace(DB_TRACE, (FTEXT(support_context_io, "\n%s\n"),cur_unique));	   
	    memcpy( unique, cur_unique, sizeof( cur_unique ) );
	}
    } while( ret && ret != SUP_ERR_CANCEL );
    return ret;
}

static TSupErr reader_test_retry_wait_function(
    void *info, 
    TSupErr code )
{
    TSupErr ret;
    TReaderTestRetryWaitInfo *inf = (TReaderTestRetryWaitInfo*)info;
    TCHAR unique[256] = _TEXT( "" );

    if( code == SUP_ERR_NO )
	return SUP_ERR_NO;
    if( code == RDR_ERR_RETRY_DUP )
    {
	TRdrRetryContextDup *dup_info = (TRdrRetryContextDup*)info;
	TReaderTestRetryWaitInfo *dest_info;
	dest_info = malloc( sizeof( TReaderTestRetryWaitInfo ) );
	if( dest_info == NULL )
	    return SUP_ERR_MEMORY;
	dest_info->context = dup_info->dest_context;
	dup_info->dest_info = dest_info;
	return SUP_ERR_NO;
    }
    if( code == RDR_ERR_RETRY_FREE )
    {
	if( info != NULL )
	    free( info );
	return SUP_ERR_NO;
    }
    if( code == RDR_ERR_RETRY_ATTACH )
    {
	TRdrRetryContextAttach *attach_info = (TRdrRetryContextAttach*)info;
	TReaderTestRetryInfo *retry_inf = (TReaderTestRetryInfo*)attach_info->info;
	retry_inf->context = attach_info->context;
	return SUP_ERR_NO;
    }
    test_error( inf->context, code );
    do
    {
	size_t length = 255;
	TCHAR cur_unique[256] = _TEXT( "" );
#ifdef UNIX
	sleep(1);
#else /* UNIX */
	Sleep( 1000 );
#endif /* UNIX */	    
	ret = rdr_same_media( inf->context, &length, cur_unique,
	    code == RDR_ERR_WRITE_PROTECT ? RDR_SAME_WRITE_AVAIL : 0 );
	if( ret && ret != code )
	    test_error( inf->context, ret );
	if( ret == RDR_ERR_ANOTHER_MEDIA && _tcsicmp( cur_unique, unique ) )
	{
	    DbTrace(DB_TRACE, (FTEXT(support_context_io, "\n%s\n"),cur_unique));	   
	    memcpy( unique, cur_unique, sizeof( cur_unique ) );
	}
	else
	    *unique = 0;
	code = ret;
    } while( ret && ret != SUP_ERR_CANCEL );
    return ret;
}


/* end of file: $Id: tstretry.c 54220 2009-05-02 02:33:10Z lse $ */
