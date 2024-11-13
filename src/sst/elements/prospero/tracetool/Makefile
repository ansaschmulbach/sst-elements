#!/bin/bash

PINDIR=${INTEL_PIN_DIRECTORY}
CXX=g++
PREPROC=-DBIGARRAY_MULTIPLIER=1 -DUSING_XED -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -DPROSPERO_LIBZ -DPIN_CRT=1
LD=g++
CC=gcc

all: sstmemtrace.so

sstmemtrace.so: sstmemtrace.o
	$(LD) -Wl,--hash-style=sysv -shared -Wl,-Bsymbolic \
                -Wl,--version-script=$(PINDIR)/source/include/pin/pintool.ver -fabi-version=2 \
                -L$(PINDIR)/extras/xed-intel64/lib \
                -L$(PINDIR)/intel64/lib \
                -L$(PINDIR)/intel64/runtime/pincrt \
                -L$(PINDIR)/intel64/lib-ext \
                -o $@ $< \
                -L$(PINDIR)/extras/xed-intel64/lib \
                -L$(PINDIR)/intel64/lib \
                -L$(PINDIR)/intel64/runtime/pincrt \
                -L$(PINDIR)/intel64/lib-ext -lpin -lxed \
                $(PINDIR)/intel64/runtime/pincrt/crtendS.o -lpindwarf -ldwarf -ldl-dynamic -nostdlib -lc++ -lc++abi -lm-dynamic -lc-dynamic -lunwind-dynamic

sstmemtrace.o: sstmemtrace.cc
	$(CXX) -g -c -O3 -fno-stack-protector -fno-exceptions -funwind-tables -fasynchronous-unwind-tables -fno-rtti -fomit-frame-pointer -fabi-version=2 -faligned-new \
        -Wl,-Bsymbolic \
        -Wl,--version-script=$(PINDIR)/source/include/pintool.ver \
        -I $(PINDIR)/source/include/pin \
        -I $(PINDIR)/source/include/pin/gen \
        -I $(PINDIR)/extras/xed-intel64/include/xed \
        -isystem $(PINDIR)/extras/cxx/include \
        -isystem $(PINDIR)/extras/crt/include \
        -isystem $(PINDIR)/extras/crt/include/arch-x86_64 \
        -isystem $(PINDIR)/extras/crt/include/kernel/uapi \
        -isystem $(PINDIR)/extras/crt/include/kernel/uapi/asm-x86 \
        -I $(PINDIR)/extras/components/include \
        $(PREPROC) \
        -o $@ \
        $<

clean:
	rm -f *.o sstmemtrace.so sstmemtrace.dylib
