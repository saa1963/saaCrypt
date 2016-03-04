@makefile_master@

SUBDIRS=Win32 . test win_ms
EXTRA_DIST=f12acall.c f12uget.c f12uset.c
lib_LTLIBRARIES=librdrfat12.la
sbin_SCRIPTS=mount_flash.sh
AM_LIBTOOLFLAGS=--tag=disable-static
BUILT_SOURCES=fat12_rc.c fat12_rc.h
lang_DATA=librdrfat12.cat librdrfat12_cp1251.cat librdrfat12_utf-8.cat librdrfat12_iso8859-5.cat librdrfat12_cp866.cat librdrfat12_koi8-r.cat
langdir=$(CSP_DEF_LDIR)
rdkdir=@prefix@/src/rdk/fat12
rdk_DATA=Makefile.rel $(dist_librdrfat12_la_SOURCES) win_ms/Version.Win32.rc 
MOSTLYCLEANFILES=$(BUILT_SOURCES) $(lang_DATA) rc_tmp.c cpp.out err.out

if IOS # было if STATIC
noinst_LTLIBRARIES=librdrfat12_tmp.la
librdrfat12_tmp_la_LIBTOOLFLAGS=
noinst_DATA=librdrfat12_static.o
endif

AM_CPPFLAGS=-I@top_srcdir@ -I@srcdir@/../../include -I@srcdir@/../../include/reader
AM_CFLAGS=$(ERR_CFLAGS) $(NOSTRICT_ALIASING_FLAGS_3x) $(NOSTRICT_ALIASING_FLAGS_41) $(NOSTRICT_ALIASING_FLAGS_42) $(WARN_FLAGS)

nodist_librdrfat12_la_SOURCES=$(BUILT_SOURCES)
dist_librdrfat12_la_SOURCES=fat12prj.c fat12prj.h fat12int.h fat12inf.h hdimginf.h\
	f12uform.c f12tvol.c f12istat.c f12dsnxt.c f12iname.c f12inick.c \
	f12iflag.c f12dcon.c f12isame.c hdis.c f12lout.c f12trest.c \
	f12kcls.c f12rreg.c f12kopen.c f12kunl.c hdiflag.c f12kchs.c f12cfree.c f12dcl.c \
	hdids.c f12kread.c hdreg.c f12iinst.c f12kwri.c f12cdup.c  f12klen.c f12con.c f12dsop.c \
	f12lin.c f12dscl.c f12dcnxt.c hdidef.c f12dcop.c f12dclr.c f12lock.c f12dccl.c f12dopen.c \
	fat12.c ehdids.c ehdiflag.c ehdis.c f12grpc.c f12grpn.c f12grpo.c f12def.c
librdrfat12_la_LDFLAGS=-version-info @LT_VERSIONINFO@
librdrfat12_la_LIBADD=@builddir@/../support/librdrsup.la @builddir@/../librdrver.la @FLOPPY_LIBS@

if LINUX
librdrfat12_la_LIBADD+=@PTHREAD_LIBS@
endif # LINUX
if USE_SYMFILES
dist_librdrfat12_la_SOURCES+= librdrfat12.sym
librdrfat12_la_LDFLAGS+= -export-symbols @srcdir@/librdrfat12.sym
endif # USE_SYMFILES

if IOS # было if STATIC
librdrfat12_tmp_la_SOURCES=
librdrfat12_tmp_la_LIBADD=$(librdrfat12_la_OBJECTS)

librdrfat12_static.o: librdrfat12_tmp.la librdrfat12.syms
	$(LD) -r -arch @ARCH@ -o $@ -exported_symbols_list librdrfat12.syms -all_load .libs/librdrfat12_tmp.a

librdrfat12.syms: @srcdir@/librdrfat12.sym	
	@SED@ 's/^/_/' < $< >$@
endif	


$(BUILT_SOURCES) $(lang_DATA): @srcdir@/Win32/fat12_rc.rc @builddir@/../copyex/derc.sh @builddir@/../copyex/localize.sh
	@builddir@/../copyex/derc.sh @srcdir@/Win32/fat12_rc.rc @srcdir@/win_ms/Version.Win32.rc fat12prj.h @srcdir@/Win32 FAT12 @builddir@/fat12_rc
