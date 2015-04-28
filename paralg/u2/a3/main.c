/* 
 * File:   main.c
 * Author: Patrick Robinson
 *
 * Created on 28. April 2015, 12:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>
#include <assert.h>
#include "prefix.h"

static atype_t sum(atype_t x, atype_t y) {
    return x + y;
}

atype_t* values = NULL;

void test_seq(unsigned int n) {
    if(values) free(values);
    values = malloc(sizeof(atype_t) * n);
    for(unsigned int i = 0; i < n; i++) {
        values[i] = i;
    }
    double t0 = gettime();
    for(unsigned int i = 1; i < n; i++) {
        values[i] = sum(values[i-1], values[i]);
    }
    t0 = gettime() - t0;
    printf("seq: n: %u t: %e\n",n, t0);
}

void test_par(unsigned int n, unsigned int p) {
    atype_t par_values[n];
    for(unsigned int i = 0; i < n; i++) {
        par_values[i] = i;
    }
    double t0 = gettime();
    prefix(p, n, par_values, &sum);
    t0 = gettime() - t0;
    printf("par: n: %u p: %u t: %e\n", n, p, t0);
    for(unsigned int i = 0; i < n; i++) {
        assert(par_values[i] == values[i]);
    }
}

int main(int argc, char** argv) {
    unsigned int ns[] = {1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456};
    unsigned int ps[] = {1, 2, 4, 8, 16, 32, 38, 56};
    for(unsigned int i = 0; i < 9; i++) {
        test_seq(ns[i]);
        for(unsigned int j = 0; j < 8; j++) {
            test_par(ns[i], ps[j]);
        }
    }
    
    return (EXIT_SUCCESS);
}

