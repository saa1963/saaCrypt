@makefile_master@

SUBDIRS=Win32 win_ms
lib_LTLIBRARIES=librdrric.la
AM_LIBTOOLFLAGS=--tag=disable-static
BUILT_SOURCES=ric_rc.c ric_rc.h
lang_DATA=librdrric.cat librdrric_cp1251.cat librdrric_utf-8.cat librdrric_iso8859-5.cat librdrric_cp866.cat librdrric_koi8-r.cat
langdir=$(CSP_DEF_LDIR)
rdkdir=@prefix@/src/rdk/ric
rdk_DATA=Makefile.rel $(dist_librdrric_la_SOURCES) win_ms/Version.Win32.rc 
MOSTLYCLEANFILES=$(BUILT_SOURCES) $(lang_DATA) rc_tmp.c cpp.out err.out

AM_CPPFLAGS=-I@top_srcdir@ -I@srcdir@/../../include\
	-I@srcdir@/../../include/reader
AM_CFLAGS=$(ERR_CFLAGS) $(NOSTRICT_ALIASING_FLAGS_3x) $(NOSTRICT_ALIASING_FLAGS_41) $(NOSTRICT_ALIASING_FLAGS_42) $(WARN_FLAGS)

nodist_librdrric_la_SOURCES=$(BUILT_SOURCES)
dist_librdrric_la_SOURCES=osc_reg.c oscchp.c oscuform.c\
	ric_clos.c ric_con.c ric_dup.c ric_err.c ric_free.c\
	ric_inst.c ric_int.h ric_limit.c ric_log.c ric_lout.c\
	ric_pch.c ric_plen.c ric_prj.c ric_prj.h\
	ric_same.c ric_uni.c ric_unreg.c ric_vpin.c\
	ricuni.c ric_disc.c\
	osc2_flen.c osc2_fop.c osc2_is.c osc2_offs.c osc2_open.c osc2_rd.c\
	osc2_wr.c osc2ids.c osc2sf.c osc2_rest.c \
	ric_is.c osc_is.c ric_fop.c ric_rd.c ric_open.c ric_rest.c \
	ric_reg.c oscids.c ricchp.c ric_wr.c ricuform.c ricsf.c\
	mag_chp.c mag_con.c mag_dup.c mag_fld.c mag_fop.c\
	mag_is.c mag_is_impl.h mag_log.c mag_lout.c\
	mag_open.c mag_rc.c mag_reg.c mag_rest.c mag_same.c\
	mag_sf.c mag_uform.c mag_uni.c mag_unig.c mag_vpin.c
librdrric_la_LDFLAGS=-version-info @LT_VERSIONINFO@
librdrric_la_LIBADD=@builddir@/../support/librdrsup.la @builddir@/../librdrver.la

if LINUX
librdrric_la_LIBADD+=@PTHREAD_LIBS@
endif # LINUX
if USE_SYMFILES
dist_librdrric_la_SOURCES+= librdrric.sym
librdrric_la_LDFLAGS+= -export-symbols @srcdir@/librdrric.sym
endif # USE_SYNFILES

$(BUILT_SOURCES) $(lang_DATA): @srcdir@/Win32/ric_rc.rc @builddir@/../copyex/derc.sh @builddir@/../copyex/localize.sh
	@builddir@/../copyex/derc.sh @srcdir@/Win32/ric_rc.rc @srcdir@/win_ms/Version.Win32.rc ric_prj.h @srcdir@/Win32 RIC @builddir@/ric_rc
