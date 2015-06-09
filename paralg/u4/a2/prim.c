/*==============================================================================
  
   Purpose:    common parts for Prim's algorithm (minimum spanning tree)
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <libFHBRS.h>

#include "set.h"
#include "graph_dense.h"


// unlimited
#define UNLIM DBL_MAX


/*============================================================================*/
// Prim's algorithm

static void
prim (int n, graph_t g, int root, double d[n], int father[n])
{
  int v, u;
  edge_weight_t du, w;
  Set_t Q;
  SetIterator_t iter;
  AdjIterator_t adj_iter;


  assert (n > 0);
  assert (root >= 0);
  assert (root < n);
  assert (g != NULL);
  assert (g->n_vertex == n);

  // initialize Q and d
  Q = SetCreate (n);
  for (v = 0; v < n; v++)
    {
      // all other than root are part of V-Vtree
      if (v != root)
	{
	  // insert v into set
	  SetSetMember (Q, v);
	  // set weight vector
	  if (GraphHasEdge (g, root, v))
	    // edge (root,v) exists
	    d[v] = EdgeWeight (g, root, v);
	  else
	    // edge (root,v) does not exist
	    d[v] = UNLIM;
	  // my father so far
	  father[v] = root;
	}
    }

  // correct for root vertex
  d[root] = 0.0;
  father[root] = -1;

  // loop
  while (SetHasMember (Q) > 0)
    {
      // find a vertex u such that d[u] = min{d[v] | v in (V - VTree)}
      u = -1;
      du = UNLIM;
      for (v = SetStartIterator (Q, &iter);
	   SetTestIterator (Q, &iter); v = SetIncIterator (Q, &iter))
	{
	  if (d[v] < du)
	    {
	      du = d[v];
	      u = v;
	    }
	}

      if (u == -1)
	{
	  printf ("graph not connected\n");
	  return;
	}

      // remove that vertex from V-VTree
      SetUnsetMember (Q, u);

      // update least weight information
      for (v = AdjStartIterator (g, u, &adj_iter);
	   AdjTestIterator (g, u, &adj_iter);
	   v = AdjIncIterator (g, u, &adj_iter))
	{
	  if (SetIsMember (Q, v) && ((w = EdgeWeight (g, u, v)) < d[v]))
	    {
	      d[v] = w;
	      father[v] = u;
	    }
	}
    }

  SetDelete (Q);
}


/*============================================================================*/
// test program fro Prim's algorithm

int
main (int argc, char **argv)
{
  int n = atoi (argv[1]);	       // number of vertices
  float probability;
  graph_t g;
  double sum;
  double d[n];
  int father[n];
  int verbose = 0;


  sscanf (argv[2], "%f", &probability);
  // create a random graph with a edge probability "probability"
  g = GraphCreateRandom (n, (double) probability);
  if (verbose)
    GraphPrint (g);

  // run the algorithm
  double t0 = gettime ();
  prim (GraphNumberVertices (g), g, 0, d, father);
  double t1 = gettime ();

  printf ("time Prim algorithm = %10.6f\n", (t1 - t0));

  // calculate edge distance sum (for plausability checks)
  if (verbose)
    printf ("v d[v] father\n");
  sum = 0.0;
  for (int v = 0; v < n; v++)
    {
      if (verbose)
	printf ("%d %f %d\n", v, d[v], father[v]);
      sum += d[v];
    }
  printf ("edge sum=%f\n", sum);

  return 0;
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
