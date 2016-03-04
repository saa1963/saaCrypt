@makefile_master@

SUBDIRS=Win32
noinst_LTLIBRARIES=librdrrndm0.la
AM_LIBTOOLFLAGS=--tag=disable-static
BUILT_SOURCES=rndm0_rc.c rndm0_rc.h
lang_DATA=librdrrndm0.cat librdrrndm0_cp1251.cat librdrrndm0_utf-8.cat librdrrndm0_iso8859-5.cat librdrrndm0_cp866.cat librdrrndm0_koi8-r.cat
langdir=$(CSP_DEF_LDIR)
rdkdir=@prefix@/src/rdk/rndm0
rdk_DATA=Makefile.rel $(dist_librdrrndm0_la_SOURCES) win_ms/Version.Win32.rc 
MOSTLYCLEANFILES=$(BUILT_SOURCES) $(lang_DATA) rc_tmp.c cpp.out err.out

AM_CPPFLAGS=-I@top_srcdir@ -I@srcdir@/../../include -I@srcdir@/../../include/reader
AM_CFLAGS=$(ERR_CFLAGS) $(NOSTRICT_ALIASING_FLAGS_3x) $(NOSTRICT_ALIASING_FLAGS_41) $(NOSTRICT_ALIASING_FLAGS_42) $(WARN_FLAGS)

nodist_librdrrndm0_la_SOURCES=$(BUILT_SOURCES)
dist_librdrrndm0_la_SOURCES=r0_prj.c r0_prj.h r0_exp.c
librdrrndm0_la_LDFLAGS=-version-info @LT_VERSIONINFO@
librdrrndm0_la_LIBADD=@builddir@/../librdrver.la

if USE_SYMFILES
dist_librdrrndm0_la_SOURCES+= librdrrndm0.sym
librdrrndm0_la_LDFLAGS+= -export-symbols @srcdir@/librdrrndm0.sym
endif # USE_SYMFILES

$(BUILT_SOURCES) $(lang_DATA): @srcdir@/Win32/rndm0_rc.rc @builddir@/../copyex/derc.sh @builddir@/../copyex/localize.sh
	@builddir@/../copyex/derc.sh @srcdir@/Win32/rndm0_rc.rc @srcdir@/win_ms/Version.Win32.rc r0_prj.h @srcdir@/Win32 RNDM0 @builddir@/rndm0_rc
