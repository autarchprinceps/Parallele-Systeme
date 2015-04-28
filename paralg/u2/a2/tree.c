#include "tree.h"
#include <assert.h>
#include <omp.h>

atype_t balanced_tree(unsigned int p, unsigned long n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y)) {
    unsigned int n = (n1 + 1) / 2;
    for(unsigned long stride = n/2; stride > 0; stride /= 2) {
        #pragma omp parallel num_threads(p)
        {
            assert(omp_get_num_threads() == p);
            #pragma omp for schedule(guided)
            for(unsigned long i = 0; i < stride; i++) {
                values[stride - 1 + i] = (*f)(values[stride*2-1 + i],values[stride*2-1 + i + stride]);
            }
        }
    }
    return values[0];
}