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
all: XMLVerifyTwoSigs.exe
!ELSE
all:
!endif

clean:
	del *.obj *.pdb *.exe *.tlh *.tli 2> nul

csources: mkd
	@copy Makefile "$(dest)"
	@copy XMLVerifyTwoSigs.cpp "$(dest)"
	@copy doc_to_sign.xml "$(dest)"
	@copy doc_to_sign_two.xml "$(dest)"

mkd:
	-mkdir "$(dest)"

OBJS = \
	XMLVerifyTwoSigs.obj

XMLVerifyTwoSigs.exe: $(OBJS)
	$(link) $(cspldebug) $(csplflags) -out:$*.exe $** $(csplibs)

XMLVerifyTwoSigs.obj: XMLVerifyTwoSigs.cpp
	$(midl) /I "..\\include" /h "msxml5.h" "..\\include\\msxml5.idl"
	$(cc) $(cspcdebug) $(cspcflags) $(cspcvars) -DUNICODE -D_UNICODE $*.cpp

