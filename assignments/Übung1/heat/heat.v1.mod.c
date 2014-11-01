#ifdef _POMP
#  undef _POMP
#endif
#define _POMP 200110

#include "heat.v1.c.opari.inc"
#line 1 "heat.v1.c"
/*==============================================================================
  
   Purpose:    heat distribution
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de

==============================================================================*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include <libFHBRS.h>


/*============================================================================*/
// defines and global variables

#define DISPLAY_SIZE  1024	                             
#define HEAT_MIN 20.0		                            
#define HEAT_MAX 100.0		                            

// display graphically?
static int graphics = 0;
static int window_number = -1;

/*============================================================================*/
// initialize h-array
static void init_data(int xsize, int ysize, double h[xsize + 2][ysize + 2]) {
	// initialize data
POMP_For_enter(&omp_rd_1);
#line 35 "heat.v1.c"
	#pragma omp for nowait
	for(int i = 0; i < xsize + 2; i++)
		for(int j = 0; j < ysize + 2; j++)
			h[i][j] = HEAT_MIN;
POMP_Barrier_enter(&omp_rd_1);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_1);
POMP_For_exit(&omp_rd_1);
#line 39 "heat.v1.c"
}

/*============================================================================*/
// permanent heat source
static void heat_source(int xsize, int ysize, double h[xsize + 2][ysize + 2]) {
	int sizex, sizey;

	// position 4 permanent heat sources
	sizex = xsize / 4;
	sizey = ysize / 4;

	// time-invariant constant values
POMP_For_enter(&omp_rd_2);
#line 51 "heat.v1.c"
	#pragma omp for nowait
	for(int i = 0; i < sizex; i++)
		for(int j = 0; j < sizey; j++) {
			h[1 * xsize / 4 - (sizex / 2) + i][1 * ysize / 4 - (sizey / 2) + j] = HEAT_MAX;
			h[1 * xsize / 4 - (sizex / 2) + i][3 * ysize / 4 - (sizey / 2) + j] = HEAT_MAX;
			h[3 * xsize / 4 - (sizex / 2) + i][1 * ysize / 4 - (sizey / 2) + j] = HEAT_MAX;
			h[3 * xsize / 4 - (sizex / 2) + i][3 * ysize / 4 - (sizey / 2) + j] = HEAT_MAX;
		}
POMP_Barrier_enter(&omp_rd_2);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_2);
POMP_For_exit(&omp_rd_2);
#line 59 "heat.v1.c"
}

/*============================================================================*/
// finite difference method (Laplace)
static void update(int xsize, int ysize, double h[xsize + 2][ysize + 2], double h_new[xsize + 2][ysize + 2]) {
	// calculate new value based upon neighbor values
POMP_For_enter(&omp_rd_3);
#line 65 "heat.v1.c"
	#pragma omp for nowait
	for(int i = 1; i <= xsize; i++)
		for(int j = 1; j <= ysize; j++)
			h_new[i][j] = 0.25 * (h[i - 1][j] + h[i + 1][j] + h[i][j - 1] + h[i][j + 1]);
POMP_Barrier_enter(&omp_rd_3);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_3);
POMP_For_exit(&omp_rd_3);
#line 69 "heat.v1.c"

	// update array with new values
POMP_For_enter(&omp_rd_4);
#line 71 "heat.v1.c"
	#pragma omp for nowait
	for(int i = 1; i <= xsize; i++)
		for(int j = 1; j <= ysize; j++)
			h[i][j] = h_new[i][j];
POMP_Barrier_enter(&omp_rd_4);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_4);
POMP_For_exit(&omp_rd_4);
#line 75 "heat.v1.c"
}


/*============================================================================*/
// display current heat distribution
static void display(int xsize, int ysize, double h[xsize + 2][ysize + 2]) {
POMP_Parallel_fork(&omp_rd_5);
#line 81 "heat.v1.c"
	#pragma omp parallel    
{ POMP_Parallel_begin(&omp_rd_5);
POMP_For_enter(&omp_rd_5);
#line 81 "heat.v1.c"
 #pragma omp          for nowait
	for(int i = 1; i <= xsize; i++)
		for(int j = 1; j <= ysize; j++) {
			unsigned int icolor;
			double color;

			color = (h[i][j] - HEAT_MIN) / (HEAT_MAX - HEAT_MIN);

			if(color < 0.0)
				color = 0.0;
			if(color > 1.0)
				color = 1.0;

			if(graphics) {
POMP_Critical_enter(&omp_rd_6);
#line 95 "heat.v1.c"
				#pragma omp critical(draw)
{ POMP_Critical_begin(&omp_rd_6);
#line 96 "heat.v1.c"
				{
					graphic_setRainbowColor(window_number, (double) color);
					graphic_drawPoint(window_number, i, j);
				}
POMP_Critical_end(&omp_rd_6); }
POMP_Critical_exit(&omp_rd_6);
#line 100 "heat.v1.c"
			}
		}
POMP_Barrier_enter(&omp_rd_5);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_5);
POMP_For_exit(&omp_rd_5);
POMP_Parallel_end(&omp_rd_5); }
POMP_Parallel_join(&omp_rd_5);
#line 102 "heat.v1.c"

	// display columnwise
	if(graphics)
		graphic_flush(window_number);
}


