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

#include "r0_prj.h" /*+ Project (READER/RNDM/RNDM0) include.
    ������ include ���� ��� ������� (READER/RNDM/RNDM0). +*/

#if defined( _RESOURCE )
/*! \ingroup rndm0_int 
 * \brief DLL Instance. 
 *
 * Instance ���������� ��� 0.
 */
HINSTANCE RNDM0_DLL_INSTANCE;
#endif /* defined( _RESOURCE ) */

/*! \ingroup rndm0_int
 * \brief Win32 ����� ����� � DLL. 
 *
 * ������������� / ��������������� ���������� ������, instance, 
 *  common controls.
 * \param instance [in] DLL INSTANCE
 * \param reason [in] ��� �����
 * \param reserved [in] ����������� ��� ������������ ��������
 * \return TRUE ������.
 */
BOOL WINAPI DllMain(
    HINSTANCE instance,
    DWORD reason,
    LPVOID reserved )
{
    UNUSED( reserved );
    if( reason == DLL_PROCESS_ATTACH )
    {
	#if defined( _RESOURCE )
	RNDM0_DLL_INSTANCE = instance;
	#endif /* defined( _RESOURCE ) */
	#if defined _WIN32
	DisableThreadLibraryCalls( instance ); 
	#endif /* _WIN32 */
	return TRUE;
    }
    return TRUE;
}
/* end of file: $Id: rndm0dll.c 75490 2012-03-04 13:04:37Z dim $ */
