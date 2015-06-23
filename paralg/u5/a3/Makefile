CC = gcc -openmp
CFLAGS	= -DNDEBUG -Wall -std=c99 -g -O2 -openmp -I/usr/local/libFHBRS/
LDLIBS	= -lm # -lFHBRS

default:: int.exe

run:: int.exe
	./int.exe

clean::
	-rm -f *.o *.exe
	
int.exe: main.o
	$(CC) -o $@ $^ $(LDLIBS)
	
main.o: main.c
