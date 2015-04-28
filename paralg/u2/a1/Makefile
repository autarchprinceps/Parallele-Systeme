CC = clang
CFLAGS	= -std=c99 -Wall -O3 -I/usr/local/libFHBRS
LDLIBS	= -L/usr/local/libFHBRS -lFHBRS

default: a1.exe

a1.exe: main.o list.o tree.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	
main.o: main.c

list.o: list.c

tree.o: tree.c
