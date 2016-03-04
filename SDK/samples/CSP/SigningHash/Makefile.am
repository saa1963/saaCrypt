srctestdir=$(prefix)/src/doxygen/CSP/SigningHash
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/SigningHash.c
noinst_PROGRAMS=SigningHash
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

SigningHash_SOURCES=SigningHash.c
SigningHash_LDADD= \
	@top_builddir@/CSP/capilite/libcapi10.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
