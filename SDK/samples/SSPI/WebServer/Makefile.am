srctestdir=$(prefix)/src/doxygen/SSPI/WebServer
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/WebServer.c @srcdir@/Default.Htm @srcdir@/ReadMe.Txt
noinst_PROGRAMS=WebServer
AM_CPPFLAGS=-I@top_srcdir@ -I@top_srcdir@/reader/include\
        -I@top_srcdir@/CSP/src/include\
        -I@top_srcdir@/CSP/SSP\
        -I@top_srcdir@/asn1/asn1c-v550/rtsrc\
        -I@top_srcdir@/shared/include\
        -I@top_srcdir@/shared/rdtsc
if CSP_32
AM_CPPFLAGS+=-DSIZEOF_VOID_P=4
else
AM_CPPFLAGS+=-DSIZEOF_VOID_P=8
endif
WebServer_SOURCES=WebServer.c
WebServer_LDADD=@top_builddir@/CSP/capilite/libcapi20.la \
		@top_builddir@/CSP/capilite/libcapi10.la\
        @top_builddir@/CSP/SSP/libssp.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@
EXTRA_DIST=Makefile.unix Default.Htm ReadMe.Txt
