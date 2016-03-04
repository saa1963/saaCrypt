noinst_PROGRAMS=pcsctest

AM_CPPFLAGS=-I@top_srcdir@ -I@srcdir@/../../../include\
	-I@srcdir@/../../../include/reader
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

pcsctest_SOURCES=test.c pcsctprj.c dummy.cpp pcsctprj.h
pcsctest_CPPFLAGS=$(AM_CPPFLAGS) -DTHROUGH_DLL
pcsctest_LDADD= @builddir@/../../rdr/librdrrdr2.la\
	@builddir@/../../support/librdrsup.la\
        @builddir@/../../rdr/test/librdrrdr_t.la\
        @builddir@/../../support/sstest/librdrsup_t.la\
	$(add_libs) @PTHREAD_LIBS@ $(LIBGCC_A)
