/*==============================================================================
  
   Purpose:    set implementation
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/


#if !defined(SET_H_INCLUDED)
#define SET_H_INCLUDED

/*============================================================================*/
// set type

typedef struct
{
  char *members;		       // members
  int max_members;		       // max. numbers of members
  int actual_members;		       // actual number of members
} *Set_t;

// set iterator
typedef int SetIterator_t;


/*============================================================================*/
/* function prototypes */

Set_t SetCreate (int n);
void SetDelete (Set_t s);
int SetHasMember (Set_t s);
void SetSetMember (Set_t s, int member);
void SetUnsetMember (Set_t s, int member);
int SetIsMember (Set_t s, int member);
void SetPrint (Set_t s, char *name);

int SetStartIterator (Set_t s, SetIterator_t * iter);
int SetTestIterator (Set_t s, SetIterator_t * iter);
int SetIncIterator (Set_t s, SetIterator_t * iter);

/*============================================================================*/

#endif

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
