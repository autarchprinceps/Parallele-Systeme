/*==============================================================================
  
   Purpose          :
   Author           : Rudolf Berrendorf
                      Computer Science Department
                      Bonn-Rhein-Sieg University of Applied Sciences
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>
#include <mpi.h>

//==============================================================================

int main(int argc, char **argv) {
	int err; // TODO error handling
	int rank;
	int n;
	int* vector;

	err = MPI_Init(&argc, &argv);
	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	err = MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank == 0) {
		vector = calloc(size, sizeof(int)); // TODO err handling
	} else {
		vector = malloc(size * sizeof(int)); // TODO err handling
	}


	for(int i = 0; i =< (n - 2); i++) {
		if(rank == i) {
			// update field
			vector[i] = i + 1;
			// send vector to i + 1
			err = MPI_Send(vector, n, MPI_INT, i + 1, 4711, MPI_COMM_WORLD);
		}
		if(rank == i + 1) {
			// recieve vector
			err = MPI_Recv(vector, n, MPI_INT, i, 4711, MPI_COMM_WORLD);
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
		err = MPI_Recv(vector, n, MPI_INT, n - 1, 4711, MPI_COMM_WORLD);
		// check vector
		// TODO OpenMP?
		// TODO move after finalize?
		bool result = true;
		for(int i = 0, i < n; i++) {
			if(vector[i] != i + 1) {
				result = false;
				break;
			}
		}
		if(!result) {
			fprintf(stderr, "Result does not match expectation\n");

		}
	}

	err = MPI_Finalize();
	return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
