.SUFFIXES: .cpp
#CC=gcc

#CCC=g++

CPCCC=$(CCC)$(CXX)
# For FreeBSD you should add -pthread to LDFLAGS.
CFLAGS=-DUNIX -DHAVE_LIMITS_H $(ARCH_FLAGS) $(add_CPPFLAGS) -I$(CSP_INCLUDE) -I$(CSP_INCLUDE)/cpcsp -I$(CSP_INCLUDE)/asn1c/rtsrc -I$(CSP_INCLUDE)/asn1data -DSIZEOF_VOID_P=$(SIZEOF_VOID_P) -g
LDFLAGS= $(ARCH_FLAGS) -L $(CSP_LIB) -lasn1data -lssp -lcapi10 -lcapi20 $(CSP_EXTRA_LIBS) $(add_ldflags) -g

OBJS=$(out).o
$(out): $(OBJS)
	$(CPCCC) $(OBJS) $(LDFLAGS) -o $@ $(add_libs)
clean:
	for i in $(out) $(OBJS) ; do test -r "$${i}" || continue ; rm "$${i}" ; done
.cpp.o:
	$(CPCCC) $(CFLAGS) -c -o $@ $<
