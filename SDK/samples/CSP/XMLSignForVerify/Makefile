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
all: XMLSignForVerify.exe
!ELSE
all:
!endif

clean:
	del *.obj *.pdb *.exe *.tlh *.tli 2> nul

csources: mkd
	@copy Makefile "$(dest)"
	@copy XMLSignForVerify.cpp "$(dest)"
	@copy doc_to_sign.xml "$(dest)"

mkd:
	-mkdir "$(dest)"

OBJS = \
	XMLSignForVerify.obj

XMLSignForVerify.exe: $(OBJS)
	$(link) $(cspldebug) $(csplflags) -out:$*.exe $** $(csplibs)
	
XMLSignForVerify.obj:  XMLSignForVerify.cpp
	$(midl) /I "..\\include" /h "msxml5.h" "..\\include\\msxml5.idl"
	$(cc) $(cspcdebug) $(cspcflags) $(cspcvars) -DUNICODE -D_UNICODE $*.cpp

