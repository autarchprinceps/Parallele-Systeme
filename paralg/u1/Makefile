# sequential compiler
CC	= gcc
# MPI compiler
MPICC	= mpicc
# compiler flags for all version
CFLAGS	= -std=c99 -Wall -O2 -g -I/usr/local/include
# libraries for all versions
LDLIBS	= -L/usr/local/lib -lFHBRS
# default matrix size for test run
N	= 5000
# default number of processors for test run (N%P == 0!)
P	= 2

default:: floyd_seq.exe floyd_mpi.exe least_square_fit.exe pp.exe

run:: floyd_seq.exe floyd_mpi.exe
	./floyd_seq.exe $(N)
	mpirun -np $(P) ./floyd_mpi.exe $(N)

clean::
	-rm -f *.exe *.o


floyd_seq.exe: floyd_seq.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

floyd_mpi.exe: floyd_mpi.o
	$(MPICC) $(CFLAGS) -o $@ $^ $(LDLIBS)
floyd_mpi.o: floyd_mpi.c
	$(MPICC) $(CFLAGS) -c $^

least_square_fit.exe: least_square_fit.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

least_square_fit.o: least_square_fit.c

pp.exe: pp.p
	$(MPICC) $(CFLAGS) -o $@ $^ $(LDLIBS)
pp.p: ping_pong.c
	$(MPICC) $(CFLAGS) -c $^