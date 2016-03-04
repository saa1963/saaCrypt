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

#include "fat12prj.h" /* Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). */

static const TCHAR KEYDEV_PATH[] = _TEXT("\\Config\\KeyDevices\\");

/*!
 * \ingroup fat12_info_general
 * \brief ������� ����������� �����������.
 * \return ��� ������
 * \retval SUP_ERR_NO ����������� ���������������
 * \retval SUP_ERR_MEMORY ��� �������� ������
 */
TSupErr fat12_register(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx;
    TSupSysInfoRegister *inf = (TSupSysInfoRegister*)info;
    TSupErr code;

    UNUSED( context );
    /* ������� ������� ����������. */

    SUPSYS_PRE_INFO( info, TSupSysInfoRegister );
    ctx = malloc( sizeof( TFat12Context ) );
    if( ctx == NULL )
	return SUP_ERR_MEMORY;
    ctx->path_to_item = NULL;
    if ( inf->in_group_name.text )
    {
	TCHAR *ptr = NULL;
	UNUSED(ptr);
	_tcsncpy( ctx->nickname, inf->in_group_name.text, sizeof(ctx->nickname)/sizeof(TCHAR) );
// TODO: improve
#ifdef UNDER_CE
	_tcsncpy( ctx->connect, inf->in_group_name.text + 6, sizeof(ctx->connect)/sizeof(TCHAR) );
#else
	ptr=_tcsrchr(inf->in_group_name.text, _TEXT('_')); 
	if (ptr)
	{
	    ctx->connect[0] = ptr[1];
	    ctx->connect[1] = 0;
	    
	}
	else
	    ctx->connect[1] = 0;
#endif
    }
    ctx->path = NULL;
    ctx->volume_label = NULL;
    ctx->file_name = NULL;
    ctx->folder = NULL;
    ctx->flags = 0;
#ifdef UNIX
  ctx->handle = NULL;
#else /* UNIX */
  ctx->handle = INVALID_HANDLE_VALUE;
  ctx->access = 0;
#endif /* UNIX */

#ifdef UNIX
  support_mutex_init(&ctx->lock);
  if(support_mutex_open(&ctx->lock, "fat12_lock", 0 ))
  {
      free(ctx);
      return SUP_ERR_CONNECT;
  }
  ctx->n_lock = 0;
#endif /* UNIX */

    inf->context = ctx;
    code=support_load_library();
    if(code) 
    {
	free(ctx);
	return code;
    }
    return SUP_ERR_NO;
}

TSupErr fat12_unregister( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx = (TFat12Context*)context;
    TSupSysInfoUnregister *inf = (TSupSysInfoUnregister*)info;

    /* ������� ������� ����������. */
    SUPSYS_PRE_INFO( info, TSupSysInfoUnregister );
    inf->size_of = sizeof( TSupSysInfoUnregister );
    if( ctx == NULL )
	return SUP_ERR_NO;
    if( ctx->path )
      free( ctx->path );
    if (ctx->path_to_item )
      free( ctx->path_to_item );
#ifdef UNIX
    support_mutex_close(&ctx->lock);
#endif /* UNIX */
    free( ctx );

    support_unload_library();
    return SUP_ERR_NO;
}
