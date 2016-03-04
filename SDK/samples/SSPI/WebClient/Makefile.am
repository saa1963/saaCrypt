srctestdir=$(prefix)/src/doxygen/SSPI/WebClient
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/WebClient.c @srcdir@/ReadMe.Txt
noinst_PROGRAMS=WebClient
AM_CPPFLAGS=-I@top_srcdir@ -I@top_srcdir@/reader/include\
        -I@top_srcdir@/CSP/src/include\
        -I@top_srcdir@/CSP/SSP\
        -I@top_srcdir@/shared/include\
        -I@top_srcdir@/asn1/asn1c-v550/rtsrc\
        -I@top_srcdir@/shared/rdtsc
if CSP_32
AM_CPPFLAGS+=-DSIZEOF_VOID_P=4
else
AM_CPPFLAGS+=-DSIZEOF_VOID_P=8
endif
WebClient_SOURCES=WebClient.c
WebClient_LDADD=@top_builddir@/CSP/capilite/libcapi20.la \
	@top_builddir@/CSP/capilite/libcapi10.la\
        @top_builddir@/CSP/SSP/libssp.la \
	@top_builddir@/reader/source/support/librdrsup.la\
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@
EXTRA_DIST=Makefile.unix ReadMe.Txt
