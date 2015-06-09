/*==============================================================================
  
   Purpose          : breadth first search (BFS)
   Author           : Rudolf Berrendorf
                      Computer Science Department
                      Bonn-Rhein-Sieg University of Applied Sciences
	              53754 Sankt Augustin, Germany
                      rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#include <omp.h>

#include <libFHBRS.h>
#include <graph.h>


//==============================================================================

// infinity for weights
#define UNLIMITED LONG_MAX

int verbosity_level = 0;

//==============================================================================

static counter_t
bfs_sequential (graph_t g,	       // I: connected undirected graph
		vertex_t source,       // I: source vertex
		counter_t bfs_level[]  // O: array with BFS level for each vertex
	)
{
  counter_t max_level;
  counter_t n_vertex = graphGetNVertices (g);

  // each vertex is in one of two states (visited / unvisited)
  bool *visited = (bool *)malloc(n_vertex * sizeof(*visited));
  assert(visited != NULL);

  // simple vertex queue implementation (at most n_vertex items enqueued!)
  vertex_t *queue;
  queue = (vertex_t *)malloc (n_vertex * sizeof (*queue));
  assert(queue != NULL);
  // read / write positions in queue
  counter_t queue_read, queue_write;
  // access functions
#define queueinit()  (queue_read = queue_write = 0)
#define queueisempty() (queue_read == queue_write)
#define enqueue(x)   (queue[queue_write++]=x)
#define dequeue()    (queue[queue_read++])


  // initialize
  max_level = 0;
  queueinit();
  graphVertexIterator (g, v)
  {
    visited[v] = false;
    bfs_level[v] = UNLIMITED;
  }

  // start with source vertex
  visited[source] = true;
  bfs_level[source] = 0;
  enqueue (source);

  // main loop: run as long as queued vertices exist
  while (!queueisempty ())
    {
      // get next queue entry
      vertex_t v = dequeue ();

      // loop over all adjacent vertices
      graphAdjacentIterator (g, v, ai)
      {
	edge_t e = graphAdjacentIteratorItem (g, v, ai);
	vertex_t u = graphEdgeGetToVertex (g, e);

	if (!visited[u])
	  {
	    // unvisited vertex changed to visited
	    visited[u] = true;

	    // all neighbors have one level more than my level
	    if((bfs_level[u] = bfs_level[v] + 1) > max_level)
	      max_level = bfs_level[u];

	    // add neighbor to queue
	    enqueue (u);
	  }
      }
    }

  // free temporary memory
  free(visited);
  free(queue);

  // return maximum level
  return max_level;
}

//==============================================================================

static counter_t
bfs_parallel (graph_t g,	       // I: connected undirected graph
	      vertex_t source,	       // I: source vertex
	      counter_t bfs_level[]    // O: array with BFS level for each vertex
	)
{
  counter_t n_vertices = graphGetNVertices (g);
  counter_t max_level = -1;



  // here comes your code ...
#pragma omp parallel
  ;


  return max_level;
}

//==============================================================================

int
main (int argc, char **argv)
{
  double t, t_seq;
  counter_t level;
  int procs[] = {1,2,4,8,16,24,32,40,48,56,-1};


  // get command line parameters
  if (argc != 2)
    {
      printf ("usage: %s graph_file\n", argv[0]);
      exit (1);
    }

  // generate random undirected vertex and edge un-weighted connected graph
  printf ("read graph...\n");
  fflush (stdout);
  graph_t g = graphRead (argv[1]);
  if(g == NULL)
    {
      printf("error in reading graph from file %s\n", argv[1]);
      exit(1);
    }
  graphPrint (g, 0);
  counter_t n_vertices = graphGetNVertices (g);
  counter_t n_edges = graphGetNEdges (g);

  int verbose = (n_vertices < 20);

  // allocate memory
  counter_t *bfs_level = (counter_t *)malloc(n_vertices * sizeof (*bfs_level));
  assert(bfs_level != NULL);


  // sequential
  t_seq = gettime ();
  level = bfs_sequential (g, (vertex_t) 0, bfs_level);
  t_seq = gettime () - t_seq;
  printf ("sequential bfs           : %10.6f s (max-level=%ld)\n", t_seq, level);
  if (verbose)
    {
      for (vertex_t v = 0; v < n_vertices; v++)
	printf ("%ld: level %ld\n", v, bfs_level[v]);
    }


  // parallel
  for(int i=0; procs[i] != -1; i++)
    {
      int p = procs[i];
      omp_set_num_threads(p);

      // dummy parallel region to get n threads initialized before we start the algorithm
#pragma omp parallel
      ;

      // here comes the call to the parallel version
      t = gettime ();
      level = bfs_parallel (g, (vertex_t) 0, bfs_level);
      t = gettime () - t;

      printf ("parallel bfs (%2d threads): %10.6f s (max-level=%ld), speedup=%.2f\n", p, t, level, t_seq/t);
      if (verbose)
	{
	  for (vertex_t v = 0; v < n_vertices; v++)
	    printf ("%ld: level %ld\n", v, bfs_level[v]);
	}
    }


  // free memory
  free (bfs_level);
  graphDelete (g);
  //memoryStatistics();


  return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
