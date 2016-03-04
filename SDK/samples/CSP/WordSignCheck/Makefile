#
# Copyright(C) 2000-2010 Проект ИОК
#
# Этот файл содержит информацию, являющуюся
# собственностью компании Крипто Про.
#
# Любая часть этого файла не может быть скопирована,
# исправлена, переведена на другие языки,
# локализована или модифицирована любым способом,
# откомпилирована, передана по сети с или на
# любую компьютерную систему без предварительного
# заключения соглашения с компанией Крипто Про.
#
# Программный код, содержащийся в этом файле, предназначен
# исключительно для целей обучения и не может быть использован
# для защиты информации.
#
# Компания Крипто-Про не несет никакой
# ответственности за функционирование этого кода.
#

!include "..\..\samples.mak"
!include "..\csp.mak"

!IF "$(CPU)" == "i386"
all: WordSignCheck.exe
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
	@copy WordSignCheck.csproj "$(dest)"
	@copy Microsoft.Office.Interop.Word.dll "$(dest)"
	@copy OFFICE.DLL "$(dest)"
	@copy Properties\\AssemblyInfo.cs "$(dest)\\Properties"

mkd:
	-mkdir "$(dest)" \
        -mkdir "$(dest)\\Properties"

WordSignCheck.exe: 
	@msbuild WordSignCheck.csproj


