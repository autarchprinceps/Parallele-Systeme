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
#include <libFHBRS.h>

double seq_midpoint(double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp = 0;
    for(double i = start; i < end; i += 2 * h) {
        tmp += (*f)(i);
    }
    return 2 * h * tmp;
}

double par_midpoint(unsigned int p, double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp = 0;
    #pragma omp parallel for reduction(+:tmp) num_threads(p)
    for(unsigned int i = 0; i < n / 2; i++) {
        double x = start + i * 2 * h;
        tmp += (*f)(x);
    }
    return 2 * h * tmp;
}

double seq_trapezoid(double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp = 0;
    for(double i = start; i < end; i += h) {
        tmp += ((*f)(i) + (*f)(i + h));
    }
    return 0.5 * h * tmp;
}

double par_trapezoid(unsigned int p, double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp = 0;
    #pragma omp parallel for reduction(+:tmp) num_threads(p)
    for(unsigned int i = 0; i < n; i++) {
        double x = start + i * h;
        tmp += ((*f)(x) + (*f)(x + h));
    }
    return 0.5 * h * tmp;
}

double seq_simpson(double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp1 = 0;
    double tmp2 = 0;
    double x = start + h;
    for(unsigned int i = 0; i < n - 1; i++) {
        if(i % 2 == 0) {
            tmp1 += (*f)(x);
        } else {
            tmp2 += (*f)(x);
        }
        x += h;
    }
    return ((*f)(start) + (*f)(end) + 2 * tmp1 + 4 * tmp2) * h / 3;
}

double par_simpson(unsigned int p, double start, double end, unsigned int n, double (*f)(double)) {
    double h = (end - start) / n;
    double tmp1 = 0;
    double tmp2 = 0;
    #pragma omp parallel for reduction(+:tmp1,tmp2) num_threads(p)
    for(unsigned int i = 0; i < n - 1; i++) {
        double x = start + (i+1) * h;
        if(i % 2 == 0) {
            tmp1 += (*f)(x);
        } else {
            tmp2 += (*f)(x);
        }
    }
    return ((*f)(start) + (*f)(end) + 2 * tmp1 + 4 * tmp2) * h / 3;
}

double seq_montecarlo(double start, double end, unsigned int n, double (*f)(double)) {
    void* generator = randp_init(0L);
    double* r = (double *)malloc(n * sizeof(double));
    randp_vector(generator, n, r);
    double tmp = 0;
    for(unsigned int i = 0; i < n; i++) {
        double x = start + r[i] * (end - start);
        tmp += (*f)(x);
    }
    return tmp * (end - start)/n;
}

double par_montecarlo(unsigned int p, double start, double end, unsigned int n, double (*f)(double)) {
    void* generator = randp_init(0L);
    double* r = (double *)malloc(n * sizeof(double));
    randp_vector(generator, n, r);
    double tmp = 0;
    #pragma omp parallel for reduction(+:tmp) num_threads(p)
    for(unsigned int i = 0; i < n; i++) {
        double x = start + r[i] * (end - start);
        tmp += (*f)(x);
    }
    return tmp * (end - start)/n;
}

int main(int argc, char** argv) {
    double b = 1000;
    double a = 0;
    unsigned int ps[] = {1,2,4,8,16,24,32,48,56};
    
    double exactResult = -cos(b) + cos(a); // F(x) = -cos(x)
    printf("exact;%e;;;;;;;;;;;;;;;;;;;\n", exactResult);
    printf("n;seq;;par;;;;;;;;;;;;;;;;;\n");
    printf(";;");
    for(unsigned int pidx = 0; pidx < 9; pidx++) {
        printf(";%u;", ps[pidx]);
    }
    printf("\n");
    for(unsigned int pidx = 0; pidx < 9; pidx++) {
        printf(";time;result");
    }
    printf("\nmidpoint\n");
    for(unsigned int n = 1; n <= 1000000; n *= 10) {
        double t, result;
        t = gettime();
        result = seq_midpoint(a, b, n, sin);
        t = gettime() - t;
        printf("%u;%e;%e", n, t, result);
        for(unsigned int pidx = 0; pidx < 9; pidx++) {
            t = gettime();
            result = par_midpoint(ps[pidx], a, b, n, sin);
            t = gettime() - t;
        }
    }
    printf("\ntrapezoid\n");
    for(unsigned int n = 1; n <= 1000000; n *= 10) {
        double t, result;
        t = gettime();
        result = seq_trapezoid(a, b, n, sin);
        t = gettime() - t;
        printf("%u;%e;%e", n, t, result);
        for(unsigned int pidx = 0; pidx < 9; pidx++) {
            t = gettime();
            result = par_trapezoid(ps[pidx], a, b, n, sin);
            t = gettime() - t;
        }
    }
    printf("\nsimpson\n");
    for(unsigned int n = 1; n <= 1000000; n *= 10) {
        double t, result;
        t = gettime();
        result = seq_simpson(a, b, n, sin);
        t = gettime() - t;
        printf("%u;%e;%e", n, t, result);
        for(unsigned int pidx = 0; pidx < 9; pidx++) {
            t = gettime();
            result = par_simpson(ps[pidx], a, b, n, sin);
            t = gettime() - t;
        }
    }
    printf("\nmontecarlo\n");
    for(unsigned int n = 1; n <= 1000000; n *= 10) {
        double t, result;
        t = gettime();
        result = seq_montecarlo(a, b, n, sin);
        t = gettime() - t;
        printf("%u;%e;%e", n, t, result);
        for(unsigned int pidx = 0; pidx < 9; pidx++) {
            t = gettime();
            result = par_montecarlo(ps[pidx], a, b, n, sin);
            t = gettime() - t;
        }
    }
    printf("\n\n");

    return (EXIT_SUCCESS);
}

