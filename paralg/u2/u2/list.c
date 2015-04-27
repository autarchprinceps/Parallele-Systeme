#include "list.h"
#include <stdlib.h>

void ll_append(llist** start, void* data) {
    llist* elem = *start;
    while(elem) {
        elem = elem->next;
    }
    elem = malloc(sizeof(llist));
    elem->next = NULL;
    elem->data = data;
}

void ll_prepend(llist** start, void* data) {
    llist* first = *start;
    *start = malloc(sizeof(llist));
    (*start)->next = first;
    (*start)->data = data;
}

clist* cl_create(int capacity) {
    clist* result = malloc(sizeof(clist));
    result->data = calloc(sizeof(void*), capacity);
    result->next = calloc(sizeof(unsigned int), capacity);
    return result;
}

void cl_append(clist* list, void* data) {
    unsigned int first_free = 0;
    while(!list->next[first_free++]);
    
    list->data[first_free] = data;
    list->next[first_free - 1] = first_free;
}
