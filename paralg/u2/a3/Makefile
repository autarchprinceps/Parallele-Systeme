CC	= icc -openmp
CFLAGS	= -std=c99 -O3 -g -I/usr/local/include
LDLIBS	= -L/usr/local/lib -lFHBRS -lpthread -lm

default:: u2a3.exe

run::
	./u2a3.exe

clean::
	-rm -f *.exe *.o


u2a3.exe: main.o prefix.o
	$(CC) -o $@ $^ $(LDLIBS)


main.o: main.c
prefix.o: prefix.c prefix.h
