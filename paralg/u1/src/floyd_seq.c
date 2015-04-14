/*==============================================================================
  
   Purpose          : Floyd's Algorithmus for all pair shortest path
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
#include <unistd.h>

#include <libFHBRS.h>

#define UL INT_MAX		         // int unlimited
#define min(x,y) (((x)<(y)) ? (x) : (y)) // minimum function


/*----------------------------------------------------------------------------*/
// Floyd's algorithm

static void
floyd (int n, double a[n][n])
{
  for (int k = 0; k < n; k++)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
	a[i][j] = min (a[i][j], a[i][k] + a[k][j]);
}

/*----------------------------------------------------------------------------*/
// generate test matrix

#if !defined(TEST)

static void
generate_matrix (int n, double a[n][n])
{
  for (int i = 0; i < n; i++)
    {
      // initialize random number generator
      srand (i);
      for (int j = 0; j < n; j++)
	{
	  double weight = rand () / (double) RAND_MAX;
	  // with a propability of 0.5 there is an edge
	  if (weight > 0.5)
	    // no edge (int unlimited)
	    weight = UL;
	  else
	    weight *= 100;
	  a[i][j] = weight;
	}

      // matrix diagonal
      a[i][i] = 0.0;
    }
}

#endif

/*----------------------------------------------------------------------------*/
// print matrix

#if defined(TEST)

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

#endif

/*----------------------------------------------------------------------------*/

int
main (int argc, char **argv)
{
#if defined(TEST)
  // small example matrix
#define N 6
  int n = N;
  double a[N][N] = {
    {0, 2, 5, UL, UL, UL},
    {UL, 0, 7, 1, UL, 8},
    {UL, UL, 0, 4, UL, UL},
    {UL, UL, UL, 0, 3, UL},
    {UL, UL, 2, UL, 0, 3},
    {UL, 5, UL, 2, 4, 0}
  };
  print_matrix (n, a);
#else
  // real example
  int n = atoi (argv[1]);
  double *a = malloc(n * n * sizeof(*a));
  assert(a != NULL);

  // generate matrix
  generate_matrix (n, (double (*)[n])a);
#endif


  // apply algorithm and print result
  double t0 = gettime ();
  floyd (n, (double (*)[n])a);
  t0 = gettime () - t0;

  printf ("seq time for Floyd with n=%d: %9.6f\n", n, t0);

#if defined(TEST)
  print_matrix (n, a);
#endif

  return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
