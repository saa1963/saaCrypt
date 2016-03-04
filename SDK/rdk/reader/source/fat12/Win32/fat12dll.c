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

/****
 * $Id: fat12dll.c 75487 2012-03-04 12:47:53Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ����� ����� ��� Win32 DLL.
 *
 * �������: DllMain
 *
 * ���������: FAT12_DLL_INSTANCE
 ****/

#include "fat12prj.h" /*+ Project (READER/FAT12) include.
    ������ include ���� ��� ������� (READER/FAT12). +*/

/*+ INSTANCE DLL. +*/
#if defined( _RESOURCE )
HINSTANCE FAT12_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

///*++++
// * Win32 ����� ����� � DLL. 
// +++++*/
//BOOL WINAPI DllMain(
//    HINSTANCE, /*+ (i) DLL INSTANCE. +*/
//    DWORD, /*+ (i) ��� �����. +*/
//    LPVOID ); /*+ (i) ����������� ��� ������������ ��������. +*/

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
	#if defined( _RESOURCE )
	FAT12_DLL_INSTANCE = instance;
	#endif /* defined( _RESOURCE ) */
#if !defined (UNDER_CE) 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	
	DisableThreadLibraryCalls( instance ); 
    }
    return TRUE;
}
/*+ end of file:$Id: fat12dll.c 75487 2012-03-04 12:47:53Z dim $ +*/
