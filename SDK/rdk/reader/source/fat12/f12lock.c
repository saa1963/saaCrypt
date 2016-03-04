/*
 * Copyright(C) 2002-2011 Проект ИОК
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

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    Мастер include файл для проекта (READER/FAT12). +*/

#if defined UNIX
#define __USE_BSD
#include <unistd.h>
#undef __USE_BSD

static int cp_system (TCHAR *command);

#ifdef SOLARIS
static int mount_shit_solaris(TFat12Context *ctx) 
{
  char cmd[64+sizeof(CSP_FLOPPY_ROOT)];
  TSupErr res;
  int i;

  snprintf(cmd, sizeof(cmd)-1, "volrmmount -i floppy%s", ctx->connect);
  res = support_thread_actualize_uids();
  if (res != SUP_ERR_NO )
    return SUP_ERR_RIGHTS;
  res = cp_system(cmd);
  support_thread_deactualize_uids();
  if (res != SUP_ERR_NO )
    return SUP_ERR_CONNECT;

  snprintf(cmd, sizeof(cmd)-1, "%s%s", CSP_FLOPPY_ROOT, ctx->connect);

  for(i=0;i<5;i++){
    struct stat buf;

    if(!stat(cmd,&buf)) break;
    sleep(1);
  }
  if(i==5){
    return SUP_ERR_CONNECT;
  }

  return SUP_ERR_NO;
}
#endif /* SOLARIS */

/* Mount shit */
static int mount_shit(TFat12Context *ctx)
{
#ifdef SOLARIS
  return mount_shit_solaris(ctx);
#else
  char cmd[64+sizeof(CSP_FLOPPY_ROOT)];
  uid_t euid;
  gid_t egid;
  int res;

#ifdef FREEBSD
  sprintf(cmd, "/sbin/mount_msdosfs ");
#else /* FREEBSD */
  sprintf(cmd, "mount ");
#endif /* FREEBSD */
  support_user_id_ex(NULL,NULL,&euid,&egid);
  if (!getuid()) /* Root mounts */
#ifdef FREEBSD
    snprintf(cmd+strlen(cmd), sizeof(cmd)-strlen(cmd)-1, "-u %d ", euid);
  snprintf(cmd+strlen(cmd), sizeof(cmd)-strlen(cmd)-1, "/dev/fd%s ", ctx->connect);
#else /* FREEBSD */
    snprintf(cmd+strlen(cmd), sizeof(cmd)-strlen(cmd)-1, "-o uid=%d -o umask=077 ", euid);
#endif /* FREEBSD */
  strncat(cmd, ctx->path, sizeof(cmd)-strlen(cmd)-1);
  res = support_revert_to_self();
  if ( res != SUP_ERR_NO )
    return res;
  res = support_thread_actualize_uids();
  if ( res != SUP_ERR_NO )
  {
	support_impersonate_user_by_uids(euid,egid);
	return res;
  }
#ifdef FREEBSD /* Damned one! */
  mkdir(ctx->path, S_IRWXU);
#endif /* FREEBSD */
  res = cp_system(cmd);	/* правильный код ошибки установлен внутри */
  support_thread_deactualize_uids();
  support_impersonate_user_by_uids(euid,egid);

  return res;
#endif
}

static void unmount_shit(TFat12Context *ctx)
{
  char cmd[64+sizeof(CSP_FLOPPY_ROOT)];

#ifdef SOLARIS
  snprintf(cmd, sizeof(cmd)-1, "volrmmount -e floppy%s", ctx->connect);
#else
  snprintf(cmd, sizeof(cmd)-1, "PATH=/sbin:/bin umount %s", ctx->path);
#endif /* UNIX flavours */
  cp_system(cmd);
#ifdef FREEBSD /* Damned one! */
  rmdir(ctx->path);
#endif /* FREEBSD */
}
#endif /* UNIX */

/*++++
 * Lock function
 ++++*/
