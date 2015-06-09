/*==============================================================================
  
   Purpose:    dense graph
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#if !defined(GRAPH_DENSE_DEFINED)
#define GRAPH_DENSE_DEFINED


/*============================================================================*/
/* types */

typedef int vertex_t;		       // vertex handle
typedef int edge_t;		       // edge handle
typedef double vertex_weight_t;	       // vertex weight
typedef double edge_weight_t;	       // edge weight
typedef int AdjIterator_t;	       // iterator for adjacency lists


// graph
typedef struct
{
  int n_vertex;			       // number of vertices
  vertex_weight_t *adjmat;	       // adjacency matrix
} *graph_t;


/*============================================================================*/
/* function prototypes */

graph_t GraphCreate (void);
graph_t GraphCreateRandom (int n_vertex, double edge_probability);
edge_t GraphAddEdge (graph_t g, vertex_t from, vertex_t to, edge_weight_t weight);
int GraphHasEdge (graph_t g, vertex_t from, vertex_t to);
edge_weight_t EdgeWeight (graph_t g, vertex_t u, vertex_t v);
int GraphNumberVertices (graph_t g);
void GraphPrint (graph_t g);
vertex_t AdjStartIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter);
int AdjTestIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter);
vertex_t AdjIncIterator (graph_t g, vertex_t u, AdjIterator_t * adj_iter);

#endif

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
