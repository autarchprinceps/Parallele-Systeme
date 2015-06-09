# !!! do not touch anything in this file !!!
CC	= icc -openmp
CFLAGS	= -std=c99 -g -O2 -Wall -I/usr/local/graph_stud/common -I/usr/local/graph_stud/graph -DWITH_INLINING
LDLIBS	= -L/usr/local/graph_stud/bin -lgraph

default:: bfs.exe

compile:: bfs.exe

run:: bfs.exe
	./bfs.exe /scratch/src/data/graphs/bin64/rmat/rmat_100M_8000M_0.30_0.25_0.26.bin64

test:: bfs.exe
	./bfs.exe /scratch/src/data/graphs/bin64/delaunay/delaunay_n10.bin64

clean::
	-rm -f *.o *.exe

bfs.exe: bfs.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
