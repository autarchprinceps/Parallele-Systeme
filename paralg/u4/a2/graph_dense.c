/*==============================================================================
  
   Purpose:    dense graphs
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <assert.h>

#include "graph_dense.h"

// some value for unlimited
#define UNLIM DBL_MAX

// access the i-th row and j-th column of the adjacency matrix of graph g
#define edge(g,i,j) ((g)->adjmat[((i)*(g)->n_vertex)+(j)])


/*============================================================================*/
// an error was raised

static void
GraphError (char *message)
{
  printf ("error: %s\n", message);
  exit (1);
}

/*============================================================================*/
// create an empty graph

graph_t
GraphCreate (void)
{
  graph_t g;

  // allocate memory for graph
  g = calloc (1, sizeof (*g));
  if (g == NULL)
    GraphError ("no more memory");
  // no vertices or edges so far
  g->n_vertex = 0;
  g->adjmat = NULL;

  return g;
}

/*============================================================================*/
// create a random graph

graph_t
GraphCreateRandom (int n_vertex, double edge_probability)
{
  int64_t nedges = (int64_t) ((double)n_vertex * n_vertex * edge_probability) + n_vertex;
  double r;
  graph_t g;
  vertex_t v, u, v2;
  edge_weight_t w;
  edge_t e;
  int max_neighbor;
  int neighbors[n_vertex];
  int nloop;


  assert (n_vertex > 0);
  assert (edge_probability >= 0.0);
  assert (edge_probability <= 1.0);

  // generate an empty graph
  printf("creating random graph ...\n");
  g = GraphCreate ();
  assert (g != NULL);

  g->n_vertex = n_vertex;
  g->adjmat = calloc (n_vertex * n_vertex, sizeof (*(g->adjmat)));
  for (v = 0; v < n_vertex; v++)
    {
      for (v2 = 0; v2 < n_vertex; v2++)
	edge (g, v, v2) = UNLIM;
      neighbors[v] = 0;
    }

  // generate random edges
  for (e = 0; e < nedges - n_vertex; e++)
    {
      // at some point we give up if we can not generate a new edge that
      // doesn't exist already
      nloop = 1000000;
      do
	{
	  // determine two random vertices
	  r = ((double) rand () / (RAND_MAX + 1.0));
	  v = (vertex_t) (r * n_vertex);
	  assert ((v >= 0) && (v < n_vertex));
	  r = ((double) rand () / (RAND_MAX + 1.0));
	  u = (vertex_t) (r * n_vertex);
	  assert ((u >= 0) && (u < n_vertex));
	  // and check if that edge exists already
      } while (((v == u) || GraphHasEdge (g, v, u)) && (--nloop > 0));
      if (nloop == 0)
	// we did not succeed to generate a non-existing edge
	printf ("double edge inserted\n");

      // determine edge weight
      w = ((double) rand () / (RAND_MAX + 1.0));
      // add edge to graph
      GraphAddEdge (g, v, u, w);
      // remember number of neighbors of each node
      ++neighbors[v];
      ++neighbors[u];
    }

  // guarantee a connected graph with artifical edges with high costs
  v = 0;
  for (e = nedges - n_vertex; e < nedges; e++)
    {
      u = (v >= n_vertex - 1) ? 0 : v + 1;
      assert ((u >= 0) && (u < n_vertex));
      assert ((v >= 0) && (v < n_vertex));
      if ((u != v) && !GraphHasEdge (g, v, u))
	{
	  GraphAddEdge (g, v, u, 100000.0);
	  ++v;
	  ++neighbors[v];
	  ++neighbors[u];
	}
      else
	--nedges;
    }


  // do some statistics
  max_neighbor = 0;
  for (v = 0; v < n_vertex; v++)
    if (neighbors[v] > max_neighbor)
      max_neighbor = neighbors[v];

  printf ("average edge degree: %8.2f\n", (double) nedges / n_vertex);
  printf ("maximum edge degree: %8d\n", max_neighbor / 2);

  return g;
}

/*============================================================================*/

edge_t
GraphAddEdge (graph_t g, vertex_t from, vertex_t to, edge_weight_t weight)
{
  edge (g, from, to) = weight;
  edge (g, to, from) = weight;

  return to;
}

/*============================================================================*/

int
GraphHasEdge (graph_t g, vertex_t from, vertex_t to)
{
  return edge (g, from, to) != UNLIM;
}

/*============================================================================*/
// number of vertices

int
GraphNumberVertices (graph_t g)
{
  return g->n_vertex;
}

/*============================================================================*/

double
EdgeWeight (graph_t g, vertex_t from, vertex_t to)
{
  return edge (g, from, to);
}

/*============================================================================*/
// print a graph

void
GraphPrint (graph_t g)
{
  vertex_t v, u;

  assert (g != NULL);
  assert (g->n_vertex >= 0);

  printf ("%d vertices\n", g->n_vertex);
  for (v = 0; v < g->n_vertex; v++)
    {
      printf ("%d: ", v);
      for (u = 0; u < g->n_vertex; u++)
	if (GraphHasEdge (g, v, u))
	  printf ("%d(%f) ", u, EdgeWeight (g, v, u));
      printf ("\n");
    }
}

/*============================================================================*/
// start an iterator

vertex_t
AdjStartIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter)
{
  int i;

  assert (g != NULL);
  assert (u >= 0);
  assert (u < g->n_vertex);
  assert (adj_iter != NULL);

  for (i = 0; i < g->n_vertex; i++)
    if (edge (g, u, i) != UNLIM)
      break;

  if (i == g->n_vertex)
    {
      *adj_iter = g->n_vertex;
      return -1;
    }
  else
    {
      *adj_iter = i;
      return i;
    }
}

/*============================================================================*/
// test an iterator

int
AdjTestIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter)
{
  assert (g != NULL);
  assert (adj_iter != NULL);
  assert (*adj_iter >= 0);

  if (*adj_iter == g->n_vertex)
    return 0;
  else
    return 1;
}

/*============================================================================*/
// increment an iterator

vertex_t
AdjIncIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter)
{
  int i;

  assert (g != NULL);
  assert (adj_iter != NULL);
  assert (*adj_iter >= 0);

  for (i = (*adj_iter) + 1; i < g->n_vertex; i++)
    if (edge (g, u, i) != UNLIM)
      break;

  if (i == g->n_vertex)
    {
      *adj_iter = g->n_vertex;
      return -1;
    }
  else
    {
      *adj_iter = i;
      return i;
    }
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
