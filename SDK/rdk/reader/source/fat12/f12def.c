/*
 * Copyright(C) 2008-2010 Проект ИОК
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

/****
 * $Id: f12def.c 75486 2012-03-04 12:45:49Z dim $
 *
 * Работа со считывателями и ключевой информацией (READER).
 *
 * Библиотека чтения / записи ключевой информации на дискеты (FAT12).
 *
 * Функции поддержки информации с ключевой дискеты.
 * Номера поддерживаемых info-функций.
 *
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/
#include "reader.kit/rdr_ver.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
static const TCHAR KEYDEV_PATH[] = _TEXT("\\Config\\KeyDevices\\");
static const TCHAR APP_PATH[] = _TEXT("\\Config\\apppath\\");
static const TCHAR SCRIPT_SUFFIX[] = _TEXT("\\Script");
#define SCRIPT_TIMEOUT 5

static TSupErr fat12_default_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info );

static TSupErr fat12_default_extract_type (
    const TCHAR * path_to_item,
    TCHAR * dest,
    size_t maxlen)
{
    const TCHAR * ptr = NULL, *ptr2= NULL;
    const size_t len = sizeof(KEYDEV_PATH)/sizeof(TCHAR)-1;
    if (path_to_item == NULL )
	return SUP_ERR_UNSUPPORTED;
    if (_tcsnicmp(path_to_item,KEYDEV_PATH,len) == 0)
    {
	size_t to_copy;
	ptr = path_to_item+len;
	ptr2 = _tcschr( ptr , _TEXT('\\') );
	to_copy = ptr2 ? (size_t)(ptr2-ptr) : _tcslen(ptr);
	if (to_copy >= maxlen)
	    return SUP_ERR_UNSUPPORTED;
	_tcsncpy(dest, ptr, to_copy );
 	dest[to_copy]=0;
	return SUP_ERR_NO;
    }
    return SUP_ERR_UNSUPPORTED;
}

static TSupErr fat12_default_carrier_type(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TReaderInfoCarrierType * inf = (TReaderInfoCarrierType *)info;
    UNUSED(context);

    return fat12_default_extract_type(inf->path_to_item, inf->type, sizeof(inf->type)/sizeof(TCHAR));
}
/*!
 * \ingroup fat12_info_general
 * \brief Функция регистрации считывателя.
 * \return Код ошибки
 * \retval SUP_ERR_NO считыватель зарегистрирован
 * \retval SUP_ERR_MEMORY при нехватке памяти
 */
static TSupErr fat12_default_register(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    TFat12Context *ctx;
    TSupSysInfoRegister *inf = (TSupSysInfoRegister*)info;
    TSupErr code;

    UNUSED( context );
    /* Провека входных параметров. */

    SUPSYS_PRE_INFO( info, TSupSysInfoRegister );
    ctx = malloc( sizeof( TFat12Context ) );
    if( ctx == NULL )
	return SUP_ERR_MEMORY;
    ctx->path_to_item = NULL;
    if ( inf->path_to_item.text )
    {
	const size_t len = sizeof(KEYDEV_PATH)/sizeof(TCHAR)-1;
	const TCHAR * ptr=NULL;
	if (_tcsnicmp(inf->path_to_item.text,KEYDEV_PATH,len) != 0)
	{
	    free(ctx->path_to_item);
	    free(ctx);
	    return SUP_ERR_CONNECT;
	}

	ctx->path_to_item = (TCHAR *)calloc(inf->path_to_item.length+1,sizeof(TCHAR));
	if ( ctx->path_to_item == NULL )
	{
	    free(ctx);
	    return SUP_ERR_MEMORY;
	}

	ptr = _tcschr( inf->path_to_item.text+len, _TEXT('\\'));
	if ( ptr == NULL )
	    _tcscpy( ctx->path_to_item, inf->path_to_item.text );
	else
	    _tcsncpy( ctx->path_to_item, inf->path_to_item.text, ptr-inf->path_to_item.text );
    }
    else
    {
	free(ctx);
	return SUP_ERR_CONNECT;
    }
    code = fat12_default_extract_type(inf->path_to_item.text, ctx->nickname, sizeof(ctx->nickname)/sizeof(TCHAR));
    if ( code != SUP_ERR_NO )
    {
	free(ctx->path_to_item);
	free(ctx);
	return SUP_ERR_CONNECT;
    }
	
    ctx->connect[0] = 0;
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
      free(ctx->path_to_item);
      free(ctx);
      return SUP_ERR_CONNECT;
  }
  ctx->n_lock = 0;
