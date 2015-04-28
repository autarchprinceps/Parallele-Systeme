/* 
 * File:   tree.h
 * Author: Patrick Robinson
 *
 * Created on 27. April 2015, 23:11
 */

#ifndef TREE_H
#define	TREE_H

typedef double atype_t;
atype_t balanced_tree(int n1 , atype_t values[n1], atype_t (*f)(atype_t x, atype_t y));

#endif	/* TREE_H */

