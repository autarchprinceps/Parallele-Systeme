#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>

int main(int argc, char **argv) {
	int err, rank, n;

	err = MPI_Init(&argc, &argv);
	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	err = MPI_Comm_size(MPI_COMM_WORLD, &n);	
	// TODO
	err = MPI_Finalize();
	return 0;
}
