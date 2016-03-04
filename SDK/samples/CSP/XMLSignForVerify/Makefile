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

