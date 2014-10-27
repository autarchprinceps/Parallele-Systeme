CC	= kinst-ompp-papi icc -openmp
#CC	= gcc -fopenmp
CFLAGS	= -O2 -g -I/usr/local/include -std=c99 -Wall
LDFLAGS	= -L/usr/local/lib -lFHBRS -lX11 -lm
DEBUGFLAGS = -g -pg 

default:: heat.exe

run:: heat.exe
	./heat.exe 2000 5000 0

clean::
	-rm -f heat.exe heat.o

heat.exe: heat.o
	$(CC) -o $@ $^ $(LDFLAGS)

debug.v1: heat.v1.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $@ $^ $(LDFLAGS)

debug.v2: heat.v2.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $@ $^ $(LDFLAGS)
