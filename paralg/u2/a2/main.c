/* 
 * File:   main.c
 * Author: Patrick Robinson
 *
 * Created on 28. April 2015, 10:37
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libFHBRS.h>
#include "tree.h"

static atype_t sum(atype_t x, atype_t y) {
    return x + y;
}

atype_t test_seq(unsigned int n) {
    atype_t values[n];
    for(unsigned int i = 0; i < n; i++) {
        values[i] = i + 1;
    }
    double t0 = gettime();
    atype_t result = sum(values[0], values[1]);
    for(unsigned int i = 2; i < n; i++) {
        result = sum(result, values[i]);
    }
    t0 = gettime() - t0;
    printf("seq: n: %u t: %e\n", n, t0);
    return result;
}

atype_t test_par(unsigned int n, unsigned int p) {
    unsigned int n2 = n - 1;
    unsigned int n1 = n + n2;
    atype_t values[n1];
    for(unsigned int i = 0; i < n; i++) {
        values[n2 - 1 + i] = i + 1;
    }
    double t0 = gettime();
    atype_t result = balanced_tree(p, n1, values, &sum);
    t0 = gettime() - t0;
    printf("par: n: %u p: %u t: %e\n",n, p, t0);
    return result;
}

int main(int argc, char** argv) {
    unsigned int ns[] = {1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456};
    unsigned int ps[] = {1, 2, 4, 8, 16, 32};
    for(unsigned int i = 0; i < 9; i++) {
        atype_t seq_res = test_seq(ns[i]);
        assert(seq_res == ns[i] * (ns[i] + 1) / 2);
        for(unsigned int j = 0; j < 6; j++) {
            atype_t par_res = test_par(ns[i], ps[j]);
            assert(par_res == seq_res);
        }
    }
    
    
    return (EXIT_SUCCESS);
}

