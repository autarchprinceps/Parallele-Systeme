/* 
 * File:   list.h
 * Author: Patrick Robinson
 *
 * Created on 27. April 2015, 19:11
 */

#ifndef LIST_H
#define	LIST_H

typedef double atype_t;

typedef struct _llist {
    atype_t data;
    struct _llist* next; 
} llist;

void ll_append(llist** start, atype_t data);
void ll_prepend(llist** start, atype_t data);
void ll_foreach(llist* start, void (*func)(atype_t*));

typedef struct {
    atype_t* data;
    unsigned int* next;
} clist;

clist* cl_create(int capacity);
void cl_append(clist* list, atype_t data);
void cl_foreach(clist* list, void (*func)(atype_t*));
#endif	/* LIST_H */