TSupErr fat12_lock( 
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
    if(mount_shit(ctx)) 
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
TSupErr fat12_unlock( 
    TSupSysContext *context, 
    TSupSysInfo *info )
{
  TFat12Context *ctx = (TFat12Context*)context;

  SUPSYS_PRE_INFO( info, TReaderInfoLock );
  SUPSYS_PRE_CONTEXT( context, TFat12Context );
  
  if (!(--ctx->n_lock))
  {
#if defined UNIX
    unmount_shit(ctx); /* If it has failed, Bug with it, Amen... */
#endif /* UNIX */
    support_mutex_unlock(ctx->lock);
  }

  return SUP_ERR_NO;
}

#if defined UNIX
#ifdef FREEBSD
/* Чиним глючную реализацию fork на FreeBSD... */

/*
 * Copyright (c) 1988, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)system.c    8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/lib/libc/stdlib/system.c,v 1.10 2002/03/22 21:53:10 obrien Exp $");

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <paths.h>
#include <errno.h>

int             _sigaction(int, const struct sigaction *, struct sigaction *);
int       	_sigprocmask(int, const sigset_t * __restrict, sigset_t * __restrict);
pid_t   	_wait4(pid_t, int *, int, struct rusage *);
pid_t		__sys_fork(void);

static int
cp_system (TCHAR *command)
{
        pid_t pid, savedpid;
        int pstat;
        struct sigaction ign, intact, quitact;
        sigset_t newsigblock, oldsigblock;

        if (!command)           /* just checking... */
                return(1);

        /*
         * Ignore SIGINT and SIGQUIT, block SIGCHLD. Remember to save
         * existing signal dispositions.
         */
        ign.sa_handler = SIG_IGN;
        (void)sigemptyset(&ign.sa_mask);
        ign.sa_flags = 0;
        (void)_sigaction(SIGINT, &ign, &intact);
        (void)_sigaction(SIGQUIT, &ign, &quitact);
        (void)sigemptyset(&newsigblock);
        (void)sigaddset(&newsigblock, SIGCHLD);
        (void)_sigprocmask(SIG_BLOCK, &newsigblock, &oldsigblock);
        switch(pid = __sys_fork()) {
//        switch(pid = fork()) {
        case -1:                        /* error */
                break;
        case 0:                         /* child */
                /*
                 * Restore original signal dispositions and exec the command.
                 */
                (void)_sigaction(SIGINT, &intact, NULL);
                (void)_sigaction(SIGQUIT,  &quitact, NULL);
                (void)_sigprocmask(SIG_SETMASK, &oldsigblock, NULL);
                execl(_PATH_BSHELL, "sh", "-c", command, (char *)NULL);
                _exit(127);
        default:                        /* parent */
                savedpid = pid;
                do {
                        pid = _wait4(savedpid, &pstat, 0, (struct rusage *)0);
                } while (pid == -1 && errno == EINTR);
                break;
        }
        (void)_sigaction(SIGINT, &intact, NULL);
        (void)_sigaction(SIGQUIT,  &quitact, NULL);
        (void)_sigprocmask(SIG_SETMASK, &oldsigblock, NULL);
        return(pid == -1 ? -1 : pstat);
}

#else

/*++++
 * Функция получения идентификатора текущего пользователя.
 ++++*/
#include <signal.h>
static int
cp_system(TCHAR *command)
{
    int ret = system(command);

#if defined LINUX
    if(WIFEXITED(ret)) {	/* сами вышли */
	ret = WEXITSTATUS(ret);
	ret = ret ? 0x3000 : SUP_ERR_NO; /* XXX 0x3000 = RDR_ERR_NO_CARRIER; */
    } else if(WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT)) /* сбили сигналом */
	ret = SUP_ERR_CANCEL;
    else	      /* всё остальное. Например, SIGSEGV, нет шелла */
	ret = SUP_ERR_CONNECT;
#else
    ret = ret ? 0x3000 : SUP_ERR_NO; /* XXX 0x3000 = RDR_ERR_NO_CARRIER; */
#endif	/* LINUX */

    return ret;
}

#endif	/* FREEBSD */
#endif	/* UNIX */
