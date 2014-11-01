#include "pomp_lib.h"


extern struct ompregdescr omp_rd_1;
extern struct ompregdescr omp_rd_2;
extern struct ompregdescr omp_rd_3;
extern struct ompregdescr omp_rd_4;
extern struct ompregdescr omp_rd_5;
extern struct ompregdescr omp_rd_6;
extern struct ompregdescr omp_rd_7;
extern struct ompregdescr omp_rd_8;
extern struct ompregdescr omp_rd_9;

int POMP_MAX_ID = 10;

struct ompregdescr* pomp_rd_table[10] = {
  0,
  &omp_rd_1,
  &omp_rd_2,
  &omp_rd_3,
  &omp_rd_4,
  &omp_rd_5,
  &omp_rd_6,
  &omp_rd_7,
  &omp_rd_8,
  &omp_rd_9,
};
