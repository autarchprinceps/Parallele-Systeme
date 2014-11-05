#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int err, rank, n;
	int* recbuf;
	int* sendbuf;
	
	err = MPI_Init(&argc, &argv);
	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	err = MPI_Comm_size(MPI_COMM_WORLD, &n);
	sendbuf = malloc(sizeof(int));
	*sendbuf = rank + 1;
	if(rank == 0) {
		recbuf = malloc(sizeof(int) * n);
	}
	MPI_Gather(sendbuf, 1, MPI_INT, recbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);
	err = MPI_Finalize();
	if(rank == 0) {
		for(int i = 0; i < n; i++) {
			printf("%i ", recbuf[i]);
		}
		printf("\n");
	}
	return 0;
}