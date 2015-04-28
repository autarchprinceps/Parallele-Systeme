#include "list.h"
#include <stdlib.h>
#include <stdio.h>

void ll_append(llist** start, atype_t data) {
    // printf("DEBUG ll_append pre %d\n", start);
    if(!(*start)) {
        // printf("DEBUG ll_append first init\n");
        *start = malloc(sizeof(llist));
        (*start)->next = NULL;
        (*start)->data = data;
        // printf("DEBUG ll_append first init exit\n");
        return;
    }
    // printf("DEBUG ll_append later %d\n", start);
    llist* elem = (*start)->next;
    // printf("DEBUG ll_append later elem assigned\n");
    llist* prev_elem = *start;
    // printf("DEBUG ll_append later prev_elem assigned\n");
    while(elem) {
        // printf("DEBUG ll_append later while\n");
        prev_elem = elem;
        elem = elem->next;
    }
    // printf("DEBUG ll_append later found end\n");
    elem = malloc(sizeof(llist));
    elem->next = NULL;
    elem->data = data;
    prev_elem->next = elem;
    // printf("DEBUG ll_append later exit\n");
}

void ll_prepend(llist** start, atype_t data) {
    llist* first = *start;
    *start = malloc(sizeof(llist));
    (*start)->next = first;
    (*start)->data = data;
}

void ll_foreach(llist* start, void (*func)(atype_t*)) {
    while(start) {
        (*func)(&start->data);
        start = start->next;
    }
}

clist* cl_create(int capacity) {
    clist* result = malloc(sizeof(clist));
    result->data = calloc(sizeof(atype_t), capacity);
    result->next = calloc(sizeof(unsigned int), capacity);
    return result;
}

void cl_append(clist* list, atype_t data) {
    unsigned int first_free = 0;
    while(list->next[first_free++]);
    
    list->data[first_free] = data;
    list->next[first_free - 1] = first_free;
}

void cl_foreach(clist* list, void (*func)(atype_t*)) {
    unsigned int idx = 0;
    while(list->next[idx]) {
        (*func)(&list->data[idx]);
        idx++;
    }
    (*func)(&list->data[idx]);
}