#
# Copyright(C) 2000-2010 ������ ���
#
# ���� ���� �������� ����������, ����������
# �������������� �������� ������ ���.
#
# ����� ����� ����� ����� �� ����� ���� �����������,
# ����������, ���������� �� ������ �����,
# ������������ ��� �������������� ����� ��������,
# ���������������, �������� �� ���� � ��� ��
# ����� ������������ ������� ��� ����������������
# ���������� ���������� � ��������� ������ ���.
#
# ����������� ���, ������������ � ���� �����, ������������
# ������������� ��� ����� �������� � �� ����� ���� �����������
# ��� ������ ����������.
#
# �������� ������-��� �� ����� �������
# ��������������� �� ���������������� ����� ����.
#

!include "..\..\samples.mak"
!include "..\csp.mak"

all: SigningHash.exe

clean:
	del *.obj *.pdb *.exe 2> nul

csources: mkd
	@copy Makefile "$(dest)"
	@copy Makefile.unix "$(dest)"
	@copy SigningHash.c "$(dest)"

mkd:
	-mkdir "$(dest)"

OBJS = \
	SigningHash.obj

SigningHash.exe: $(OBJS)
	$(link) $(cspldebug) $(csplflags) -out:$*.exe $** $(csplibs)