#endif /* UNIX */

    inf->context = ctx;
    code=support_load_library();
    if(code) 
    {
	free(ctx->path_to_item);
	free(ctx);
	return code;
    }
    return SUP_ERR_NO;
}

static int fat12_default_lexec(const char *prog, const char * arg, char ** env, time_t tmout,char * out)
{
    pid_t pid;
    int status,wres,ret=-1;
    int fl[2];
    if (out)
	out[0]=0;
    if (pipe (fl) == -1)
	return -1;
    fcntl(fl[0], F_SETFL, fcntl(fl[0],F_GETFL) | O_NONBLOCK);
    pid = fork();
    if ( pid == -1 )
    {
	close(fl[1]);
	goto done;
    }
    if ( pid == 0 )
    {
	dup2(fl[1],1);
	close(fl[0]);
	close(fl[1]);
	if(seteuid(getuid()))
	    exit(1);
	if(setegid(getgid()))
	    exit(1);
	if (execle(prog,prog,arg,NULL,env) == -1 )
	    exit(1);
    }
    else
    {
	char * ptr;
	struct timeval tv,tv0;
	size_t nread = 0;
	ssize_t read_cur = 0;
	close(fl[1]);
	tv.tv_sec = tmout;
	tv.tv_usec = 500000; /* to be sure */
	gettimeofday(&tv0,NULL);
	do 	{
	    int rsel;
	    fd_set rfds;
	    struct timeval tv1;
	    FD_ZERO(&rfds);
	    FD_SET(fl[0],&rfds);
	    rsel = select(fl[0]+1,&rfds,NULL,NULL,&tv);
	    gettimeofday(&tv1,NULL);
	    if (rsel < 0 && errno != EINTR)
	    {
		wres = waitpid(pid,&status,WNOHANG);
		break;
	    }
	    if (rsel > 0 && out && FD_ISSET(fl[0],&rfds))
	    {
		read_cur=read(fl[0],out+nread, PATH_MAX-nread);
		if (read_cur > 0)
		    nread+=read_cur;
		else if (read_cur == -1 && errno != EINTR)
		{
			wres = waitpid(pid,&status,WNOHANG);
			break;
		}
	    }
	    if (tv1.tv_sec > tv0.tv_sec+tmout || 
		    (tv1.tv_sec == tv0.tv_sec+tmout && tv1.tv_usec > tv0.tv_usec))
	    {	
		kill(pid,SIGKILL);
		if (out)
		    out[0] = 0;
		wres = 0;
		break;
	    }
	    if (tv1.tv_usec <= tv0.tv_usec)
	    {
		tv.tv_sec = tmout + tv0.tv_sec - tv1.tv_sec;
		tv.tv_usec = tv0.tv_usec - tv1.tv_usec;
	    }
	    else
	    {
		tv.tv_sec = tmout-1 + tv0.tv_sec - tv1.tv_sec;
		tv.tv_usec = 1000000 + tv0.tv_usec - tv1.tv_usec;
	    }
	} while ((wres = waitpid(pid,&status,WNOHANG))==0 );

	if (wres != pid)
	    goto done;
	if (!WIFEXITED(status))
	    goto done;
	ret=WEXITSTATUS(status);
	if (ret!=0)
	    goto done;
	if (out)
	{
	    read_cur=read(fl[0],out+nread, PATH_MAX-nread);
	    if (read_cur > 0)
		nread+=read_cur;
	    out[nread]=0;
	    ptr=strchr(out,'\n');
	    if (ptr)
		*ptr=0;
	    ptr=out+strlen(out)-1;
	    if (*ptr!= '/')
	    {
		*++ptr='/';
		*++ptr=0;
	    }
	}
    }
done:
    close(fl[0]);
    return ret;
}

