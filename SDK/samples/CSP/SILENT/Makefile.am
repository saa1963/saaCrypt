srctestdir=$(prefix)/src/doxygen/CSP/SILENT
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/SILENT.c
noinst_PROGRAMS=SILENT
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

SILENT_SOURCES=SILENT.c
SILENT_LDADD= \
	@top_builddir@/CSP/capilite/libcapi10.la \
	@DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
