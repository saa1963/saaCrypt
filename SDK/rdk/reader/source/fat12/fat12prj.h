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
 * $Id: fat12prj.h 75486 2012-03-04 12:45:49Z dim $
 *
 * ������ �� ������������� � �������� ����������� (READER).
 *
 * ���������� ������ / ������ �������� ���������� �� ������� (FAT12).
 *
 * ������ include ���� ��� ������� (READER/FAT12).
 *
 * ������ ����������� �� define: 
 *
 * �����������: _MSC_VER, _WIN32
 *
 * ������������ �������������: _USRDLL
 *
 ****/

#if !defined( _READER_FAT12_FAT12PRJ_H )
#define _READER_FAT12_FAT12PRJ_H /*+ ���������� �������� ���������. +*/

#if defined( _MSC_VER )
#pragma once
#define WIN32_LEAN_AND_MEAN 
#endif /* defined( _MSC_VER ) */

#include "reader.kit/gen_prj.h"
#include "reader/dprint.h"

/*+ Standart. +*/
#if !defined _WIN32
#include <sys/stat.h> /*+ Std include. +*/
#include <errno.h> /*+ DOS include. +*/
#endif /* _WIN32 */

#ifdef SOLARIS
#include <volmgt.h>
#endif

#if !defined ( UNIX ) && !defined ( UNDER_CE ) 
#include <shlobj.h> 
#endif /* UNIX */

/*+ Project (READER/SUPPORT) +*/
#include "reader/sup_syse.h"
#include "reader.kit/sup_sys.h"

/*+ Project (READER/RDR) +*/
#include "reader/rdr.h" /*+ Project (READER) include.
    ������� ��������� ������ �� �������������. +*/
#include "reader.kit/reader.h" /*+ Project (READER/RDR) include.
    ����������� ����������� ��� �������� ���������� �����������. +*/

/*+ Project (READER/FAT12) +*/
#if defined( _USRDLL )
#define _FAT12_DECL _STDEXP_DECL
#define _HDIMAGE_DECL _STDEXP_DECL
#endif /* defined( _USRDLL ) */

#include "reader/fat12.h"
#include "reader/hdimage.h"

/*+ Project (READER/FAT12) ���������� include �����. +*/
#include "fat12int.h" /*+ Project (READER/FAT12) include.
    ��������� ������������ ������� ��� ���������� ������� FAT12. +*/
#include "fat12inf.h"
#include "hdimginf.h"

/*+ Project(READER/FAT12) include.
    ��������� ���������. +*/
#ifdef UNIX
#include "fat12_rc.h" 
#else /* UNIX */
#include "Win32/fat12_rc.h" 
#endif /* UNIX */

#if defined( _USRDLL ) 
_HDIMAGE_DECL const TSupSysEHandle* reader_get_handle( size_t i );
#endif /* defined( _USRDLL ) */

#endif /* !defined( _READER_FAT12_FAT12PRJ_H ) */
/*+ end of file: $Id: fat12prj.h 75486 2012-03-04 12:45:49Z dim $ +*/
