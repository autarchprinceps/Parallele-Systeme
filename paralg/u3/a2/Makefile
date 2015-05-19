########################################################################

MPICC		= mpicc
# CFLAGS		= -O2 -std=c99 -I/usr/local/include -Wall
CFLAGS		= -O2 -std=c99 -I/usr/local/libFHBRS -Wall
# LDLIBS		= -L/usr/local/lib -lFHBRS -lm
LDLIBS		= -L/usr/local/libFHBRS -lFHBRS -lm

########################################################################

compile:: sort.exe

clean::
	-rm -f *.exe *.o

run:: sort.exe
# only as an example. possible only inside a batch script
	mpirun -np 16 sort.exe


########################################################################

%.o: %.c
	$(MPICC) $(CFLAGS) -c $<
%.exe: %.o
	$(MPICC) -o $@ $< $(LDLIBS)

########################################################################
