CC	= icc -openmp
CFLAGS	= -std=c99 -O3 -g -I/usr/local/include
LDLIBS	= -L/usr/local/lib -lFHBRS -lpthread -lm

default:: u2a2.exe

run::
	./u2a2.exe

clean::
	-rm -f *.exe *.o


u2a2.exe: main.o tree.o
	$(CC) -o $@ $^ $(LDLIBS)


main.o: main.c
tree.o: tree.c tree.h
