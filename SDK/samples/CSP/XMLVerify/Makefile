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

!IF "$(CPU)" == "i386"
all: XMLVerify.exe
!ELSE
all:
!endif

clean:
	del *.obj *.pdb *.exe *.tlh *.tli 2> nul

csources: mkd
	@copy Makefile "$(dest)"
	@copy XMLVerify.cpp "$(dest)"

mkd:
	-mkdir "$(dest)"

OBJS = \
	XMLVerify.obj

XMLVerify.exe: $(OBJS)
	$(link) $(cspldebug) $(csplflags) -out:$*.exe $** $(csplibs)

XMLVerify.obj: XMLVerify.cpp
	$(midl) /I "..\\include" /h "msxml5.h" "..\\include\\msxml5.idl"
	$(cc) $(cspcdebug) $(cspcflags) $(cspcvars) -DUNICODE -D_UNICODE $*.cpp

