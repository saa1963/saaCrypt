if !IOS
noinst_PROGRAMS=rndm_test
endif
rdktestdir=@prefix@/src/rdk/rndm/test
rdktest_DATA=$(dist_rndm_test_SOURCES) Makefile.rel

AM_CPPFLAGS=-I@top_srcdir@ -I@srcdir@/../../../include -DBIO_RNDM -DBUFFER_LEN=100
AM_CFLAGS=$(ERR_CFLAGS) $(WARN_FLAGS)

dist_rndm_test_SOURCES=test.c rndm_tst.h dummy.cpp
rndm_test_LDADD=@builddir@/../librdrrndm2.la @builddir@/../../support/librdrsup.la @PTHREAD_LIBS@ $(LIBGCC_A)

