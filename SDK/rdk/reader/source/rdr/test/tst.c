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

#include "tst_prj.h"

TSupportDbContext *support_context_io = NULL;
#if defined( _RESOURCE )
/*+ INSTANCE DLL. +*/
HINSTANCE READER_TEST_RESOURCE;
#endif /* defined( _RESOURCE ) */

/*++++
 * Win32 ����� ����� � DLL. 
 +++++*/
BOOL WINAPI DllMain(
    HINSTANCE instance, /*+ (i) DLL INSTANCE. +*/
    DWORD reason, /*+ (i) ��� �����. +*/
    LPVOID reserved ) /*+ (i) ����������� ��� ������������ ��������. +*/
/*+
 * ����������: 
 *
 * TRUE, ������
 +*/
{
    UNUSED( reserved );
    if( reason == DLL_PROCESS_ATTACH )
    {
	support_context_io = support_print_init( NULL, DTEXT( "tst_dll" ),
	    SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );
	#if defined( _RESOURCE )
	READER_TEST_RESOURCE = instance;
	#endif /* defined( _RESOURCE ) */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
    }
    return TRUE;
}
/*+ end of file: $Id: tst.c 75490 2012-03-04 13:04:37Z dim $ +*/

