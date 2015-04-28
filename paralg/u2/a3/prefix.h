/* 
 * File:   prefix.h
 * Author: Patrick Robinson
 *
 * Created on 28. April 2015, 12:35
 */

#ifndef PREFIX_H
#define	PREFIX_H

typedef double atype_t;

void prefix(unsigned int p, unsigned int n, atype_t values[n], atype_t (*f)(atype_t, atype_t));

#endif	/* PREFIX_H */

