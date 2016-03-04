#if !defined( _READER_GEN_PRJ_H )
#define _READER_GEN_PRJ_H

#include "common.h"

#if defined( _WIN32 ) && !defined( __STDC__ ) && !defined( WIN32 )
#define WIN32
#endif /* defined( _WIN32 ) && !defined( __STDC__ ) && !defined( WIN32 ) */

#if defined( UNICODE ) && !defined( _UNICODE )
#define _UNICODE
#endif /* defined( UNICODE ) && !defined( _UNICODE ) */

#if defined( _UNICODE ) && !defined( UNICODE )
#define UNICODE
#endif /* defined( _UNICODE ) && !defined( UNICODE ) */

#if defined( _MSC_VER )
#define _STDEXP_DECL __declspec(dllexport)
#else
#define _STDEXP_DECL
#endif /* _MSC_VER */

#ifdef UNIX
# define _USRDLL 1 
#endif /* UNIX */

#if defined( _LIB ) && !defined( SUPPORT_RESOURCE_STD )
#define SUPPORT_RESOURCE_STD 1
#endif /* defined( _LIB ) && !defined( SUPPORT_RESOURCE_STD ) */

#if defined( _USRDLL ) && defined _WIN32
#define _RESOURCE
#endif /* _USRDLL && _WIN32 */

#include "reader.kit/os_inc.h"
#include "reader/support.h"

#endif /* defined( _READER_GEN_PRJ_H ) */
