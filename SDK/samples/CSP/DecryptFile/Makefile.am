srctestdir=$(prefix)/src/doxygen/CSP/DecryptFile
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/DecryptFile.c
noinst_PROGRAMS=DecryptFile

AM_CPPFLAGS= \
	-I@top_srcdir@ \
	-I@top_srcdir@/reader/include \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

DecryptFile_SOURCES=DecryptFile.c
DecryptFile_LDADD= \
	@top_builddir@/CSP/capilite/libcapi20.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
