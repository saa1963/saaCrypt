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

#include "sststprj.h"

#if defined( _RESOURCE )
/*+ INSTANCE DLL. +*/
HINSTANCE SSTEST_RESOURCE;
#endif /* defined( _RESOURCE ) */

TSupportDbContext *support_context_io = NULL;


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
	support_context_io = support_print_init( NULL, DTEXT( "sstest" ),
	    SUPPORT_DBLEVEL_ALL, SUPPORT_DBFMT_CLEAN );

	#if defined _RESOURCE
	SSTEST_RESOURCE = instance;
	#endif /* _RESOURCE */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
    }
    return TRUE;
}
/*+ end of file: $Id: sststdll.c 61962 2010-05-24 09:22:53Z dim $ +*/
