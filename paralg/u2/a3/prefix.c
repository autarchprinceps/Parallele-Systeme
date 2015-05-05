#include "prefix.h"
#include <stdio.h>
#include <omp.h>

#define MIN(x,y) ((x) > (y)) ? (y) : (x)

void prefix(unsigned int p, unsigned int n, atype_t values[n], atype_t (*f)(atype_t, atype_t)) {
    atype_t last_elems[p];
    last_elems[0] = 0;
    #pragma omp parallel num_threads(p)
    {
        unsigned int rank = omp_get_thread_num(); // [0 .. p-1]
        unsigned int blockstart = ((n % p == 0) ? (n / p) : (n / p + 1)) * rank; 
        unsigned int next_blockstart = MIN(((n % p == 0) ? (n / p) : (n / p + 1)) * (rank + 1), n);
        
        // sequential prefix per block
        for(unsigned int i = blockstart + 1; i < next_blockstart; i++) {
            values[i] = (*f)(values[i-1], values[i]);
        }
        
        // get last block elements
        if(rank < p - 1) {
            last_elems[rank + 1] = values[next_blockstart - 1];
        }
        #pragma omp barrier
        // prefix on last elements
        if(rank == 0) {
            for(unsigned int i = 1; i < p; i++) {
                last_elems[i] = (*f)(last_elems[i-1], last_elems[i]);
            }
        }
        #pragma omp barrier
        if(rank > 0) {
            for(unsigned int i = blockstart; i < next_blockstart; i++) {
                values[i] = (*f)(last_elems[rank], values[i]);
            }
        }
    }
}
