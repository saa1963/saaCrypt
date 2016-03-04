srctestdir=$(prefix)/src/doxygen/CSP/EncryptFile
srctest_DATA=@srcdir@/Makefile.unix @srcdir@/EncryptFile.c
noinst_PROGRAMS=EncryptFile
AM_CPPFLAGS= \
	-I@top_srcdir@ \
        -I@top_srcdir@/CSP/src/include \
        -I@top_srcdir@/shared/include
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

EncryptFile_SOURCES=EncryptFile.c
EncryptFile_LDADD= \
	@top_builddir@/CSP/capilite/libcapi20.la\
        @DL_LIBS@ @BIN_LIBS@ @NET_LIBS@ @PTHREAD_LIBS@ @COMPAT_LIBS@

EXTRA_DIST=Makefile.unix
