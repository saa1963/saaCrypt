#if !defined( READER_OS_H )
#define READER_OS_H

#ifdef CSP_LITE
# include "csplitecrt.h"
#else
# ifdef HAVE_STDLIB_H
#   include <stdlib.h>
# endif /* HAVE_STDLIB_H */
# include <stdio.h>
# include <locale.h>
#endif /* CSP_LITE */

#if defined( _MSC_VER )
#pragma once
#endif /* _MSC_VER */

#if defined _WIN32 && !defined CSP_LITE
#pragma warning( push )
#pragma warning( disable: 4100 4115 4201 4214 )

#define STRICT 1
#include <windows.h> /* Windows include. */

#pragma warning( pop )

#endif /* _WIN32 && !CSP_LITE */

#include "reader/tchar.h"
#if defined( UNIX ) || ( defined( _WIN32 ) && defined( __STDC__ ) )
#include <errno.h>
#endif

/* Лимит пути, не используется для выделения памяти. Требуется только для
   проверки максимального ограничения. Должен быть int. */
#if defined( _WIN32 )
#define PATH_LIMIT 32767
#else
#define PATH_LIMIT INT_MAX
#endif

#ifdef _WIN32
// Для _ASSERTE
#ifdef WINCE
#include <altcecrt.h>
#else // WINCE
#include <crtdbg.h>
#endif // WINCE
#endif // _WIN32

#endif /* defined( READER_OS_H ) */
