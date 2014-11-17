/*==============================================================================
  
   Purpose          : matrix multiply
   Author           : Rudolf Berrendorf
                      Computer Science Department
                      Bonn-Rhein-Sieg University of Applied Sciences
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(ACML)
// use DGEMM from ACML
#include <acml.h>         // AMD Core Math Library
#else
// original Fortran interface in BLAS
extern void dgemm_(char *, char *,int *, int *, int *,double *, double *,int *, double *, int*, double *, double *, int *);
// our C interface
#define dgemm(transa,transb,m,n,k,alpha,a,lda,b,ldb,beta,c,ldc) \
        dgemm_(&transa,&transb,&m,&n,&k,&alpha,a,&lda,b,&ldb,&beta,c,&ldc)
#endif

#include <libFHBRS.h>

/*===========================================================================*/

#define N 999             // size of matrix
#define EPS 1e-4          // tolerance on correctness proof


// three matrix sets
static double a1[N][N], b1[N][N], c1[N][N];
static double a2[N][N], b2[N][N], c2[N][N];
static double a3[N][N], b3[N][N], c3[N][N];


/*===========================================================================*/
// apply your optimizations to this version

#define min(x,y) x < y ? x : y
#define B 64

static void matmul(int n, double a[n][n], double b[n][n], double c[n][n]) {
	for(int kk = 0; kk < n; kk += B)
		for(int jj = 0; jj < n; jj += B)
			for(int i = 0; i < n; i++)
				for(int k = kk; k < min(kk+B, n); k++) {
					printf("%i %i %i %i ", kk, jj, i, k);
					double tmp = b[i][k];
					printf("tmp done\n");
					for(int j = jj; j < min(jj+B, n); j++) {
						a[i][j] += tmp * c[k][j];
						/*a[i][j+1] += tmp * c[k][j+1];
						a[i][j+2] += tmp * c[k][j+2];
						a[i][j+3] += tmp * c[k][j+3];
						a[i][j+4] += tmp * c[k][j+4];
						a[i][j+5] += tmp * c[k][j+5];
						a[i][j+6] += tmp * c[k][j+6];
						a[i][j+7] += tmp * c[k][j+7];
						a[i][j+8] += tmp * c[k][j+8];*/
						printf(".");
					}
					printf("\n");
				}
}


/*===========================================================================*/
// original version

static void matmul_original(int n, double a[n][n], double b[n][n], double c[n][n])
{
  for(int j=0; j<n; j++)
    for(int k=0; k<n; k++)
      for(int i=0; i<n; i++)
	a[i][j] += b[i][k] * c[k][j];
}


/*===========================================================================*/
// highly optimized version

static void matmul_optimized(int n, double a[n][n], double b[n][n], double c[n][n])
{
  char trans = 'T';
  double one = 1.0;
  double zero = 0.0;

  // call BLAS DGEMM
  dgemm(trans, trans, n, n, n, one, (double *)b, n, (double *)c, n, one, (double *)a, n);
}


/*===========================================================================*/
// initialize matrices

static void matinit(int n, double a[n][n], double b[n][n], double c[n][n])
{
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
      {
	a[i][j] = 0.0;
	b[i][j] = c[i][j] = i + j;
      }
}


/*===========================================================================*/
// check for correctness against original version

static int matcheck(int n, double a1[n][n], double a2[n][n])
{
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
      if(fabs(a1[i][j] - a2[i][j]) > EPS)
	{
	  printf("error at position a[%d][%d]: %f should be %f\n", i, j, a1[i][j], a2[i][j]);
	  return -1;
	}
     
  return 0;
}


/*===========================================================================*/

int main(int argc, char **argv)
{
  double t1, t2, t3;
  int n = N;


  // start optimized version
  matinit(n, a1, b1, c1);
  t1 = gettime();
  matmul_optimized(n, a1, b1, c1);
  t1 = gettime() - t1;

  printf("%10s: %9.3f s, %9.3f MFLOPS\n", "optimized", t1, 2.0*n*n*n/1e6/t1);


  // start your version
  matinit(n, a2, b2, c2);
  t2 = gettime();
  matmul(n, a2, b2, c2);
  t2 = gettime() - t2;

  // check results for correctness
  if(matcheck(n, a1, a2) == 0)
    printf("%10s: %9.3f s, %9.3f MFLOPS\n", "your", t2, 2.0*n*n*n/1e6/t2);
  else
    printf("incorrect result for your version\n");


  // start original version
  matinit(n, a3, b3, c3);
  t3 = gettime();
  matmul_original(n, a3, b3, c3);
  t3 = gettime() - t3;

  // check results for correctness
  if(matcheck(n, a1, a3) == 0)
    printf("%10s: %9.3f s, %9.3f MFLOPS\n", "original", t3, 2.0*n*n*n/1e6/t3);
  else
    printf("incorrect result for ACML/original version\n");


  return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
