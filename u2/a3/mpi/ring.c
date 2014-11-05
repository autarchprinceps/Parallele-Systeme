#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int err, rank, n, i;
	int* vector;
	MPI_Status status;

	err = MPI_Init(&argc, &argv);
	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	err = MPI_Comm_size(MPI_COMM_WORLD, &n);
	
	if(rank == 0) {
		vector = calloc(n, sizeof(int));
	} else {
		vector = malloc(n * sizeof(int));
	}


	for(i = 0; i <= (n - 2); i++) {
		if(rank == i) {
			// update field
			vector[i] = i + 1;
			// send vector to i + 1
			err = MPI_Send(vector, n, MPI_INT, i + 1, 4711, MPI_COMM_WORLD);
		}
		if(rank == i + 1) {
			// recieve vector
			err = MPI_Recv(vector, n, MPI_INT, i, 4711, MPI_COMM_WORLD, &status);
		}
	}

	if(rank == (n - 1)) {
		// update last field
		vector[n - 1] = n;
		// send vector to 0
		err = MPI_Send(vector, n, MPI_INT, 0, 4711, MPI_COMM_WORLD);
	}
	if(rank == 0) {
		// recieve vector
		err = MPI_Recv(vector, n, MPI_INT, n - 1, 4711, MPI_COMM_WORLD, &status);
		// check vector
		// TODO OpenMP?
		int result = 1;
		for(i = 0; i < n; i++) {
			if(vector[i] != i + 1) {
				result = 0;
				break;
			}
		}
		if(result == 0) {
			fprintf(stderr, "Result does not match expectation\n");

		} else {
			for(i = 0; i < n; i++) {
				printf("%i ", vector[i]);
			}
			printf("Result matches expectation\n");
		}
	}
	
	err = MPI_Finalize();
	return 0;
}
