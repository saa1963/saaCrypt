SUBDIRS=. CSP SSPI
srctestdir=$(prefix)/src/doxygen
srctest_DATA=examples.unix.mk @srcdir@/readme.txt
if DARWIN
srctest_DATA+=examples.macos.mk
endif #DARWIN
srctest_SCRIPTS=setenv.sh
EXTRA_DIST=examples.unix.mk setenv.sh.in readme.txt

