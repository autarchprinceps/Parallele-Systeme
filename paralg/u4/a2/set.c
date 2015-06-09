/*==============================================================================
  
   Purpose:    set implementation
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "set.h"

/*============================================================================*/
/* create a set with n members named 0,...,n-1 */

Set_t
SetCreate (int n)
{
  assert(n >= 0);

  // allocate memory
  Set_t s = malloc (sizeof (*s));
  s->members = calloc (n, sizeof (*(s->members)));
  // in total n elements
  s->max_members = n;
  // currently no set member
  s->actual_members = 0;

  return s;
}

/*============================================================================*/
/* delete a set */

void
SetDelete (Set_t s)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  // free memory
  free (s->members);
  free (s);
}

/*============================================================================*/
/* query for current number of elements in set */

int
SetHasMember (Set_t s)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  // array element non-zero?
  return s->actual_members;
}

/*============================================================================*/
/* add an element */

void
SetSetMember (Set_t s, int member)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);
  assert(member >= 0);
  assert(member < s->max_members);

  // increment number of set members by 1 if not already in set
  if (s->members[member] == 0)
    s->actual_members++;

  // this element is a member
  s->members[member] = 1;
}

/*============================================================================*/
/* delete an element */

void
SetUnsetMember (Set_t s, int member)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);
  assert(member >= 0);
  assert(member < s->max_members);

  // decrease number of set members by 1 if member in set
  if (s->members[member] == 1)
    s->actual_members--;

  // this element is no longer a member
  s->members[member] = 0;
}

/*============================================================================*/
/* test whether an element is present in set */

int
SetIsMember (Set_t s, int member)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);
  assert(member >= 0);
  assert(member < s->max_members);

  return s->members[member] == 1;
}

/*============================================================================*/
/* set iterator */

int
SetStartIterator (Set_t s, SetIterator_t * iter)
{
  int i;

  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  // search for first member in set (if present)
  for (i = 0; i < s->max_members; i++)
    if (s->members[i] == 1)
      break;

  // store and return position if first member
  *iter = i;
  return i;
}

int
SetTestIterator (Set_t s, SetIterator_t * iter)
{
  int i;

  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  // start at old position and check for next member
  for (i = *iter; i < s->max_members; i++)
    if (s->members[i] == 1)
      break;

  // store position of next member
  *iter = i;
  // indicate with return value success or failure of search
  if (i >= s->max_members)
    return 0;
  else
    return 1;
}

int
SetIncIterator (Set_t s, SetIterator_t * iter)
{
  int i;

  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  // start a next position and search for next member
  for (i = *iter + 1; i < s->max_members; i++)
    if (s->members[i] == 1)
      break;

  *iter = i;
  return i;
}

/*============================================================================*/
/* print out a set */

void
SetPrint (Set_t s, char *name)
{
  assert(s != NULL);
  assert(s->max_members >= 0);
  assert(s->actual_members >= 0);
  assert(s->actual_members < s->max_members);

  printf ("%s:\n", name);
  for (int i = 0; i < s->max_members; i++)
    if (s->members[i] == 1)
      printf ("%d ", i);
  printf ("\n");
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
