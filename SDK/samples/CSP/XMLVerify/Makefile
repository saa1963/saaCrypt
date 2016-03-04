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

