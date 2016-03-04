#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

/* Code types for USE_CODE #define */
#define USE_CODE_C	    1
#define USE_CODE_ASM	    2
#define USE_CODE_ASM64	    1//3

/* Processor types for PROCESSOR_TYPE #define */
#define PROC_TYPE_SPARC 1
#define PROC_TYPE_I386  2
#define PROC_TYPE_X64	3
#define PROC_TYPE_PPC32	4
#define PROC_TYPE_PPC64	5
#define PROC_TYPE_ARM	6
#define PROC_TYPE_ITANIUM   7

#ifdef HAVE_CONFIG_H
#ifdef DARWIN
#   include <stdint.h> 
#endif //DARWIN
#include "myconfig.h"
#elif !defined DEVL && !defined CSP_LITE && !defined _WIN64 && !defined WINCE/* no config.h :Windows ��� ������� �� doxygen*/
#if !defined(UNIX)
# define PATH_MAX MAX_PATH
#endif
# define HAVE_STDLIB_H 1
# define HAVE_LIMITS_H 1
#else
#if defined _WIN64 || defined WINCE
# define PATH_MAX MAX_PATH
# define HAVE_STDLIB_H 1
# define HAVE_LIMITS_H 1 
#endif
#endif /* ������ Windows */

#if !defined(PROCESSOR_TYPE)
    /* 
     * ��� ��������������� ��������:
     * TODO:XXX �������� ������ �� MSDN
     */
#  if defined(_WIN32)
#    if defined(_M_IX86)
#      define PROCESSOR_TYPE PROC_TYPE_I386
#    elif defined(_M_X64)
#      define PROCESSOR_TYPE PROC_TYPE_X64
#    endif //defined(_M_IX86) or defined(_M_X64)
#  endif //defined(_WIN32)
#  if defined(_M_IA64)
#    define PROCESSOR_TYPE PROC_TYPE_ITANIUM
#  endif //defined(_M_IA64)
#  if defined(WINCE)
#    if defined(_M_IX86)
#      define PROCESSOR_TYPE PROC_TYPE_I386
#    else //defined(_M_IX86)
#      define PROCESSOR_TYPE PROC_TYPE_ARM
#    endif //defined(_M_IX86)
#  endif //defined(WINCE)
#endif //defined(PROCESSOR_TYPE)
    /*
     * � ����� ������ ��� Mac OS X, AIX, Solaris � ��. 
     * PROCESSOR_TYPE ������������ � configure
     */
#if !defined(PROCESSOR_TYPE)
    /* 
     * ��� ��������� � �������� ��������������� ��������:
     * Agner Fog, "Calling conventions for different C++ 
     * compilers and operating systems",
     * <http://www.agner.org/optimize/calling_conventions.pdf>
     */
#  if defined(__amd64) || defined(__x86_64__)
#    define PROCESSOR_TYPE PROC_TYPE_X64
#  elif defined(__IA64__)
#    define PROCESSOR_TYPE PROC_TYPE_ITANIUM
#  elif defined(__i386__) || defined(__i386)
#    define PROCESSOR_TYPE PROC_TYPE_I386
#  elif defined(__powerpc__) || defined(__POWERPC__)
       /*
        * ������������� ������������ IBM
	* <http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=%2Fcom.ibm.xlcpp8a.doc%2Fcompiler%2Fref%2Fruopt64b.htm>
	* TODO:XX ������ ����������, ������ �������� � ���������
	*/
#    if __64BIT__ 
#      define PROCESSOR_TYPE PROC_TYPE_PPC64
#    else
#      define PROCESSOR_TYPE PROC_TYPE_PPC32
#    endif
     /*
      * ������������� ������������ Oracle(Sun)
      * <http://docs.oracle.com/cd/E19060-01/stud8.compiler/817-0926/Comp_Options_App.html#15342>
      * TODO:XX ������ ����������, ������ �������� � ���������
      */
#  elif defined(__sparc)
#    define PROCESSOR_TYPE PROC_TYPE_SPARC
#  elif defined(__sparcv9)
       // TODO:XXX ������:
       // D:\4_0\build\CSP\src\RuNetCSP\param.c
       // D:\4_0\build\CSP\src\RuNetCSP\G28147C.c
