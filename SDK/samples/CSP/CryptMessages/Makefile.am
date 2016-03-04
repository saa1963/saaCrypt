srctestdir=$(prefix)/src/doxygen/CSP/CryptMessages
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/CryptMessages.c
noinst_PROGRAMS=CryptMessages
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

CryptMessages_SOURCES=CryptMessages.c
CryptMessages_LDADD= \
	@top_builddir@/CSP/capilite/libcapi20.la \
	@DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
