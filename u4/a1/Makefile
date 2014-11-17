# use either standard or optimized version

# standard version (needs any BLAS compatible library)
#CC	= cc
#CFLAGS	= -O -std=c99 -I/usr/local/include
#LINKER	= ifort -nofor-main
#LDLIBS	= -L/usr/local/lib -lFHBRS -lblas -lm

# optimized version using ACML (needs AMD processors and ACML installed)
CC	= cc
CFLAGS	= -DACML -O -std=c99 -I/usr/local/include -I/usr/local/acml/ifort64/include
LINKER	= ifort -nofor-main
LDLIBS	= -L/usr/local/lib -lFHBRS -L/usr/local/acml/ifort64/lib -lacml -lm

default:: matmul_c.exe

run:: matmul_c.exe
	./matmul_c.exe

clean::
	-rm -f *.o *.exe

matmul_c.exe: matmul_c.o
	$(LINKER) -o $@ $^ $(LDLIBS)

