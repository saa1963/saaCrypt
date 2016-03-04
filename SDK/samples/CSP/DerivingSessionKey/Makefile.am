srctestdir=$(prefix)/src/doxygen/CSP/DerivingSessionKey
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/DerivingSessionKey.c
noinst_PROGRAMS=DerivingSessionKey
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

DerivingSessionKey_SOURCES=DerivingSessionKey.c
DerivingSessionKey_LDADD= \
	@top_builddir@/CSP/capilite/libcapi10.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
