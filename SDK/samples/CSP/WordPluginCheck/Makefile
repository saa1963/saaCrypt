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
all: WordPluginCheck.exe
!ELSE
all:
!endif

clean:
	del *.obj *.pdb *.exe *.tlh *.tli 2> nul
	-rd /s /q bin
	-rd /s /q obj

csources: mkd
	@copy Makefile "$(dest)"
	@copy Program.cs "$(dest)"
	@copy WordPluginCheck.csproj "$(dest)"
	@copy Microsoft.Office.Interop.Word.dll "$(dest)"
	@copy OFFICE.DLL "$(dest)"
	@copy Properties\\AssemblyInfo.cs "$(dest)\\Properties"

mkd:
	-mkdir "$(dest)" \
        -mkdir "$(dest)\\Properties"

WordPluginCheck.exe: 
	@msbuild WordPluginCheck.csproj


