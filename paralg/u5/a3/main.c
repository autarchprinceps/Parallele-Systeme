/* 
 * File:   main.c
 * Author: autarch
 *
 * Created on 19. Juni 2015, 18:36
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef double atype_t;

atype_t seq_midpoint(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = 0;
    for(atype_t i = start; i < end; i += 2 * h) { // TODO Summe bis n/2 ?
        tmp += 2 * h * (*f)(i);
    }
    return tmp;
}

atype_t par_midpoint(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = 0;
    #pragma omp parallel for reduction(+:tmp)
    for(atype_t i = start; i < end; i += 2 * h) { // TODO Summe bis n/2 ?
        tmp += 2 * h * (*f)(i);
    }
    return tmp;
}

atype_t seq_trapezoid(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = 0;
    for(atype_t i = start; i < end; i += h) {
        tmp += 0.5 * h * ((*f)(i) + (*f)(i + h));
    }
    return tmp;
}

atype_t par_trapezoid(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = 0;
    #pragma omp parallel for reduction(+:tmp)
    for(atype_t i = start; i < end; i += h) {
        tmp += 0.5 * h * ((*f)(i) + (*f)(i + h));
    }
    return tmp;
}

atype_t seq_simpson(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = (*f)(start) + (*f)(end);
    atype_t x = start + h;
    for(unsigned int i = 0; i < n - 1; i++) {
        if(i % 2 == 0) {
            tmp += 2 * (*f)(x);
        } else {
            tmp += 4 * (*f)(x);
        }
        x += h;
    }
    return tmp * h / 3;
}

atype_t par_simpson(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    atype_t h = (end - start) / n;
    atype_t tmp = (*f)(start) + (*f)(end);
    #pragma omp parallel for reduction(+:tmp)
    for(unsigned int i = 0; i < n - 1; i++) {
        atype_t x = start + (i + 1) * h;
        if(i % 2 == 0) {
            tmp += 2 * (*f)(x);
        } else {
            tmp += 4 * (*f)(x);
        }
    }
    return tmp * h / 3;
}

atype_t seq_montecarlo(atype_t start, atype_t end, atype_t n, atype_t (*f)(atype_t)) {
    return 0;
}

/*
 * 
 */
int main(int argc, char** argv) {
    atype_t b = 10000;
    atype_t a = -1000;
    atype_t n = 123456789;
    printf("Exact: %e\n", -cos(b) + cos(a)); // F(x) = -cos(x)
    printf("Seq Midpoint: %e\n", seq_midpoint(a, b, n, sin));
    printf("Par Midpoint: %e\n", par_midpoint(a, b, n, sin));
    printf("Seq Trapezoid: %e\n", seq_trapezoid(a, b, n, sin));
    printf("Par Trapezoid: %e\n", par_trapezoid(a, b, n, sin));
    printf("Seq Simpson: %e\n", seq_simpson(a, b, n, sin));
    printf("Par Simpson: %e\n", par_simpson(a, b, n, sin));
    printf("Seq MonteCarlo: %e\n", seq_montecarlo(a, b, n, sin));

    return (EXIT_SUCCESS);
}

