// -----------------------------------------------------------------------------
// lmbench bcopy bandwidth test
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

// bcopy - memmove, copy nbytes from src byte string to dest byte string

void loop_bcopy(int iterations, int *buf, int *buf2, int nbytes)
{	
  int *p = buf;
  int *dst = buf2;

  while (iterations-- > 0)
  {
	bcopy(p, dst, nbytes);
  }
}

int main (void)
{

  int size = SIZE;
  int warmup_loop = WARMLOOP;
  int bench_loop = BENCHLOOP;
  int *buf = (int *)mem_start;
  int *buf2 = (int *)(mem_start + 0x10000000/sizeof(int));
  int *lastone = (int *)((char *)buf + size - 512);
  
  // Initialize
  bzero(buf, size);

  // Warmup
  loop_bcopy(warmup_loop, buf, buf2, size);
  
  // PMU Start
  sim_resetstats();

  // Bench
  loop_bcopy(bench_loop, buf, buf2, size);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();
  
  return 0;

}


