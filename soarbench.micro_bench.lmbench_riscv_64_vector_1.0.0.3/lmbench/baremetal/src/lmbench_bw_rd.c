// -----------------------------------------------------------------------------
// lmbench rd bandwidth test
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

static volatile uint64_t use_result_dummy;

void use_int(int result)
{
  use_result_dummy += result; 
}

// rd - 4 byte read, 16 byte stride
// 512 byte chunks in a loop

void rd(int iterations, int *buf, int *lastone)
{
  int *p = NULL;
  int sum = 0;

  while (iterations-- > 0)
  {
    p = buf;
    while (p <= lastone)
    {
        sum += 
#define	DOIT(i)	p[i]+
     	DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
     	DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
     	DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
     	DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
     	DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) 
     	p[124];
     	p += 128;
    }
  }
  use_int(sum);
}
#undef	DOIT

int main (void)
{

  int size = SIZE;
  int warmup_loop = WARMLOOP;
  int bench_loop = BENCHLOOP;
  int *buf = (int *)mem_start;
  int *lastone = (int *)((char *)buf + size - 512);
  
  // Initialize
  bzero(buf, size);

  // Warmup
  rd(warmup_loop, buf, lastone);
  
  // PMU Start
  sim_resetstats();

  // Bench
  rd(bench_loop, buf, lastone);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();

  return 0;

}


