
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall -std=c++11
ISPC=ispc
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

default: calpi

.PHONY: dirs clean
.PRECIOUS: objs/calpi.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ calpi

calpi: dirs  objs/calpi.o objs/calpi_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/calpi.o objs/calpi_ispc.o

objs/calpi.o: calpi.cpp objs/calpi_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
