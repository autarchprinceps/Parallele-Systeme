#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int err, rank, n;
	int* recbuf, sendbuf;
	
	err = MPI_Init(&argc, &argv);
	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	err = MPI_Comm_size(MPI_COMM_WORLD, &n);
	sendbuf = malloc(sizeof(int));
	*sendbuf = rank + 1;
	if(rank == 0) {
		recbuf = malloc(sizeof(int));
	}
	MPI_Gather(sendbuf, 1, MPI_INT, recbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);
	err = MPI_Finalize();
	return 0;
}