static void fat12_default_freeenv( char ** env )
{
    char ** ptr;
    if ( env == NULL )
	return;
    for (ptr=env;ptr[0];ptr++)
	free(ptr[0]);
    free(env);
}
#define put_str(name) \
    if (ctx->name) \
    {\
	int len=strlen(ctx->name)+strlen(#name)+2;\
	ptr[0]=(char *)malloc(len);\
	if (ptr[0] == NULL)\
	    goto err;\
	snprintf(ptr++[0],len,"%s=%s",#name,ctx->name);\
    }
#define put_hex(name) \
    {\
	int len=8+strlen(#name)+2;\
	ptr[0]=(char *)malloc(len);\
	if (ptr[0] == NULL)\
	    goto err;\
	snprintf(ptr++[0],len,"%s=%08lx",#name,ctx->name);\
    }
#define put_val_noctx(name) \
    {\
	int len=8+strlen(#name)+2;\
	ptr[0]=(char *)malloc(len);\
	if (ptr[0] == NULL)\
	    goto err;\
	snprintf(ptr++[0],len,"%s=%d",#name,name);\
    }

static char ** fat12_default_makeenv( TFat12Context * ctx )
{
    char ** ret = NULL, ** ptr =NULL;
    uid_t euid,uid;
    gid_t egid,gid;
    ret = (char **)calloc(11,sizeof(char *));
    if (ret == NULL )
	return NULL;
    uid = getuid();
    gid = getgid();
    support_user_id_ex(NULL,NULL,&euid,&egid);
    ptr = ret;
    put_str(path);
    put_hex(volume_serial);
    put_str(volume_label);
    put_str(path_to_item);
    put_str(nickname);
    put_str(connect);
    put_val_noctx(uid);
    put_val_noctx(gid);
    put_val_noctx(euid);
    put_val_noctx(egid);
    ptr[0]=NULL;
    return ret;
err:
    fat12_default_freeenv(ret);
    return NULL;
}
#undef put_str
#undef put_hex
#undef put_val_noctx

static char * fat12_default_get_script(TFat12Context * ctx)
{
    TCHAR * name = NULL;
    TCHAR buf[PATH_MAX+1];
    size_t nlen=_tcslen(ctx->path_to_item)+sizeof(SCRIPT_SUFFIX)/sizeof(TCHAR);
    if (ctx->path_to_item == NULL)
	return NULL;
    name = (TCHAR *)malloc(nlen*sizeof(TCHAR));
    if (name == NULL)
	return NULL;
    _sntprintf(name,nlen,"%s%s",ctx->path_to_item,SCRIPT_SUFFIX);
    nlen = PATH_MAX;
    if (support_registry_get_string(name,&nlen,buf) != SUP_ERR_NO)
    {
	free(name);
	return NULL;
    }
    free (name);
    nlen = sizeof(APP_PATH)/sizeof(TCHAR)+_tcslen(buf);
    name = (TCHAR *)malloc(nlen*sizeof(TCHAR));
    if  (name == NULL)
	return NULL;
    _sntprintf(name,nlen,"%s%s",APP_PATH,buf);
    nlen = PATH_MAX;
    if (support_registry_get_string(name,&nlen ,buf) != SUP_ERR_NO)
    {
	free(name);
	return NULL;
    }
    free(name);
    name=_tcsdup(buf);
    return name;
}

static int mount_default_shit (TFat12Context * ctx)
{
    char * script= NULL;
    char ** env = NULL;
    char buf[PATH_MAX];
    int ret=-1;
    struct stat st;
    script=fat12_default_get_script(ctx);
    if (script == NULL)
	goto done;
    env = fat12_default_makeenv(ctx);
    if (env == NULL)
	goto done;
    if (fat12_default_lexec(script,"lock",env,SCRIPT_TIMEOUT,buf) != 0)
	goto done;
    if (stat(buf,&st)==-1)
	goto done;
    if (!S_ISDIR(st.st_mode))
	goto done;
    if (ctx->path)
	free(ctx->path);
    ctx->path = strdup(buf);
    if (ctx->path)
       ret = 0;
done:
    if (env)
	fat12_default_freeenv(env);
    if (script)
	free(script);
    return ret;
}

static int unmount_default_shit (TFat12Context * ctx)
{
    char * script= NULL;    
    char ** env = NULL;
    int ret=-1;
    script=fat12_default_get_script(ctx);
    if (script == NULL)
	goto done;
    env = fat12_default_makeenv(ctx);
    if (env == NULL)
	goto done;
    if (fat12_default_lexec(script,"unlock",env,SCRIPT_TIMEOUT,NULL) != 0)
	goto done;
    ret = 0;
done:
    if (env)
	fat12_default_freeenv(env);
    if (script)
	free(script);
    return ret;
}

// READER_FUN_CARRIER_TYPE
static TSupErr fat12_default_lock( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TFat12Context *ctx = (TFat12Context*)context;

  SUPSYS_PRE_INFO( info, TReaderInfoLock );
  SUPSYS_PRE_CONTEXT( context, TFat12Context );

  if (!ctx->n_lock)
  { 
    if(support_mutex_lock(ctx->lock))
	return SUP_ERR_CANCEL;
    /* n_lock - для рекурсии.
     * Вроде, безопасно - ctx используется в рамках нити, так что n_lock у каждой нити свой,
     * а вот lock - общий.
     */
#if defined UNIX
    if(mount_default_shit(ctx)) 
    {
        support_mutex_unlock(ctx->lock);
	return SUP_ERR_CONNECT;
    }
#endif /* UNIX */
  }
  ctx->n_lock++;
  return SUP_ERR_NO;
}

/*++++
 * Unlock function
 ++++*/
static TSupErr fat12_default_unlock( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TFat12Context *ctx = (TFat12Context*)context;

  SUPSYS_PRE_INFO( info, TReaderInfoLock );
  SUPSYS_PRE_CONTEXT( context, TFat12Context );
  
  if (!(--ctx->n_lock))
  {
#if defined UNIX
    unmount_default_shit(ctx); /* If it has failed, Bug with it, Amen... */
#endif /* UNIX */
    support_mutex_unlock(ctx->lock);
  }

  return SUP_ERR_NO;
}

static const TSupSysFunctionTableItem fat12_default_funs[] =
{
    { SUPSYS_FUN_NICKNAME, fat12_info_nickname },
    { SUPSYS_FUN_NAME, fat12_info_name },
    { SUPSYS_FUN_INSTANCE, fat12_info_instance },
    { SUPSYS_FUN_CONTEXT_DUP, fat12_context_dup },
    { SUPSYS_FUN_CONTEXT_FREE, fat12_context_free },
    { SUPSYS_FUN_IDS_BLOCK, fat12_default_info_ids },
    { SUPSYS_FUN_FLAGS, fat12_info_system_flag },
#ifndef UNIX
    { SUPSYS_FUN_CALL, fat12_aux_call },
#endif
    { SUPSYS_FUN_VERSIONSUPPORT, info_versionsupport},
    { READER_FUN_CARRIER_TYPE, fat12_default_carrier_type},
    { SUPSYS_FUN_REGISTER, fat12_default_register },
    { SUPSYS_FUN_UNREGISTER, fat12_unregister },
    { SUPSYS_FUN_CONNECT, fat12_connect },
    { SUPSYS_FUN_DISCONNECT, fat12_disconnect },
    { READER_FUN_SAME_MEDIA, fat12_info_same_media },
#ifdef UNIX
    { READER_FUN_LOCK, fat12_default_lock },
    { READER_FUN_UNLOCK, fat12_default_unlock },
#endif /* UNIX */
    { READER_FUN_CONNECT_CARRIER, fat12_connect_carrier },
    { READER_FUN_DISCONNECT_CARRIER, fat12_disconnect_carrier },
    { READER_FUN_RESTORE_CONTEXT, fat12_restore_context },
#ifndef UNIX
    { READER_FUN_UNIQUE_GET, fat12_unique_get },
#ifndef WINCE
    { READER_FUN_UNIQUE_SET, fat12_unique_set },
#endif // WINCE
#endif /* UNIX */

#ifndef _WIN32
    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_locked_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_locked_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_locked_folder_enum_close },
#else
    { READER_FUN_FOLDER_ENUM_OPEN, fat12_info_folder_enum_open },
    { READER_FUN_FOLDER_ENUM_NEXT, fat12_info_folder_enum_next },
    { READER_FUN_FOLDER_ENUM_CLOSE, fat12_info_folder_enum_close },
#endif
    { READER_FUN_FOLDER_OPEN, fat12_folder_open },
    { READER_FUN_FOLDER_CLOSE, fat12_folder_close },

    { READER_FUN_FOLDER_CLEAR, fat12_folder_clear },
    { READER_FUN_OPEN, fat12_open },
    { READER_FUN_CLOSE, fat12_close },
    { READER_FUN_UNLINK, fat12_unlink },

    { READER_FUN_READ, fat12_read },
    { READER_FUN_WRITE, fat12_write },
    { READER_FUN_CHSIZE, fat12_chsize },
    { READER_FUN_LENGTH, fat12_length },
};

static const TSupSysFunctionTable fat12_default_table =
{
    sizeof( TSupSysFunctionTable ),
    sizeof( fat12_default_funs ) / sizeof( TSupSysFunctionTableItem ),
    fat12_default_funs
};

const TSupSysEHandle *default_reader_get_handle( void )
{
    return (const TSupSysEHandle*)&fat12_default_table;
}

static const TSupSysInfoIDSBaseItem DEFAULT_IDS_ITEMS[] = 
{
    { SUPSYS_IDS_BLOCK_BASE, IDS_DEFAULT_NAME, SUPSYS_IDS_BASE_QUANT_V2 },
    { SUPSYS_IDS_BLOCK_ICON, IDI_DEFAULT, SUPSYS_IDS_ICON_QUANT_V1 },
    { READER_IDS_BLOCK, IDS_DEFAULT_INSERT_TEXT, READER_IDS_QUANT_V1 }
};

/*!
 * \ingroup fat12_internal
 * \brief Описание смещений ресурса для DEFAULT
 */
static const TSupSysInfoIDSBase DEFAULT_IDS = 
{
    sizeof( TSupSysInfoIDSBase ),
    sizeof( DEFAULT_IDS_ITEMS ) / sizeof( TSupSysInfoIDSBaseItem ),
    DEFAULT_IDS_ITEMS
};

/*!
 * \ingroup default_fun_general
 * \brief Функция возвращающая смещение ресурса для создаваемых динамических
 * \brief media
 * \param context [in] контекст считывателя. Не используется.
 * \param info [out] структура #TSupSysInfoIDSBase
 * \return код ошибки.
 * \retval #SUP_ERR_NO смещение ресурса возвращено.
 * \retval #SUP_ERR_PARAM при неправильных параметрах
 */
static TSupErr fat12_default_info_ids(
    TSupSysContext *context, 
    TSupSysInfo *info )
{
    UNUSED( context );

    SUPSYS_PRE_INFO( info, TSupSysInfoIDSBase );
    memcpy( info, &DEFAULT_IDS, sizeof( TSupSysInfoIDSBase ) );
    return SUP_ERR_NO;
}
