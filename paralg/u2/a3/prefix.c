#include "prefix.h"
#include <omp.h>

void prefix(unsigned int p, unsigned int n, atype_t values[n], atype_t (*f)(atype_t, atype_t)) {
    atype_t last_elems[p-1];
    #pragma omp parallel num_threads(p)
    {
        unsigned int rank;
        unsigned int blockstart = n / p * rank; // TODO does rank start with 0? then correct else - 1;
        unsigned int next_blockstart = n / p * (rank + 1); // TODO was wenn kann nicht ganz aufgeteilt werden
        for(unsigned int i = blockstart + 1; i < next_blockstart; i++) {
            values[i] = (*f)(values[i-1], values[i]);
        }
        #pragma omp barrier
        if(rank == 0) { // or 1 ?
            unsigned int j = 0;
            last_elems[0] = values[n/p - 1];
            for(unsigned int i = n/p * 2 - 1; i < n; i += n/p) { // TODO letzter block nicht?
                last_elems[j] = (*f)(last_elems[j-1], values[i])
                j++;
            }
        }
        #pragma omp barrier
        if(rank > 0) { // or 1 ?
            for(unsigned int i = blockstart + 1; i < next_blockstart; i++) {
                values[i] = (*f)(last_elems[rank - 1], values[i]); // or rank - 2?
            }
        }
    }
}