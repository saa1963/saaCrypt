cspcflags= $(cflags)
cspcdebug=$(cdebug)
cspcvars=$(cvars) -D_CRT_SECURE_NO_WARNINGS
cspldebug=$(ldebug)
csplflags=$(conlflags)
csplibs=$(conlibsdll) advapi32.lib crypt32.lib

.c.obj:
	$(cc) $(cspcdebug) $(cspcflags) $(cspcvars) $*.c


.cpp.obj:
	$(cc) $(cspcdebug) $(cspcflags) $(cspcvars) $*.cpp

