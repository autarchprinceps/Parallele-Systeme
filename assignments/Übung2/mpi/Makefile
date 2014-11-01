########################################################################

MPICC		= mpecc
CFLAGS		= -O2 -std=c99 -I/usr/local/include -Wall -mpilog
LDLIBS		= -L/usr/local/lib -lFHBRS -lm -mpilog

########################################################################

compile:: ring.exe

clean::
	-rm -f *.exe *.o

run:: ring.exe
# only as an example. possible only inside a batch script
	mpirun -np 16 ring.exe


########################################################################

%.o: %.c
	$(MPICC) $(CFLAGS) -c $<
%.exe: %.o
	$(MPICC) -o $@ $< $(LDLIBS)

########################################################################
