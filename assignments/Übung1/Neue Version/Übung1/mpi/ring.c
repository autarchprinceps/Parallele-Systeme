#include <stdio.h>
#include <stdlib.h>
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
	if(rank > 0) {
		// recieve vector
		err = MPI_Recv(vector, n, MPI_INT, rank - 1, 4711, MPI_COMM_WORLD, &status);
	}

	// update field
	vector[rank] = rank + 1;
	// send vector to i + 1
	err = MPI_Send(vector, n, MPI_INT, (rank + 1) % n, 4711, MPI_COMM_WORLD);

	if(rank == 0) {
		// recieve final vector
		err = MPI_Recv(vector, n, MPI_INT, n - 1, 4711, MPI_COMM_WORLD, &status);
		// check vector
		int result = 1;
		for(i = 0; i < n; i++) {
			if(vector[i] != i + 1) {
				result = 0;
				break;
			}
		}
		for(i = 0; i < n; i++) {
			printf("%i ", vector[i]);
		}
		printf("\n");
		if(result == 0) {
			fprintf(stderr, "Result does not match expectation\n");

		} else {
			printf("Result matches expectation\n");
		}
	}
	
	err = MPI_Finalize();
	return 0;
}
