srctestdir=$(prefix)/src/doxygen/CSP/CryptMsgSign
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/CryptMsgSign.cpp
noinst_PROGRAMS=CryptMsgSign
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CXXFLAGS=$(ERR_CXXFLAGS) $(WARN_FLAGS)

CryptMsgSign_SOURCES=CryptMsgSign.cpp
CryptMsgSign_LDADD= \
	@top_builddir@/CSP/capilite/libcapi20.la \
	@top_builddir@/CSP/capilite/libcapi10.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
