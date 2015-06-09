# Intel compiler
CC	= icc -openmp
CFLAGS	= -DNDEBUG -Wall -std=c99 -g -O2 -openmp -I/usr/local/include

LDLIBS	= -lFHBRS


default:: prim.exe

run:: prim.exe
	./prim.exe 30000 0.1

clean::
	-rm -f *.o *.exe

prim.exe: prim.o graph_dense.o set.o
	$(CC) -o $@ $^ $(LDLIBS)

graph_dense.o: graph_dense.c graph_dense.h
prim.o: prim.c set.h graph_dense.h
set.o: set.c set.h
