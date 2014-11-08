# compile
default::
	(cd bubblesort; $(MAKE))
	(cd gauss; $(MAKE))
	(cd gauss_pivot; $(MAKE))
	(cd heat; $(MAKE))
	(cd matmul; $(MAKE))
	(cd quicksort; $(MAKE))
	(cd stream; $(MAKE))

# run normal program
run::
	(cd bubblesort; $(MAKE) run)
	(cd gauss; $(MAKE) run)
	(cd gauss_pivot; $(MAKE) run)
	(cd heat; $(MAKE) run)
	(cd matmul; $(MAKE) run)
	(cd quicksort; $(MAKE) run)
	(cd stream; $(MAKE) run)

# run program with dynamic instrumentation and cache simulator
batch::
	(cd bubblesort; qsub job.sh)
	(cd gauss; qsub job.sh)
	(cd gauss_pivot; qsub job.sh)
	(cd heat; qsub job.sh)
	(cd matmul; qsub job.sh)
	(cd quicksort; qsub job.sh)
	(cd stream; qsub job.sh)

# clean up
clean::
	(cd bubblesort; $(MAKE) clean)
	(cd gauss; $(MAKE) clean)
	(cd gauss_pivot; $(MAKE) clean)
	(cd heat; $(MAKE) clean)
	(cd matmul; $(MAKE) clean)
	(cd quicksort; $(MAKE) clean)
	(cd stream; $(MAKE) clean)
