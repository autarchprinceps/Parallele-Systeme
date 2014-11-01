#include "pomp_lib.h"

struct ompregdescr omp_rd_1 = {
  "for", "", 0, "heat.v1.c", 35, 35, 38, 38
};

struct ompregdescr omp_rd_2 = {
  "for", "", 0, "heat.v1.c", 51, 51, 58, 58
};

struct ompregdescr omp_rd_3 = {
  "for", "", 0, "heat.v1.c", 65, 65, 68, 68
};

struct ompregdescr omp_rd_4 = {
  "for", "", 0, "heat.v1.c", 71, 71, 74, 74
};

struct ompregdescr omp_rd_5 = {
  "parallelfor", "", 0, "heat.v1.c", 81, 81, 101, 101
};

#define POMP_DLIST_00005 shared(omp_rd_5,omp_rd_6)

struct ompregdescr omp_rd_6 = {
  "critical", "draw", 0, "heat.v1.c", 95, 95, 99, 99
};

struct ompregdescr omp_rd_7 = {
  "parallelfor", "", 0, "heat.v1.c", 114, 114, 117, 117
};

#define POMP_DLIST_00007 shared(omp_rd_7)

struct ompregdescr omp_rd_8 = {
  "parallel", "", 0, "heat.v1.c", 178, 178, 185, 185
};

#define POMP_DLIST_00008 shared(omp_rd_8)

struct ompregdescr omp_rd_9 = {
  "parallel", "", 0, "heat.v1.c", 201, 201, 208, 208
};

#define POMP_DLIST_00009 shared(omp_rd_9)

