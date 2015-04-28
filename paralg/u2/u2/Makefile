CC = clang
CFLAGS	= -std=c99 -Wall -O3 -I/usr/local/libFHBRS
LDLIBS	= -L/usr/local/libFHBRS -lFHBRS

default: u2.exe

u2.exe: main.o list.o tree.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	
main.o: main.c

list.o: list.c

tree.o: tree.c
