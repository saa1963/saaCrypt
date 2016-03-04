srctestdir=$(prefix)/src/doxygen/CSP/CodingData
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/CodingData.c
noinst_PROGRAMS=CodingData
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

CodingData_SOURCES=CodingData.c
CodingData_LDADD= \
	@top_builddir@/CSP/capilite/libcapi20.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@
EXTRA_DIST=Makefile.unix
