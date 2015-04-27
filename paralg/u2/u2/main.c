/* 
 * File:   main.c
 * Author: Patrick Robinson
 *
 * Created on 27. April 2015, 19:10
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <libFHBRS.h>

void walker_function(atype_t* data) {
    (*data)++;
}

void print(char* title, unsigned int n, double t0, double t1, double t2, double t3) {
    printf(title);
    printf(" %u\n", n);
    printf("Init list: %e\n", t1 - t0);
    printf("Add elements: %e\n", t2 - t1);
    printf("For each: %e\n", t3 - t2);
}

void test_lists(unsigned int n) {
    double t0, t1, t2, t3;
    // llist append
    t0 = gettime();
    llist* list1 = NULL;
    t1 = gettime();
    for(unsigned int i = 0; i < n; i++) {
        ll_append(&list1, i);
    }
    t2 = gettime();
    ll_foreach(list1, &walker_function);
    t3 = gettime();
    print("llist append", n, t0, t1, t2, t3);
    // llist prepend
    t0 = gettime();
    llist* list2 = NULL;
    t1 = gettime();
    for(unsigned int i = 0; i < n; i++) {
        ll_prepend(&list2, i);
    }
    t2 = gettime();
    ll_foreach(list2, &walker_function);
    t3 = gettime();
    print("llist prepend", n, t0, t1, t2, t3);
    // clist append
    t0 = gettime();
    clist* list3 = cl_create(n+10);
    t1 = gettime();
    for(unsigned int i = 0; i < n; i++) {
        cl_append(&list3, i);
    }
    t2 = gettime();
    cl_foreach(list3, &walker_function);
    t3 = gettime();
    print("clist append", n, t0, t1, t2, t3);
}

int main(int argc, char** argv) {
    unsigned int ns = {1048576, 2097152, 4194304, 8388608, 16777216, 33554432};
    for(unsigned int i = 0; i < 6; i++) {
        test_lists(ns[i]);
    }
    return (EXIT_SUCCESS);
}

