#include "prefix.h"
#include <stdio.h>
#include <omp.h>

#define MIN(x,y) ((x) > (y)) ? (y) : (x)

void prefix(unsigned int p, unsigned int n, atype_t values[n], atype_t (*f)(atype_t, atype_t)) {
    // TODO what if p == 1
    atype_t last_elems[p];
    last_elems[0] = 0;
    #pragma omp parallel num_threads(p)
    {
        unsigned int rank = omp_get_thread_num(); // [0 .. p-1]
        unsigned int blockstart = ((n % p == 0) ? (n / p) : (n / p + 1)) * rank; 
        unsigned int next_blockstart = MIN(((n % p == 0) ? (n / p) : (n / p + 1)) * (rank + 1), n);
        
        /*printf("%u %u %u\n", rank, blockstart, next_blockstart);
        #pragma omp barrier // DEBUG
        if(rank == 0) {
            printf("pre\n");
            for(unsigned int i = 0; i < n; i++) {
                printf("%f ", values[i]);
            }
            printf("\n");
        }
        #pragma omp barrier // DEBUG*/
        
        // sequential prefix per block
        for(unsigned int i = blockstart + 1; i < next_blockstart; i++) {
            values[i] = (*f)(values[i-1], values[i]);
        }
        
        // get last block elements
        if(rank < p - 1) {
            last_elems[rank + 1] = values[next_blockstart - 1];
        }
        #pragma omp barrier
        /*if(rank == 0) {
            printf("seq prefix per block\n");
            for(unsigned int i = 0; i < n; i++) {
                printf("%f ", values[i]);
            }
            printf("\n");
            for(unsigned int i = 0; i < p; i++) {
                printf("%f ", last_elems[i]);
            }
            printf("\n");
        }
        #pragma omp barrier // DEBUG*/
        // prefix on last elements
        if(rank == 0) {
            for(unsigned int i = 1; i < p; i++) {
                last_elems[i] = (*f)(last_elems[i-1], last_elems[i]);
            }
        }
        #pragma omp barrier
        /*if(rank == 0) {
            printf("prefix on last elems\n");
            for(unsigned int i = 0; i < n; i++) {
                printf("%f ", values[i]);
            }
            printf("\n");
            for(unsigned int i = 0; i < p; i++) {
                printf("%f ", last_elems[i]);
            }
            printf("\n");
        }
        #pragma omp barrier // DEBUG*/
        if(rank > 0) {
            for(unsigned int i = blockstart; i < next_blockstart; i++) {
                values[i] = (*f)(last_elems[rank], values[i]);
            }
        }
        /*#pragma omp barrier // DEBUG
        if(rank == 0) {
            printf("final\n");
            for(unsigned int i = 0; i < n; i++) {
                printf("%f ", values[i]);
            }
            printf("\n");
            for(unsigned int i = 0; i < p; i++) {
                printf("%f ", last_elems[i]);
            }
            printf("\n");
        }
        #pragma omp barrier // DEBUG*/
    }
}
