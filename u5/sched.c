/*==============================================================================
  
   Purpose          : loop scheduling algorithms
   Author           : Rudolf Berrendorf
                      Computer Science Department
                      Bonn-Rhein-Sieg University of Applied Sciences
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@h-brs.de
  
==============================================================================*/
	
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#if defined(_OPENMP)
#include <omp.h>
#endif
	
#include "sched.h"
	

/*============================================================================*/ 
/*
  Some remarks:
  -  every scheduling algorithm has an initialization function (xx_setup)
     and a scheduling function (xxx).
     The initialization gets called once, the scheduling function
     possibly several times and for several parallel loops

  -  a scheduling function has the prototype:
     bool xxx (int *start_iteration, int *end_iteration, int n, int p, int iam) 
     where *start_iteration and *end_iteration  must be assigned meaningful
     values for the iteration block assigned to the calling processor
     n is the total number of iteration
     p the number of threads
     iam my thread number between 0 and p-1

  -  a scheduling function is called every first time for one loop scheduling
     with *start_iteration==INVALID_ITERATION. I.e. you can see the start of a
     new loop to be scheduled if *start_iteration==INVALID_ITERATION.
     You may use that e.g. to reset some variables etc.
     See for example sequential_scheduling.

  -  *start_iteration and *end_iteration have the same values when the
     scheduling function is called again (other than the first time as
     described above)

  -  A scheduling function must *assign start_iteration and *end_iteration
     values for the next iteration block to be executed.
     E.g. block scheduling of 100 iterations on 2 processors would return
     0-49 when called by the first processor.

  -  A scheduling function returns true if start_iteration/end_iteration have
     meaningful values, i.e. you have assigned another chunk to that processor.
     The function returns false if no iteration is left over for the calling
     processor.

  -  Loops to be scheduled run always from 0 to some n

  -  There is a static variable current_iteration that you may use for dynamic
     scheduling algorithms e.g. to keep track of the last scheduled iteration

  Example call flow:
    xxx_setup()

    // new parallel loop
    xxx(*start_iteration==INVALID_ITERATION, *end_iteration, ...)      -> true
    xxx(*start_iteration==some_value, *end_iteration==some_value, ...) -> true
    xxx(*start_iteration==some_value, *end_iteration==some_value, ...) -> false

    // new parallel loop
    xxx(*start_iteration==INVALID_ITERATION, *end_iteration, ...)      -> true
    xxx(*start_iteration==some_value, *end_iteration==some_value, ...) -> false
 */

/*============================================================================*/ 
/* helpful macros */ 
	
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))	

/*============================================================================*/ 
/* global variables */ 


/* current iteration (you need that for dynamic algorithms
   to know what was scheduled last) */
static volatile int current_iteration;


/*============================================================================*/ 

static int upper_gauss(int x, int y)
{
  int div = x / y;

  if(div * y == x)
    return div;
  else
    return div + 1;
}

/*============================================================================*/ 
/* sequential (all iterations get assigned to the first calling processor) */ 

void
sequential_scheduling_setup (int n, int p, int iam) 
{
  /* nothing */
}

bool sequential_scheduling (int *start_iteration, int *end_iteration, int n, int p, int iam) 
{
  bool first_time = (*start_iteration == INVALID_ITERATION);

  if (first_time && (iam == 0))
    {
      /* we schedule only once all iterations and only on first processor */ 
      *start_iteration = 0;
      *end_iteration = n-1;
      return true;
    }

  else
    return false;
}


/*============================================================================*/ 
/* static block scheduling */ 

static volatile int block_q;
static volatile int block_r;

void
block_scheduling_setup (int n, int p, int iam) 
{
  /* block factor (iterations from 0 to n-1) */ 
  block_q = (n - 1) / p;
  
  /* number of processors that get blocks of size (q+1) */ 
  block_r = n - (block_q * p);
}

