/*==============================================================================
  
   Purpose          : Floyd's Algorithmus for all pair shortest path (MPI)
                      based on Quinn
   Author           : Rudolf Berrendorf
                      Computer Science Department
                      Bonn-Rhein-Sieg University
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@h-brs.de
  
 =============================================================================*/

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

#include <libFHBRS.h>


#define UL INT_MAX		         // int unlimited
#define min(x,y) (((x)<(y)) ? (x) : (y)) // minimum function


// macros for mapping a matrix to n partial distributed matrices

// size of a row block
#define BLOCKSIZE(nproc,n) ((n) / (nproc))

// owner of a row block
#define BLOCKOWNER(nproc,n,k) ((k) / BLOCKSIZE(nproc,n))

// global start index for a row onm processor me
#define BLOCKLOW(nproc,me,n) ((me) * (BLOCKSIZE(nproc,n)))


/*----------------------------------------------------------------------------*/

static void
floyd (int m, int n, double a[m][n])
{
  double tmp[n];
  int me, nproc, res;

  // we need our processor number and the total number of processors
  res = MPI_Comm_rank (MPI_COMM_WORLD, &me);
  assert (res == MPI_SUCCESS);
  res = MPI_Comm_size (MPI_COMM_WORLD, &nproc);
  assert (res == MPI_SUCCESS);

  for (int k = 0; k < n; k++)
    {
      // who is owner of k-th block?
      int root = BLOCKOWNER (nproc, n, k);

      if (root == me)
	{
	  /* determine global start index for the rows of this processor
	     and therefore from the local index k a global offset
	  */
	  int offset = k - BLOCKLOW (nproc, me, n);

	  // gather k-th column
	  for (int j = 0; j < n; j++)
	    tmp[j] = a[offset][j];
	}

      // send column to all processors
      res = MPI_Bcast (tmp, n, MPI_DOUBLE, root, MPI_COMM_WORLD);
      assert (res == MPI_SUCCESS);

      for (int i = 0; i < BLOCKSIZE (nproc, n); i++)
	for (int j = 0; j < n; j++)
	  // here we access this column through tmp
	  a[i][j] = min (a[i][j], a[i][k] + tmp[j]);
    }
}

/*----------------------------------------------------------------------------*/
// generate test matrix

static void
generate_matrix (int n_local, int n, double a[n_local][n], int me)
{
  for (int i = 0; i < n_local; i++)
    {
      int i_global = me * n_local + i;
      // initialize random number generator
      srand (i_global);

      for (int j = 0; j < n; j++)
	{
	  double weight = rand () / (double) RAND_MAX;
	  // with a propability of 0.5 there is an edge
	  if (weight > 0.5)
	    // no edge
	    weight = UL;
	  else
	    weight *= 100;
	  a[i][j] = weight;
	}

      // matrix diagonal
      a[i][i_global] = 0.0;
    }
}

/*----------------------------------------------------------------------------*/
// distribute matrix rowwise to processors

#if defined(TEST)

static void
distribute_matrix (int n, double a[n][n], int nproc, int me, int *nlocal)
{
  MPI_Status status;
  int count, row, res;


  // to simplify life in this version nproc must be a divisor of n
  assert ((n % nproc) == 0);

  // number of rows per processor
  *nlocal = BLOCKSIZE (nproc, n);

  // nlocal rows with n elements for each processor
  count = n * (*nlocal);

  if (me == 0)
    {
      for (int p = 1; p < nproc; p++)
	{
	  row = p * (*nlocal);
	  res = MPI_Send (&(a[row][0]), count, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
	  assert (res == MPI_SUCCESS);
	}
    }
  else
    {
      // receive assigned rows in first part of original matrix
      res = MPI_Recv (a, count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
      assert (res == MPI_SUCCESS);
    }
}

/*----------------------------------------------------------------------------*/
// gather parts of matrix rowwise from processors

static void
gather_matrix (int n, double a[n][n], int nproc, int me, int *nlocal)
{
  MPI_Status status;
  int count, row, res;


  // to simplify life in this version nproc must be a divisor of n
  assert ((n % nproc) == 0);

  // number of rows per processor
  *nlocal = BLOCKSIZE (nproc, n);

  // nlocal rows with n elements for each processor
  count = n * (*nlocal);

  if (me == 0)
    {
      for (int p = 1; p < nproc; p++)
	{
	  row = p * (*nlocal);
	  res = MPI_Recv (&(a[row][0]), count, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &status);
	  assert (res == MPI_SUCCESS);
	}
    }
  else
    {
      // receive assigned rows in first part of original matrix
      res = MPI_Send (a, count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      assert (res == MPI_SUCCESS);
    }
}

/*----------------------------------------------------------------------------*/

static void
print_matrix (int n, double a[n][n])
{
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
	printf (" %3d", (int) ((a[i][j] == UL) ? -1 : a[i][j]));
      printf ("\n");
    }
}

#endif // defined(TEST)

/*----------------------------------------------------------------------------*/

int
main (int argc, char **argv)
{
  int me, nproc, n_local, res;

  res = MPI_Init (&argc, &argv);
  assert (res == MPI_SUCCESS);
  res = MPI_Comm_rank (MPI_COMM_WORLD, &me);
  assert (res == MPI_SUCCESS);
  res = MPI_Comm_size (MPI_COMM_WORLD, &nproc);
  assert (res == MPI_SUCCESS);

  assert(argc > 1);
  int n = atoi (argv[1]);
  n_local = BLOCKSIZE(nproc,n);

  // distribute n
  //MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // allocate matrix (we need to know n for that)
  double *a = malloc(n_local*n*sizeof(*a));
  assert(a != NULL);
  // generate matrix (redundantly!)
  generate_matrix (n_local, n, (double (*)[n])a, me);

  // apply algorithm and print results
  res = MPI_Barrier(MPI_COMM_WORLD);
  assert(res == MPI_SUCCESS);
  double t0 = gettime ();

  floyd (n_local, n, (double (*)[n])a);

  res = MPI_Barrier(MPI_COMM_WORLD);
  assert(res == MPI_SUCCESS);
  t0 = gettime () - t0;
  if(me == 0)
    printf ("time for Floyd on %d processors with n=%d: %9.6f\n", nproc, n, t0);

  //gather_matrix (n, a, nproc, me, &n_local);

  res = MPI_Finalize ();
  assert (res == MPI_SUCCESS);

  return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
