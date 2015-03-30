/*==============================================================================
  
   Purpose          : 2D least square fit to straight line y = a*x+b
   Author           : Rudolf Berrendorf
                      Department of Computer Science
                      University of Applied Sciences Bonn-Rhein-Sieg
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@fh-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <assert.h>


//==============================================================================
// linear least square fit

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

//==============================================================================

int main(int argc, char **argv)
{
  double alpha, beta;
  double x[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  double y[] = { 5, 10, 8, 9, 8, 12, 6, 8, 9, 13 };
  int n = sizeof (x) / sizeof (double);;

  leastSquareFit(n, x, y, &alpha, &beta);

  printf("alpha=%.2f, beta=%.2f\n", alpha, beta);

  return 0;
}


//==============================================================================