/*============================================================================*/
// calculate checksum
static unsigned long checksum(int xsize, int ysize, double h[xsize + 2][ysize + 2]) {
	unsigned long checksum = 0;

POMP_Parallel_fork(&omp_rd_7);
#line 114 "heat.v1.c"
	#pragma omp parallel     reduction(+:checksum)
{ POMP_Parallel_begin(&omp_rd_7);
POMP_For_enter(&omp_rd_7);
#line 114 "heat.v1.c"
 #pragma omp          for                       nowait
	for(int i = 0; i < xsize + 2; i++)
		for(int j = 0; j < ysize + 2; j++)
			checksum += (unsigned long)h[i][j];
POMP_Barrier_enter(&omp_rd_7);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_7);
POMP_For_exit(&omp_rd_7);
POMP_Parallel_end(&omp_rd_7); }
POMP_Parallel_join(&omp_rd_7);
#line 118 "heat.v1.c"

	return checksum;
}

/*============================================================================*/
static void usage(char *progname) {
	printf("usage: %s size niter graphics\n", progname);
	printf("where\tsize mesh size in each dimension (e.g. 2000)\n");
	printf("\tniter number of iterations (e.g. 1000)\n");
	printf("\tdisplay graphically every x steps (e.g. 100). 0 means no graphic\n");
	exit (EXIT_FAILURE);
}


/*============================================================================*/
int main(int argc, char **argv) {
	int niter, size;
	double t0, t1, ttotal;


	/*--------------------------------------------------------------------------*/
	// check optional runtime parameters

	if(argc == 4) {
		if(sscanf(argv[1], "%d", &size) != 1)
			usage(argv[0]);
		if(sscanf(argv[2], "%d", &niter) != 1)
			usage(argv[0]);
		if(sscanf(argv[3], "%d", &graphics) != 1)
			usage(argv[0]);
	} else {
		// error
		usage(argv[0]);
	}

	/*--------------------------------------------------------------------------*/
	// start graphics
	if(graphics) {
		if((window_number = graphic_start (DISPLAY_SIZE, DISPLAY_SIZE, "Heat Distribution")) < 0)
			return EXIT_FAILURE;
		graphic_userCoordinateSystem (window_number, 0.0, 0.0, size, size);
	}

	// finite difference array with additional 2 boundary cells to simplify calculation
	double *h_buf = malloc ((size + 2) * (size + 2) * sizeof (*h_buf));
	double *h_new_buf = malloc ((size + 2) * (size + 2) * sizeof (*h_new_buf));
	if((h_buf == NULL) || (h_new_buf == NULL)) {
		// error
		printf ("no more memory\n");
		exit (EXIT_FAILURE);
	}

	int xsize = size;
	int ysize = size;

	// for better reading/casting define an array type and appropriate variables
	typedef double array_t[ysize + 2];
	array_t *h = (array_t *) h_buf;
	array_t *h_new = (array_t *) h_new_buf;

POMP_Parallel_fork(&omp_rd_8);
#line 178 "heat.v1.c"
	#pragma omp parallel POMP_DLIST_00008
{ POMP_Parallel_begin(&omp_rd_8);
#line 179 "heat.v1.c"
	{
		// initialize data
		init_data(xsize, ysize, h);

		// add heat sources
		heat_source(xsize, ysize, h);
	}
POMP_Barrier_enter(&omp_rd_8);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_8);
POMP_Parallel_end(&omp_rd_8); }
POMP_Parallel_join(&omp_rd_8);
#line 186 "heat.v1.c"

	/*--------------------------------------------------------------------------*/
	// iteration steps
	t0 = ttotal = gettime ();
	if(graphics)
		display (xsize, ysize, h);

	for(int t = 0; t < niter; t++) {
		// display every niter step
		if((graphics > 0) && (t % graphics == (graphics - 1))) {
			t1 = gettime();
			display(xsize, ysize, h);
			printf("time step %8d of %8d: %8.6f s per time step\n", (t + 1), niter, (t1 - t0) / graphics);
			t0 = gettime();
		}
POMP_Parallel_fork(&omp_rd_9);
#line 201 "heat.v1.c"
		#pragma omp parallel POMP_DLIST_00009
{ POMP_Parallel_begin(&omp_rd_9);
#line 202 "heat.v1.c"
		{
			// compute new values
			update(xsize, ysize, h, h_new);

			// permanent heat source
			heat_source(xsize, ysize, h);
		}
POMP_Barrier_enter(&omp_rd_9);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_9);
POMP_Parallel_end(&omp_rd_9); }
POMP_Parallel_join(&omp_rd_9);
#line 209 "heat.v1.c"
	}

	/*--------------------------------------------------------------------------*/

	ttotal = gettime() - ttotal;
	printf("total time: %.6f\n", ttotal);
	printf("checksum  : %lu\n", checksum(xsize, ysize, h));

	// finish graphics
	if(graphics)
		graphic_end(window_number);

	free(h);
	free(h_new);

	return EXIT_SUCCESS;
}
