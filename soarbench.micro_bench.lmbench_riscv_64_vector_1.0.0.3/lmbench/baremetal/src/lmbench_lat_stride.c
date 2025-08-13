// -----------------------------------------------------------------------------
// lmbench stride latency test
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

#ifndef STRIDE
#define STRIDE 1024
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

/*
 * Create a circular list of pointers using a simple striding algorithm.  
 * 
 * This access pattern corresponds to many array/matrix algorithms.
 * It should be easily and correctly predicted by any decent hardware prefetch algorithm.
 */
void stride_initialize(int range, int stride, char *addr)
{
	int i;

	for (i = stride; i < range; i += stride) {
		*(char **)&addr[i - stride] = (char*)&addr[i];
	}
	*(char **)&addr[i - stride] = (char*)&addr[0];
}

#define	ONE	p = (char **)*p;
#define	FIVE	ONE ONE ONE ONE ONE
#define	TEN	FIVE FIVE
#define	FIFTY	TEN TEN TEN TEN TEN
#define	HUNDRED	FIFTY FIFTY

void benchmark_loads(int iterations, int range, int stride, char *addr)
{
	char **p = (char**)addr;
	int i;
	int count = range / (stride * 100) + 1;

	while (iterations-- > 0) {
		for (i = 0; i < count; ++i) {
			HUNDRED;
		}
	}

	use_pointer((void *)p);
}

int main (void)
{
  int size = SIZE;
  int stride = STRIDE;
  int warmup_loop = WARMLOOP;
  int bench_loop = BENCHLOOP;
  char *addr = (char *)mem_start;
  
  // Initialize
  stride_initialize(size, stride, addr);

  // Warmup
  benchmark_loads(warmup_loop, size, stride, addr);
  
  // PMU Start
  sim_resetstats();

  // Bench
  benchmark_loads(bench_loop, size, stride, addr);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();

  return 0;

}


