if !IOS
noinst_PROGRAMS=fat12test
endif
rdktestdir=@prefix@/src/rdk/fat12/test
rdktest_DATA=$(fat12test_SOURCES) Makefile.rel

AM_CPPFLAGS=\
	-I@top_srcdir@\
	-I@srcdir@/../../../include\
	-I@srcdir@/../../../include/reader
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

fat12test_SOURCES=test.cpp tst12prj.c tst12prj.h
fat12test_CPPFLAGS=-DTHROUGH_DLL $(AM_CPPFLAGS)
fat12test_LDFLAGS=-no-install
fat12test_LDADD=\
	@builddir@/../librdrfat12.la\
	@builddir@/../../rdr/librdrrdr2.la\
	@builddir@/../../support/sstest/librdrsup_t.la\
	@builddir@/../../rdr/test/librdrrdr_t.la\
	@builddir@/../../support/librdrsup.la\
	@PTHREAD_LIBS@
