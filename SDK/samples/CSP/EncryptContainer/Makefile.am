srctestdir=$(prefix)/src/doxygen/CSP/EncryptContainer
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/EncryptContainer.c
noinst_PROGRAMS=EncryptContainer
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

EncryptContainer_SOURCES=EncryptContainer.c
EncryptContainer_LDADD= \
	@top_builddir@/CSP/capilite/libcapi10.la \
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
