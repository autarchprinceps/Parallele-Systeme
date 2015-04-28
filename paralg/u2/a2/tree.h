/* 
 * File:   tree.h
 * Author: Patrick Robinson
 *
 * Created on 28. April 2015, 10:53
 */

#ifndef TREE_H
#define	TREE_H

typedef double atype_t;
atype_t balanced_tree(unsigned int p, unsigned long n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y));

#endif	/* TREE_H */

