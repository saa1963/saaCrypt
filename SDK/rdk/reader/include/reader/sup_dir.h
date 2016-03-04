#ifndef SUP_DIR_H
#define SUP_DIR_H 1

#ifdef UNIX
#include <dirent.h>
#endif /* UNIX */

#define SUPDIR_TYPE_UNKNOWN 0
#define SUPDIR_TYPE_REGULAR 1
#define SUPDIR_TYPE_DIR     2

#ifdef UNIX
#define FOUND_NAME(a) ((a)->d_name)
#define no_more_files(a) (a == NULL)
#else /* UNIX */
#define FOUND_NAME(a) ((a).cFileName)
#define no_more_files(a) ( GetLastError() == ERROR_NO_MORE_FILES )
#endif /* UNIX */

typedef struct TSupFolderEnum_
{
#if defined(UNIX)
    struct dirent *find_data;
    DIR *handle;
#else 
    WIN32_FIND_DATA find_data;
    HANDLE handle;
#endif /* defined( UNIX ) */
    TSupErr code;
    TCHAR *full_prefix;
    TCHAR prefix[10];
    int num;
} TSupFolderEnum;

#ifdef UNIX
TSupErr support_validname(TCHAR *path, struct dirent *fd);
#else /* UNIX */
TSupErr support_validname(TCHAR *path, WIN32_FIND_DATA fd);
#endif /* UNIX */


#endif /* SUP_DIR_H */
