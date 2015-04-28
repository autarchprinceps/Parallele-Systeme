#include "tree.h"

atype_t balanced_tree(unsigned int p, unsigned int n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y)) {
    unsigned int n = (n1 + 1) / 2;
    for(unsigned int stride = n/2; stride > 0; stride /= 2) {
        #pragma omp parallel num_threads(p)
        {
            #pragma omp for schedule(guided)
            for(unsigned int i = 0; i < stride; i++) {
                values[stride - 1 + i] = (*f)(values[stride*2-1 + i],values[stride*2-1 + i + stride]);
            }
        }
    }
    return values[0];
}