#    define PROCESSOR_TYPE PROC_TYPE_SPARC
#  elif defined(__ARM_ARCH__) || defined(__arm__)
       // TODO:
       // �������� ������
       // ��������, �����-������ ��� ARM-� ������� ���������
#    define PROCESSOR_TYPE PROC_TYPE_ARM
#  endif
#endif //defined(PROCESSOR_TYPE)
#if !defined(PROCESSOR_TYPE)
#  error "PROCESSOR_TYPE - Can't autodected"
#endif //defined(PROCESSOR_TYPE)

#if !defined(USE_CODE)
#  if PROCESSOR_TYPE == PROC_TYPE_I386
#    if defined(IOS) || defined(WINCE)
#      define USE_CODE USE_CODE_C
#    else
#      define USE_CODE USE_CODE_ASM
#    endif /* IOS */
#  elif PROCESSOR_TYPE == PROC_TYPE_X64
#    define USE_CODE USE_CODE_ASM64
#  else
#    define USE_CODE USE_CODE_C
#  endif /* PROCESSOR_TYPE_* */
#endif /* !USE_CODE */
#if !defined(USE_CODE)
#  error "USE_CODE - Can't autodected"
#endif //defined(PROCESSOR_TYPE)

    //TODO: ������������� 
    //TODO:XXXX ���-�� HAVE_MMX_INSTRUCTIONS ������������ �� �� ����������
#if !defined(HAVE_MMX_INSTRUCTIONS)
#  if (PROCESSOR_TYPE == PROC_TYPE_X64) || \
      (PROCESSOR_TYPE == PROC_TYPE_I386 && !defined(IOS) && !defined(WINCE)) 
#    define HAVE_MMX_INSTRUCTIONS 1 // ��� �������� ����� �������������
      				    // ������ FPU
#  endif /* PROCESSOR_TYPE_* */
#endif /* !defined(HAVE_MMX_INSTRUCTIONS) */


//TODO: � ��� ����������� UNUSED(x) ���� 20-30, � ��������� ������
#ifndef UNUSED
    #define UNUSED(x) (void)(x)
#endif

#if defined DEBUG && !defined _DEBUG
#   define _DEBUG 1
#endif
#if defined _DEBUG && !defined DEBUG
# define DEBUG 1
#endif

/*#define UNICODE
#define _UNICODE
*/
#ifdef UNIX
#define _ANTI_STR _TEXT("%s")
#else
#define _ANTI_STR _TEXT("%S")
#endif

/* Security levels for SECURITY_LEVEL #define
 * ����������� �� �����������. ��� ��� ����� ������, ��������, ���:
 *   #if SECURITY_LEVEL >= KB1
 */

#define KC1 1
#define KC2 2
#define KC3 3
#define KB1 4
#define KB2 5
#define KA1 6

#ifndef SECURITY_LEVEL
#define SECURITY_LEVEL KC1
#endif

#ifdef UNIX
# define _POSIX_PTHREAD_SEMANTICS 1 /* Must be defined for pthreads */
# if !defined _REENTRANT
#   define _REENTRANT 1 /* Must be defined for pthreads */
# endif	/* !_REENTRANT */
# define CSP_NO_GUI 1 /* We don't have GUI writen yet */
# define SUPPORT_RESOURCE_STD 1 /* Should be default on UNIX */
# define MAX_PATH PATH_MAX
# define Sleep(a) usleep(a*1000)
/* Some kind of cheating?  For pthread...settype __USE_UNIX98 must be defined */
# if defined LINUX
#  if !defined _XOPEN_SOURCE
#   define _XOPEN_SOURCE 700
#  endif /* !_XOPEN_SOURCE */
# endif /* LINUX */
#else /* UNIX */
# define X_DISPLAY_MISSING 1 /* We don't have X, do we? */
  ///* ����������� ��� _Win32 ������������ MMX*/
  //    //TODO: �������������
  //# if USE_CODE==USE_CODE_ASM
  //#   define HAVE_MMX_INSTRUCTIONS 1
  //# endif
#endif /* UNIX */

#if defined DEVL && !defined CSP_LITE
# define CSP_LITE 1
#endif
#ifdef CSP_LITE
# define EXCLUDE_READER 1
#endif

