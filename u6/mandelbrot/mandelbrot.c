/*==============================================================================
  
   Purpose:    MPI Mandelbrot (master/slave approach)
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>

#include <mpi.h>
#include <metis.h>

#include <libFHBRS.h>


//==============================================================================
// constants

#define NROW 1500                      // number of rows

#define	X_RESOLUTION NROW	       // number of pixels in x-direction (rows)
#define	Y_RESOLUTION NROW	       // number of pixels in y-direction (columns)

// debug output (0=nothing, 1=moderate, 2=more details)
#define DEBUG 0


//==============================================================================
// global variables

// MPI size and rank
static int size, rank;

// checksum if we don't display
static unsigned long checksum;


//==============================================================================
/*  Only executed on master processor (üprocessor 0).
    Receive one row from a slave processor.
*/

static void
receive_data ()
{
  int anziter[Y_RESOLUTION + 1];
  MPI_Status status;
  int err;


  // processor 0 receives information
  err = MPI_Recv (anziter, Y_RESOLUTION+1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  assert(err == MPI_SUCCESS);

  // compute checksum (without line number in first place)
  for (int j = 1; j <= Y_RESOLUTION; j++)
    checksum += anziter[j];
}

//==============================================================================
/* Only executed on processors other than 0.
   Get gather data on a point (and in a non-batch environment we would display this pixel).
*/

static void
drawPoint (int i, int j, int anziter)
{
  // static: lifetime over all calls to the function
  static int iter[Y_RESOLUTION+1];

  /* processor 1..n gather information for a complete row
     and send the complete row to processor 0
  */

  // store row number
  iter[0] = i;

  // store value of (i,j)
  iter[j+1] = anziter;

  // last pixel in row?
  if (j == Y_RESOLUTION-1)
    {
      // end of row reached: send whole row to processor 0
      // we assume here that a processor computes all pixels of a row in sequence
      int err = MPI_Send (iter, Y_RESOLUTION+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      assert(err == MPI_SUCCESS);
    }
}

//==============================================================================
/* does a block distribution from start-end to p processors returning in
   local_start / local_end block boundaries for processor iam
 */

static void
block_distribution (int start,	       /* start iteration */
		    int end,	       /* end iteration (incl.) */
		    int p,	       /* number of processors */
		    int iam,	       /* my processor number */
		    int *local_start,  /* local start iteration */
		    int *local_end     /* local end iteration (incl.) */
	)
{
  int n = end - start + 1;
  int q = n / p;
  int r = n - (q * p);

  if (r == 0)
    {
      *local_start = iam * q;
      *local_end = (iam + 1) * q - 1;
    }
  else
    {
      if (iam < r)
	{
	  *local_start = iam * (q + 1);
	  *local_end = *local_start + (q + 1) - 1;
	}
      else
	{
	  *local_start = (r * (q + 1)) + (iam - r) * q;
	  *local_end = *local_start + q - 1;
	}
    }

#if (DEBUG > 0)
  printf ("p=%d, n=%d, q=%d, r=%d, start=%d, end=%d, localstart=%d, localend=%d\n",
	  p, n, q, r, start, end, *local_start, *local_end);
#endif
}

static void mandelbrot_simulate(int maxiter, double dx, double dy, double xmin, double ymin, idx_t task_times[X_RESOLUTION]) {
	/*int err;
	if(rank == 0) {
		MPI_Request requests[X_RESOLUTION];
		for(int i = 0; i < X_RESOLUTION; i++) {
			MPI_Irecv(&task_times[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &requests[i]);
		}
		MPI_Status status;
		for(int i = 0; i < size - 1; i++) {
			err = MPI_Wait(&requests[i], &status);
			assert(err == MPI_SUCCESS);
		}
	} else {
		int start_iter, end_iter;
		block_distribution(0, X_RESOLUTION - 1, size - 1, rank - 1, &start_iter, &end_iter);
		MPI_Request requests[end_iter - start_iter + 1];
		*/
		// calculate values for every point in complex plane
		for(int i = 0; i < X_RESOLUTION; i++) {
			// measure row computation time, i.e. execution time for one single task
			double t_task = gettime();

			for(int j = 0; j < Y_RESOLUTION; j++) {
				int k;
				double absvalue, temp;
				struct {
					double real, imag;
				} z, c;

				// map point to window
				c.real = xmin + i * dx;
				c.imag = ymin + j * dy;
				z.real = z.imag = 0.0;
				k = 0;

				do {
					temp = z.real * z.real - z.imag * z.imag + c.real;
					z.imag = 2.0 * z.real * z.imag + c.imag;
					z.real = temp;
					absvalue = z.real * z.real + z.imag * z.imag;
					k++;
				} while(absvalue < 4.0 && k < maxiter);
			}

			// task time
			t_task = gettime() - t_task;

			fprintf(stderr, "%f ", t_task);

			task_times[i] = (int)ceil(task_times[i] * 100000);

			fprintf(stderr, "%i ", task_times[i]);

			//MPI_Isend(&task_times[i], 1, MPI_INT, 0, i, MPI_COMM_WORLD, &requests[i - start_iter]);
		}
		/*MPI_Status status;
		for(int i = 0; i <= end_iter - start_iter; i++) {
			err = MPI_Wait(&requests[i], &status);
			assert(err == MPI_SUCCESS);
		}*/
	//}
}

static void graph_distribution(int numprocs, idx_t vwgt[X_RESOLUTION], idx_t part[X_RESOLUTION]) {
	idx_t n_vertex = X_RESOLUTION;
	idx_t n_edge = X_RESOLUTION - 1;
	idx_t adjwgt[n_edge*2];
	for(int i = 0; i < n_edge*2; i++) {
		adjwgt[i] = 1;
	}

	idx_t xadj[n_vertex+1];
	xadj[0] = 0;
	xadj[1] = 1;
	for(int i = 2; i < n_vertex - 1; i++) {
		xadj[i] = xadj[i-1] + 2; // TODO remove arr depend?
	}
	xadj[n_vertex - 1] = 2*n_edge - 1;
	xadj[n_vertex] = 2*n_edge;
	
	idx_t adjncy[2*n_edge]; // = { 1,   0,2,  1,3,  2,4, ...,   n_vertex-3,n_vertex-1,    n_vertex-2}
	adjncy[0] = 1;
	for(int i = 1; i < n_vertex - 1; i++) {
		int idx = 1 + 2 * (i - 1); // TODO check
		adjncy[idx] = i - 1;
		adjncy[idx+1] = i + 1;
	}
	adjncy[2*n_edge - 1] = n_vertex - 2;

	// Metis options (initialized to default values)
	idx_t options[METIS_NOPTIONS];
	METIS_SetDefaultOptions(options);
	// partitioning method: k-way partitioning
	options[METIS_OPTION_PTYPE] = METIS_PTYPE_KWAY;
	// edge cut minimization
	options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
	// C-style numbering
	options[METIS_OPTION_NUMBERING] = 0;

	// number of balancing constraints
	idx_t ncon = 1;

	// edge cut after partitioning
	idx_t edgecut;

	idx_t nparts = numprocs - 1;

	//print graph
	printf("graph (%ld vertices, %ld edges):\n", (long)n_vertex, (long)xadj[n_vertex] / 2);

	// print vertices with adjacency list
	printf("%6s (%10s) : %s\n", "vertex", "weight", "neighbors(edge weight)");
	for(idx_t vertex = 0; vertex < n_vertex; vertex++) {
		// print vertex number and vertex weight
		printf("%6ld (%10ld) : ", (long)vertex, (long)vwgt[vertex]);

		// adjacent vertices. print vertex number and edge weight
		idx_t start_edge = xadj[vertex];
		idx_t end_edge = xadj[vertex + 1] - 1;
		for(idx_t neighbor = start_edge; neighbor <= end_edge; neighbor++)
			printf("%4ld(%ld) ", (long)adjncy[neighbor], (long)adjwgt[neighbor]);
		printf("\n");
	}
	
	int rc = METIS_PartGraphKway(
	    &n_vertex,		// number of vertices
		&ncon,          // number of balancing constraints
		xadj,           // adjacency structure of graph
		adjncy,	        // adjacency structure of graph
		vwgt,           // vertex weights
		NULL,           // only for total communication volume
		adjwgt,	        // edge weights
		&nparts,		// number of partitions wanted
		NULL,           // tpwgts: no desired partition weights
		NULL,           // ubvec: allowed load imbalance
		options,		// special options
		&edgecut,		// objective value (edge cut)
		part 			// vector with partition information for each vertex
	);

	// check Metis return value
	switch (rc) {
		case METIS_OK:
			fprintf(stderr, "DEBUG: METIS_OK\n");
			break;
		case METIS_ERROR_INPUT:
			fprintf(stderr, "error in Metis input\n");
			exit(1);
		case METIS_ERROR_MEMORY:
			fprintf(stderr, "no more memory in Metis\n");
			exit(1);
		case METIS_ERROR:
			fprintf(stderr, "some error in Metis\n");
			exit(1);
		default:
			fprintf(stderr, "unknown return code ffrom Metis\n");
			exit(1);
	}
}

//==============================================================================
// mandelbrot computation (on clients)

static void mandelbrot_client(int maxiter, double dx, double dy, double xmin, double ymin, idx_t part[X_RESOLUTION]) {
	// calculate values for every point in complex plane
	for (int i = 0; i < X_RESOLUTION; i++) {
		if(part[i] == rank - 1) { // rank starts with 1 due to 0 being master, but metis starts with 0, since it doesn't schedule master
			#if (DEBUG > 1)
				printf("process %d starts working on row %d\n", rank, i);
			#endif
	  
			for(int j = 0; j < Y_RESOLUTION; j++) {
				int k;
				double absvalue, temp;
				struct {
					double real, imag;
				} z, c;

				// map point to window
				c.real = xmin + i * dx;
				c.imag = ymin + j * dy;
				z.real = z.imag = 0.0;
				k = 0;
		  
				do {
					temp = z.real * z.real - z.imag * z.imag + c.real;
					z.imag = 2.0 * z.real * z.imag + c.imag;
					z.real = temp;
					absvalue = z.real * z.real + z.imag * z.imag;
					k++;
				} while(absvalue < 4.0 && k < maxiter);
		  
				// display result (in our case just add to checksum)
				drawPoint (i, j, k);
			}
		}
	}
}


//==============================================================================
/* mandelbrot computation */

static void mandelbrot(int maxiter, double dx, double dy, double xmin, double ymin, idx_t part[X_RESOLUTION]) {
	if (rank == 0) {
	// master processor waits for data from slave processors
	for (int i = 0; i < X_RESOLUTION; i++)
		receive_data ();
	} else {
		// all clients work on mandelbrot computations and send results to master
		mandelbrot_client(maxiter, dx, dy, xmin, ymin, part);
	}
}


//==============================================================================
// main program

int main (int argc, char **argv) {
	int maxiter, err;
	double xmin, ymin, xmax, ymax;
	double dx, dy;
	double t_start, t_end;

	idx_t part[X_RESOLUTION];

	// initialize MPI
	err = MPI_Init (&argc, &argv);
	assert(err == MPI_SUCCESS);
	err = MPI_Comm_size (MPI_COMM_WORLD, &size);
	assert(err == MPI_SUCCESS);
	err = MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	assert(err == MPI_SUCCESS);

	// initialization of mandelbrot variables
	xmin = -1.5;
	ymin = -1.5;
	xmax = 1.5;
	ymax = 1.5;
	maxiter = 100000;

	// init display variables
	dx = (xmax - xmin) / X_RESOLUTION;
	dy = (ymax - ymin) / Y_RESOLUTION;

	MPI_Status status;

	if(rank == 0) {
		idx_t task_times[X_RESOLUTION];
	//if(rank == 0) {
		t_start = gettime();
		fprintf(stderr, "Starting simulation\n");
	//}
		mandelbrot_simulate(maxiter, dx, dy, xmin, ymin, task_times);
	//if(rank == 0) {
		t_end = gettime();
		fprintf(stderr, "Simulation finished in %.2f s\n", t_end - t_start);
	//}

	// Scheduling
		for(int i = 0; i < X_RESOLUTION; i++) {
			fprintf(stderr, "%i ", task_times[i]);
		}
		fprintf(stderr, "\n");
	
	//if(rank == 0) {
		t_start = gettime();
		fprintf(stderr, "Reached distrtibute mpi_size: %i \n", size);
		graph_distribution(size, task_times, part);
		t_end = gettime();
		fprintf(stderr, "Finished distribute in %.2f s, sending results \n", t_end - t_start);
		t_start = gettime();
		MPI_Request async[size - 1];
		for(int i = 1; i < size; i++) {
			MPI_Isend(part, X_RESOLUTION, MPI_INT, i, 42, MPI_COMM_WORLD, &async[i-1]);
		}
		fprintf(stderr, "Waiting for sending to be completed \n");
		for(int i = 0; i < size - 1; i++) {
			err = MPI_Wait(&async[i], &status);
			assert(err == MPI_SUCCESS);
		}
		t_end = gettime();
		fprintf(stderr, "Sending completed in %.2f s\n", t_end - t_start);
	} else {
		t_start = gettime();
		err = MPI_Recv(part, X_RESOLUTION, MPI_INT, 0, 42, MPI_COMM_WORLD, &status);
		assert(err == MPI_SUCCESS);
		t_end = gettime();
		fprintf(stderr, "%i received distribution after %.2f s waiting \n", rank, t_end - t_start);
	}
	// END
	
	//--------------------------------------------------------------------------
	// mandelbrot computation

	// get start time
	t_start = gettime();

	mandelbrot(maxiter, dx, dy, xmin, ymin, part);

	// get end time
	t_end = gettime();

	//--------------------------------------------------------------------------

	if(rank == 0) {
		printf ("calculation took %.2f s on %d+1 processors\n", t_end - t_start, size-1);
		printf("checksum = %lu\n", checksum);
	}

	// exit MPI
	err = MPI_Finalize ();
	assert(err == MPI_SUCCESS);

	return EXIT_SUCCESS;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
