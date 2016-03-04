srctestdir=$(prefix)/src/doxygen/CSP/SessionKeyParameters
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/SessionKeyParameters.c
noinst_PROGRAMS=SessionKeyParameters
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

SessionKeyParameters_SOURCES=SessionKeyParameters.c
SessionKeyParameters_LDADD= \
	@top_builddir@/CSP/capilite/libcapi10.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@


EXTRA_DIST=Makefile.unix
