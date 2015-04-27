/* 
 * File:   list.h
 * Author: Patrick Robinson
 *
 * Created on 27. April 2015, 19:11
 */

#ifndef LIST_H
#define	LIST_H

typedef struct _llist {
    void* data;
    struct _llist* next; 
} llist;

void ll_append(llist** start, void* data);
void ll_prepend(llist** start, void* data);

typedef struct {
    void** data;
    unsigned int* next;
} clist;

clist* cl_create(int capacity);
void cl_append(clist* list, void* data);
#endif	/* LIST_H */