bool block_scheduling (int *start_iteration, int *end_iteration, int n, int p, int iam) 
{
  bool first_time = (*start_iteration == INVALID_ITERATION);

  if (iam <= block_r-1)    
    {
      /* first r processors get larger blocks of size q+1 */ 
      *start_iteration = iam * (block_q + 1);
      *end_iteration = ((iam + 1) * (block_q + 1)) - 1;
    }
  
  else
    {
      /* rest gets smaller blocks of size q */ 
      *start_iteration = block_r * (block_q + 1)
	                 + ((iam - block_r) * block_q);
      *end_iteration = block_r * (block_q + 1)
	                 + ((iam - block_r + 1) * block_q) - 1;
    }

  if (first_time)
    /* we schedule only once */ 
    return true;
  else
    return false;
}


/*============================================================================*/ 
/* static block-cyclic scheduling */ 

static volatile int cyclic_q;

void
blockcyclic_scheduling_setup (int n, int p, int iam) 
{
  /* block factor constantly 1 */ 
  cyclic_q = 1;
}

bool blockcyclic_scheduling (int *start_iteration, int *end_iteration, int n, int p, int iam) 
{  
  if(*start_iteration == INVALID_ITERATION)
    {
      *start_iteration = iam * cyclic_q;
      *end_iteration = min(*start_iteration + cyclic_q - 1, n-1);
    }
  else
    {
      *start_iteration += cyclic_q * p;
      *end_iteration = min(*end_iteration + cyclic_q * p, n-1);
    }

  return *start_iteration < n;
}


/*============================================================================*/ 
/* dynamic self scheduling */ 

void self_scheduling_setup(int n, int p, int iam) {
	current_iteration = 0;
}

bool self_scheduling(int *start_iteration, int *end_iteration, int n, int p, int iam) {
	bool result = false;
	int tmp;
	#pragma omp atomic capture
	tmp = current_iteration++;
	if(tmp < n) {
		*start_iteration = *end_iteration = tmp;
		result = true;
	}
	return result;
}


/*============================================================================*/ 
/* dynamic guided self scheduling */ 
static volatile int remaining_iterations;

void gss_setup(int n, int p, int iam) {
	remaining_iterations = n;
}

bool gss(int *start_iteration, int *end_iteration, int n, int p, int iam) {
	bool result = false;
	#pragma omp critical gss
	{
		if(remaining_iterations > 0) {
			int c = (remaining_iterations + p - 1) / p;
			*start_iteration = n - remaining_iterations;
			*end_iteration = *start_iteration + c - 1;
			remaining_iterations -= c;
			result = true;
		}
	}
	return result;
}


/*============================================================================*/ 
/* dynamic factoring (simplified version 1) */ 
typedef struct {
	int start;
	int end;
} pair;

static volatile pair* sched_list;
static volatile bool running;

void factoring_setup(int n, int p, int iam) {
	current_iteration = 0;
	remaining_iterations = n;
	sched_list = malloc(sizeof(pair)*p);
	int tmp_position = n - remaining_iterations;
	int c = upper_gauss(remaining_iterations, 2 * p);
	remaining_iterations -= p * c;
	for(int j = 0; j < p; j++) {
		sched_list[j].start = tmp_position;
		tmp_position += c;
		sched_list[j].end = tmp_position - 1;
	}
	running = true;
}

bool factoring(int *start_iteration, int *end_iteration, int n, int p, int iam) {
	bool result = false;
	#pragma omp critical fac
	if(running) {
		if(current_iteration < p) {
			*start_iteration = sched_list[current_iteration].start;
			*end_iteration = sched_list[current_iteration].end;
			result = true;
			current_iteration++;
		} else {
			int tmp_position = n - remaining_iterations;
			int c = upper_gauss(remaining_iterations, 2 * p);
			remaining_iterations -= p * c;
			if(remaining_iterations > 0) {
				for(int j = 0; j < p; j++) {
					sched_list[j].start = tmp_position;
					tmp_position += c;
					sched_list[j].end = tmp_position - 1;
				}
				*start_iteration = sched_list[0].start;
				*end_iteration = sched_list[0].end;
				current_iteration = 1;
			} else {
				*start_iteration = tmp_position;
				*end_iteration = n - 1;
				running = false;
			}
			result = true;
		}
	}
	return result;
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/ 
