
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall -std=c++11
ISPC=ispc
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

default: multiplymatrix

.PHONY: dirs clean
.PRECIOUS: objs/multiplymatrix.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ multiplymatrix

multiplymatrix: dirs  objs/multiplymatrix.o objs/multiplymatrix_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/multiplymatrix.o objs/multiplymatrix_ispc.o

objs/multiplymatrix.o: multiplymatrix.cpp objs/multiplymatrix_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
