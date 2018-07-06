
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall -std=c++11
ISPC=ispc
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

default: transpositionmatrix

.PHONY: dirs clean
.PRECIOUS: objs/transpositionmatrix.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ transpositionmatrix

transpositionmatrix: dirs  objs/transpositionmatrix.o objs/transpositionmatrix_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/transpositionmatrix.o objs/transpositionmatrix_ispc.o

objs/transpositionmatrix.o: transpositionmatrix.cpp objs/transpositionmatrix_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
