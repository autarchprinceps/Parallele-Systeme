#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <libFHBRS.h>

void leastSquareFit
  (
   int n,                      // I: vector length
   double x[n],		       // I: x values
   double y[n],		       // I: y values
   double *alpha,	       // O: intercept
   double *beta                // O: slope
)

{
  double sumx, sumy, sumxy, sumx2;

  assert(n > 0);

  sumx = sumy = sumxy = sumx2 = 0.0;
  for (int i = 0; i < n; i++)
    {
      sumx += x[i];
      sumy += y[i];
      sumxy += x[i] * y[i];
      sumx2 += x[i] * x[i];
    }

  if (n == 1)
    {
      *alpha = y[0];
      *beta = 0.0;
    }
  else
    {
      double det = (n * sumx2) - (sumx * sumx);
      *alpha = ((sumy * sumx2) - (sumxy * sumx)) / det;
      *beta = ((n * sumxy) - (sumx * sumy)) / det;
    }
}

int rank, n;
#define ITERMAX 100
typedef struct {
	double alpha;
	double beta;
	double n;
	double* x;
	double* y;
} resultset;
void pp(unsigned int MSGLENSTART, unsigned int MSGLENMAX, unsigned int MSGLENINC, resultset* log) {
	log->n = (MSGLENMAX - MSGLENSTART) / MSGLENINC;
	log->x = malloc(sizeof(double) * log->n);
	log->y = malloc(sizeof(double) * log->n);
	char* buf = calloc(sizeof(char), MSGLENMAX);
	for(unsigned int i = 0; i < log->n; i++) {
		unsigned int msglen = MSGLENSTART + i * MSGLENINC;
		double tmin = 999999999;
		// printf("DEBUG %d: outerloop %d %d\n", rank, i, msglen);
		for(int iter = 0; iter < ITERMAX; iter++) {
			// printf("DEBUG %d: prebarrier %d\n", rank, iter);
			MPI_Barrier(MPI_COMM_WORLD);
			// printf("DEBUG %d: postbarrier %d\n", rank, iter);
			if(rank == 0) {
				double t0 = gettime();
				// printf("DEBUG %d: presend\n", rank);
				MPI_Send(buf, msglen, MPI_BYTE, n-1, 0, MPI_COMM_WORLD);
				MPI_Status status;
				// printf("DEBUG %d: postsend\n", rank);
				MPI_Recv(buf, msglen, MPI_BYTE, n-1, 0, MPI_COMM_WORLD, &status);
				// printf("DEBUG %d: postrecv\n", rank);
				t0 = (gettime() - t0) / 2;
				if(t0 < tmin) tmin = t0;
			} else if(rank == n-1) {
				MPI_Status status;
				// printf("DEBUG %d: prerecv\n", rank);
				MPI_Recv(buf, msglen, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
				// printf("DEBUG %d: postrecv\n", rank);
				MPI_Send(buf, msglen, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
				// printf("DEBUG %d: postsend\n", rank);
			}
		}
		if(rank == 0) {
			// printf("DEBUG %d: prelog %d %f\n", rank, msglen, tmin);
			log->x[i] = msglen;
			log->y[i] = tmin;
			// printf("DEBUG %d: postlog %d %f\n", rank, msglen, tmin);
		}
	}
	if(rank == 0) {
		// printf("DEBUG %d: prelsf\n", rank);
		leastSquareFit(log->n, log->x, log->y, &log->alpha, &log->beta);
	}
}

void printRS(char* filename, resultset* log) {
	FILE* f = fopen(filename, "w+");
	fprintf(f, "alpha;%f;beta;%f\n", log->alpha, log->beta);
	fprintf(f, "msglen;tmin\n");
	for(unsigned int i = 0; i < log->n; i++) {
		fprintf(f, "%f;%f\n", log->x[i], log->y[i]);
	}
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n);

	resultset* log = malloc(sizeof(resultset));
	pp(0, 16384, 256, log);
	if(rank == 0) {
		printRS("pp.0-2_14.csv", log);
	}
	pp(16384, 16777216, 261888, log);
	if(rank == 0) {
		printRS("pp.2_14-2_24.csv", log);
	}

	MPI_Finalize();
}