#ifdef UNIX
typedef struct __TSupCSPPaths__
{
    const char * csp_static_root;   /* CSP_STATIC_ROOT */
    const char * csp_etc_dir;       /* CSP_ETC_DIR */
    const char * csp_volatile_dir;  /* CSP_VOLATILE_ROOT */
    const char * csp_floppy_root;   /* CSP_FLOPPY_ROOT */
    const char * csp_users_dir;     /* CSP_USERS_DIR */
    const char * csp_dsrf_dir;	    /* CSP_DSRF_DIR */
    const char * csp_protected_dir; /* CSP_PROTECTED_DIR */
    const char * csp_bin_dir;	    /* CSP_BIN_DIR */
    const char * csp_cp1251_ldir;   /* CSP_CP1251_LDIR */
    const char * csp_cp866_ldir;    /* CSP_CP866_LDIR */
    const char * csp_def_ldir;	    /* CSP_DEF_LDIR */
    const char * csp_iso8859_ldir;  /* CSP_ISO88595_LDIR */
    const char * csp_koi8r_ldir;    /* CSP_KOI8R_LDIR */
    const char * csp_utf8_ldir;	    /* CSP_UTF8_LDIR */
    const char * csp_lib_dir;	    /* CSP_LIB_DIR */
    const char * csp_sbin_dir;	    /* CSP_SBIN_DIR */
} TSupCSPPaths;
#ifdef __cplusplus
extern "C" {
#endif
extern const TSupCSPPaths * csp_get_paths(void);
#ifdef __cplusplus
}
#endif
#define CSP_STATIC_ROOT	(csp_get_paths()->csp_static_root)
#define CSP_ETC_DIR	(csp_get_paths()->csp_etc_dir)
#define CSP_VOLATILE_ROOT	(csp_get_paths()->csp_volatile_dir)
#define CSP_FLOPPY_ROOT	(csp_get_paths()->csp_floppy_root)
#define CSP_USERS_DIR	(csp_get_paths()->csp_users_dir)
#define CSP_DSRF_DIR	(csp_get_paths()->csp_dsrf_dir)
#define CSP_PROTECTED_DIR	(csp_get_paths()->csp_protected_dir)
#define CSP_BIN_DIR	(csp_get_paths()->csp_bin_dir)
#define CSP_CP1251_LDIR	(csp_get_paths()->csp_cp1251_ldir)
#define CSP_CP866_LDIR	(csp_get_paths()->csp_cp866_ldir)
#define CSP_DEF_LDIR	(csp_get_paths()->csp_def_ldir)
#define CSP_ETC_DIR	(csp_get_paths()->csp_etc_dir)
#define CSP_ISO88595_LDIR	(csp_get_paths()->csp_iso8859_ldir)
#define CSP_KOI8R_LDIR	(csp_get_paths()->csp_koi8r_ldir)
#define CSP_UTF8_LDIR	(csp_get_paths()->csp_utf8_ldir)
#define CSP_LIB_DIR	(csp_get_paths()->csp_lib_dir)
#define CSP_SBIN_DIR	(csp_get_paths()->csp_sbin_dir)
#endif 
/*

common.h

#define PROC_TYPE_SPARC 1
#define PROC_TYPE_I386  2
#define PROC_TYPE_X64	3
#define PROC_TYPE_PPC32	4
#define PROC_TYPE_PPC64	5
#define PROC_TYPE_ARM	6

���������:
#define PROC_TYPE_ITANIUM	7


#if (PROC_TYPE==PROC_TYPE_SPARC) || (PROC_TYPE==PROC_TYPE_ARM) ||  (PROC_TYPE==PROC_TYPE_PPC32) || (PROC_TYPE==PROC_TYPE_PPC64)|| (PROC_TYPE==PROC_TYPE_ITANIUM)
#define USE_CODE  USE_CODE_C
#endif

#if (PROC_TYPE==PROC_TYPE_I386) //+mmx
#define USE_CODE  USE_CODE_ASM
#endif

#if (PROC_TYPE==PROC_TYPE_X64) 
//��������� ������������ �������� 
#if defined( _M_AMD64 ) || defined(PROCESSOR_TYPE) && (PROCESSOR_TYPE == PROC_TYPE_X64 )
#endif
#endif

*/
#endif /* _COMMON_H_INCLUDED_ */
