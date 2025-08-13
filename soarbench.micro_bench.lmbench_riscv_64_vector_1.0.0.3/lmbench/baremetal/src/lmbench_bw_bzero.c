// -----------------------------------------------------------------------------
// lmbench bzero bandwidth test
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bench.h"

#define KB (1024)
#define MB (1024*1024)
#define GB (1024*1024*1024)

#ifndef SIZE
#define SIZE (16*KB)
#endif

#ifndef WARMLOOP
#define WARMLOOP 2
#endif

#ifndef BENCHLOOP
#define BENCHLOOP 10
#endif

typedef unsigned long int uint64_t;
extern uint64_t mem_start[];

// bzero - memset, write zeros to a byte string

void loop_bzero(int iterations, int *buf, int nbytes)
{	
  int *p = buf;
  
  while (iterations-- > 0)
  {
  	bzero(p, nbytes);
  }
}

int main (void)
{

  int size = SIZE;
  int warmup_loop = WARMLOOP;
  int bench_loop = BENCHLOOP;
  int *buf = (int *)mem_start;
  
  // Initialize
  bzero(buf, size);

  // Warmup
  loop_bzero(warmup_loop, buf, size);
  
  // PMU Start
  sim_resetstats();

  // Bench
  loop_bzero(bench_loop, buf, size);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();

  return 0;

}


