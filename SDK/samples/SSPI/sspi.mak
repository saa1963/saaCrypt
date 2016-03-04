sspicflags= $(cflags)
sspicdebug=$(cdebug)
sspicvars=$(cvars) -D_CRT_SECURE_NO_WARNINGS
sspildebug=$(ldebug)
sspilflags=$(conlflags)
sspilibs=$(conlibsdll) advapi32.lib crypt32.lib

.c.obj:
	$(cc) $(sspicdebug) $(sspicflags) $(sspicvars) $*.c

.cpp.obj:
	$(cc) $(sspicdebug) $(sspicflags) $(sspicvars) $*.cpp

