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

#include "ric_prj.h"

TSupErr ric_error( unsigned char sw1, unsigned char sw2 )
{
    unsigned sw = sw1 << 8 | sw2;
    if( ( sw & 0xFFF0 ) == 0x63C0 )
    {
	if( sw & 0xf )
	    return RDR_ERR_INVALID_PASSWD;
	else
	    return RDR_ERR_PASSWD_LOCKED;
    }
    switch( sw )
    {
    case 0x65F0: return RDR_ERR_INVALID_MEDIA;
    case 0x6982: return RDR_ERR_NEED_PASSWD;
    case 0x6a82: return RDR_ERR_FILE_NOT_FOUND;
    }
    return SUP_ERR_VERIFY;
}
