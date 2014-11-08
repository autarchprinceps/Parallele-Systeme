FC	= ifort
FFLAGS	= -O0 -mcmodel=large -shared-intel
LDFLAGS	= -mcmodel=large -shared-intel

default: kernel.exe

run: kernel.exe
	./kernel.exe

clean:
	-rm -f *.o *.exe

kernel.o: kernel.f kernel.inc
	$(FC) -c $(FFLAGS) $<

%.exe: %.o
	$(FC) $(LDFLAGS) -o $@ $< $(LDLIBS)
