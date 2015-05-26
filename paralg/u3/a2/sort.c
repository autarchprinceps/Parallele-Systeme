/* 
 * File:   main.c
 * Author: Patrick Robinson
 *
 * Created on 18. Mai 2015, 11:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libFHBRS.h>
#include <mpi.h>

typedef int atype_t;

int compare(const void *p, const void *q) {
    atype_t x = *(const atype_t *)p;
    atype_t y = *(const atype_t *)q;
    return (x > y) ? 1 : ((x == y) ? 0 : -1);
}

void test_seq(unsigned int n) {
    int rank, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	if(rank == 0) {
		atype_t* values = malloc(sizeof(atype_t) * n);
		for(unsigned int i = 0; i < n; i++) {
		    values[i] = n - i;
		}
		double t0 = gettime();
		qsort(values, n, sizeof(atype_t), compare);
		t0 = gettime() - t0;
		
		free(values);
	    printf("\nseq: n: %u t: %e\n", n, t0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void swap(atype_t** a, atype_t** b) {
   atype_t* tmp = *a;
   *a = *b;
   *b = tmp;
}

void merge(int lower, unsigned int n, atype_t* a, atype_t* b, atype_t* c) {
	if(lower) {
		unsigned int i = 0, j = 0, k = 0;
		while(k < n) {
			if(a[i] < b[j]) {
				c[k++] = a[i++];
			} else {
				c[k++] = b[j++];
			}
		}
	} else {
		unsigned int i = n - 1;
		unsigned int j = n - 1;
		int k = n - 1;
		while(k >= 0) {
			if(a[i] > b[j]) {
				c[k--] = a[i--];
			} else {
				c[k--] = b[j--];
			}
		}
	}
}

void test_par(unsigned int n) {
    int rank, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Status status;
	// parallel alloc
	size_t ats = sizeof(atype_t);
	unsigned int np = n / p;
	atype_t* values = malloc(ats * np);
	atype_t* rec_values = malloc(ats * np);
	atype_t* new_values = malloc(ats * np);
	unsigned int startidx = rank * np;
	unsigned int nextstartidx = (rank + 1) * np;
	for(unsigned int i = startidx; i < nextstartidx; i++) {
		values[i - startidx] = n - (startidx + i);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	double t0;
	if(rank == 0) {
		t0 = gettime();
	}
	// initial sort
	qsort(values, np, ats, compare);
	// odd even exchange & merge
	for(unsigned int j = 0; j < p; j++) {
		int even = (rank % 2) == (j % 2);
		int other = rank + (even ? 1 : -1);
		if(other < 0 || other >= p) continue;
		MPI_Request request;
		MPI_Isend(values, np, MPI_INT, other, j, MPI_COMM_WORLD, &request);
		MPI_Recv(rec_values, np, MPI_INT, other, j, MPI_COMM_WORLD, &status);
		merge(even, np, values, rec_values, new_values);
		swap(&values, &new_values);
		MPI_Wait(&request, &status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) {
		t0 = gettime() - t0;
		printf("\npar: %u n: %u t: %e\n", p, n, t0);
	}
	// check
	// isSorted(values)
	atype_t old = values[0];
	for(unsigned int i = 0; i < np; i++) {
		if(old > values[i]) {
			fprintf(stderr, "ERROR %d: own values unsorted\n", rank);
		}
		old = values[i];
	}
	// is smaller than all previous maxs:
	// receive maxs & send maxs
	MPI_Request sends[p];
	MPI_Request recvs[p];
	atype_t maxmins[p];
	for(unsigned int ranks = 0; ranks < p; ranks++) {
		if(ranks != rank) {
			if(ranks > rank) {
				MPI_Isend(&old, 1, MPI_INT, ranks, rank, MPI_COMM_WORLD, &sends[ranks]);
			} else {
				MPI_Isend(values, 1, MPI_INT, ranks, rank, MPI_COMM_WORLD, &sends[ranks]);
			}
			MPI_Irecv(&maxmins[ranks], 1, MPI_INT, ranks, ranks, MPI_COMM_WORLD, &recvs[ranks]);
		}
	}
	for(unsigned int ranks = 0; ranks < p; ranks++) {
		if(ranks != rank) {
			MPI_Wait(&recvs[ranks], &status);
			if(ranks < rank) {
				for(unsigned int i = 0; i < np; i++) {
					if(maxmins[ranks] > values[0]) {
						fprintf(stderr, "ERROR %d: maxs[%u] bigger\n", rank, ranks);
					}
				}
			} else {
				for(unsigned int i = 0; i < np; i++) {
					if(maxmins[ranks] < old) {
						fprintf(stderr, "ERROR %d: mins[%u] bigger\n", rank, ranks);
					}
				}
			}
		}
	}
	for(unsigned int ranks = 0; ranks < p; ranks++) {
		if(ranks != rank) MPI_Wait(&sends[ranks], &status);
	}
	
	free(values);
	free(rec_values);
	free(new_values);
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
    unsigned int ns[] = {1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456};

	for(unsigned int i = 0; i < 9; i++) {
		test_seq(ns[i]);
		test_par(ns[i]);
	}   
	
    MPI_Finalize();
    return (EXIT_SUCCESS);
}
