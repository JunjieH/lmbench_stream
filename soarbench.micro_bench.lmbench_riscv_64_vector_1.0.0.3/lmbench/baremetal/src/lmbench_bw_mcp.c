// -----------------------------------------------------------------------------
// lmbench mcp bandwidth test
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

void use_pointer(void *result)
{
  use_result_dummy += (long)result;
}

// mcp - 4 byte read then 4 byte write to different place, 16 byte stride
// 512 byte chunks in a loop

void mcp(int iterations, int *buf, int *buf2, int *lastone)
{
  int *p = NULL;
  int *dst = NULL;
  int *p_save = NULL;

  while (iterations-- > 0)
  {
    p = buf;
    dst = buf2;
    while (p <= lastone)
    {
#define	DOIT(i)	dst[i] = p[i];
		DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
		DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
		DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
		DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
		DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
		p += 128;
		dst += 128;
	}
	p_save = p;
  }
  use_pointer(p_save);
}
#undef	DOIT

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
  mcp(warmup_loop, buf, buf2, lastone);
  
  // PMU Start
  sim_resetstats();

  // Bench
  mcp(bench_loop, buf, buf2, lastone);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();

  return 0;

}


