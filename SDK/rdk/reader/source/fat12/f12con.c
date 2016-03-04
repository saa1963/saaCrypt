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
#ifdef UNIX
#include <unistd.h>
#endif /* UNIX */

/*!
 * \ingroup fat12_interface
 * \brief 
 * \return \link fat12_error Код ошибки\endlink
 */
TSupErr fat12_connect( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TFat12Context *ctx = (TFat12Context*)context;
  TSupErr code=SUP_ERR_MEMORY;
#ifdef FREEBSD
  uid_t euid;
#endif
  size_t len;

  UNUSED(info);

  SUPSYS_PRE_INFO( info, TSupSysInfoConnect );
  SUPSYS_PRE_CONTEXT( context, TFat12Context );
  SUPSYS_PRE( ctx->path == NULL );
  
  len=_tcslen(ctx->connect)+2;
#ifdef UNIX
  len+=strlen(CSP_FLOPPY_ROOT)+1;
#ifdef FREEBSD
  /* Not correct, as we don't know what is uid_t */
  len+=16;
#endif /* FREEBSD */
#else /* UNIX */
  len+=1;
#endif /* UNIX */
  ctx->path = (TCHAR*)malloc( (len + 1) * sizeof( TCHAR ) );
  if( ctx->path == NULL ) goto exit;
#ifdef UNIX
  _stprintf( ctx->path, "%s%s/", CSP_FLOPPY_ROOT, ctx->connect );
#ifdef FREEBSD
  support_user_id_ex(NULL,NULL,&euid,NULL);
  _stprintf( ctx->path+_tcslen(ctx->path), "%lu/", (unsigned long)euid );
#endif /* FREEBSD */
#else /* UNIX */
#   ifndef UNDER_CE
  _tcscpy( ctx->path, ctx->connect );
  _tcscat( ctx->path, _TEXT(":/") );
#   else
  _tcscpy( ctx->path, TEXT("\\"));
  _tcscat( ctx->path, ctx->connect);
  _tcscat( ctx->path, TEXT("\\"));
#   endif
#endif /* UNIX */
  code=SUP_ERR_NO;

 exit:
  if(code) free(ctx);

  return code;
}
