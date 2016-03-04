/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

/*!
 * \file $RCSfile$
 * \version $Revision: 68807 $
 * \date $Date: 2011-06-27 11:16:13 +0400 (Mon, 27 Jun 2011) $
 * \author $Author: dim $
 * \brief ������� ����������� �����������.
 */

#include "fat12prj.h"

/*!
 * \ingroup hdimage_fun_reader
 * \brief ������� ����������� �����������.
 * \param context [in] �������� �����������.
 * \param info [in/out] ��������� #TSupSysInfoRegister
 * \sa #TSupSysInfoRegister, #SUPSYS_FUN_REGISTER, 
 *  #TSupSysFunction, #hdimage_info_unregister, 
 *  #rdr_register, #rdr_unregister
 * \return ��� ������
 * \retval #SUP_ERR_NO ����������� ���������������
 * \retval #SUP_ERR_PARAM ��� ����������� ���������� ����������
 * \retval #SUP_ERR_MEMORY ��� �������� ����������� ������
 * \retval #SUP_ERR_VERIFY ��� ����� ��� ������������ ���������
 */
TSupErr hdimage_register(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TSupSysInfoRegister *inf = (TSupSysInfoRegister*)info;
    TFat12Context *ctx;
    TSupErr code;

    SUPSYS_PRE( context == NULL );
    SUPSYS_PRE_INFO( info, TSupSysInfoRegister );

    inf->size_of = sizeof( TSupSysInfoRegister );
    if( support_load_library() )
	return SUP_ERR_VERIFY;
#ifndef UNIX
    if( support_path2dir( NULL, NULL, NULL ) )
	return SUP_ERR_VERIFY;
#endif
    inf->context = NULL;
    ctx = malloc( sizeof( TFat12Context ) );
    if( ctx == NULL )
    {
	support_unload_library();
	return SUP_ERR_MEMORY;
    }
    ctx->path = NULL;
    ctx->path_to_item = NULL;
    code = hdimage_path_default( &( ctx->path ), 0 );
    if( code )
    {
	support_unload_library(); 
	free( ctx->path ); 
	free( ctx );
	return code;
    }
    ctx->folder = NULL;
    ctx->flags = FAT12_FLAG_HDIMAGE;
    ctx->nickname[0] = 0;
    ctx->connect[0] = 0;

#ifdef UNIX
    support_mutex_init(&ctx->lock);
    ctx->n_lock = 0;
    ctx->handle = NULL;
#else /* UNIX */
    ctx->handle = INVALID_HANDLE_VALUE;
#endif /* UNIX */

    inf->context = ctx;

    return SUP_ERR_NO;
}

TSupErr hdimage_local_machine( 
    void *context, 
    TSupSysInfo *info )
{
    TReaderInfoLocalMachine *inf = (TReaderInfoLocalMachine*)info;
    TFat12Context *ctx = (TFat12Context*)context;
    TSupErr code;
    TCHAR *path;

    SUPSYS_PRE_CONTEXT( context, TFat12Context );
    SUPSYS_PRE_INFO( info, TReaderInfoLocalMachine );

    if( inf->flag & 1 )
    {
	code = hdimage_path_default( &path, 1 );
	ctx->machine=1;
    }
    else
    {
	code = hdimage_path_default( &path, 0 );
	ctx->machine=0;
    }
    if( code )
	return code;
    if( ctx->path )
	free( ctx->path );
    ctx->path = path;
    inf->size_of = sizeof( TReaderInfoLocalMachine );
    return SUP_ERR_NO;
}
/* end of file: $Id: hdreg.c 68807 2011-06-27 07:16:13Z dim $ */
