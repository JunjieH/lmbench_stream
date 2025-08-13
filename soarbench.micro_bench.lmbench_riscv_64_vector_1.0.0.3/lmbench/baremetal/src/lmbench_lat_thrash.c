// -----------------------------------------------------------------------------
// lmbench thrash latency test
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bench.h"

#define KB (1024)
#define MB (1024*1024)
#define GB (1024*1024*1024)

#if (!defined PAGE_1G) && (!defined PAGE_2M) && (!defined PAGE_4K)
#define PAGE 1*GB
#endif
#ifdef PAGE_1G
#define PAGE 1*GB
#endif
#ifdef PAGE_2M
#define PAGE 2*MB
#endif
#ifdef PAGE_4K
#define PAGE 4*KB
#endif

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

struct mem_state {
	char*	addr;	/* raw pointer returned by malloc */
	char*	base;	/* page-aligned pointer */
	char*	p;
	int	width;
	size_t	len;
	size_t	maxlen;
	size_t	line;
	size_t	pagesize;
	size_t	nlines;
	size_t	npages;
	size_t	nwords;
	size_t*	pages;
	size_t*	lines;
	size_t*	words;
};

int fixedpid()
{
    return 0x12345678;
}

int fixedppid()
{
    return 0x87654321;
}

size_t *permutation(size_t max, size_t scale)
{
	size_t i, v, o;
	static size_t r = 0;
    static size_t result[262144]; // max nmpages: 1GB/4KB

	if (result == NULL) return NULL;

	for (i = 0; i < max; ++i) {
		result[i] = i * scale;
	}

	if (r == 0)
		r = (fixedpid()<<6) ^ fixedppid() ^ rand() ^ (rand()<<10);

	/* randomize the sequence */
	for (i = 0; i < max; ++i) {
		r = (r << 1) ^ rand();
		o = r % max;
		v = result[o];
		result[o] = result[i];
		result[i] = v;
	}

	return (result);
}

void base_initialize(void* cookie)
{
	size_t	nwords, nlines, nbytes, npages, nmpages;
	size_t *pages;
	size_t *lines;
	size_t *words;
	struct mem_state* state = (struct mem_state*)cookie;
	register char *p = 0 /* lint */;

	nbytes = state->len;
	nwords = state->line / sizeof(char*);
	nlines = state->pagesize / state->line;
	npages = (nbytes + state->pagesize - 1) / state->pagesize;
	nmpages= (state->maxlen + state->pagesize - 1) / state->pagesize;

	//srand(fixedpid());

	words = NULL;
	lines = NULL;
	pages = permutation(nmpages, state->pagesize);
	p = state->addr;

	state->nwords = nwords;
	state->nlines = nlines;
	state->npages = npages;
	state->lines = lines;
	state->pages = pages;
	state->words = words;

	if (state->addr == NULL || pages == NULL)
		return;

	if ((unsigned long)p % state->pagesize) {
		p += state->pagesize - (unsigned long)p % state->pagesize;
	}
	state->base = p;
}

/*
 * words_initialize
 *
 * This is supposed to create the order in which the words in a "cache line" are used.
 * Since we rarely know the cache line size with any real reliability, we need to jump around so as to
 * maximize the number of potential cache misses, and to minimize the possibility of re-using a cache line.
 */
size_t *words_initialize(size_t max, int scale)
{
	size_t i, j, nbits;
    static size_t words[16777216]; // max nwords: 1G/64, FIXME

	bzero(words, max * sizeof(size_t));
	for (i = max>>1, nbits = 0; i != 0; i >>= 1, nbits++)
		;
	for (i = 0; i < max; ++i) {
		/* now reverse the bits */
		for (j = 0; j < nbits; j++) {
			if (i & (1<<j)) {
				words[i] |= (1<<(nbits-j-1));
			}
		}
		words[i] *= scale;
	}
	return words;
}

/*
 * Create a circular list of pointers with a random access pattern.
 * 
 * This stream corresponds more closely to linked list memory access patterns.
 * For large data structures each access will likely cause both a cache miss and a TLB miss.
 * 
 * Access a different page each time. This will eventually cause a tlb miss each page.
 * It will also cause maximal thrashing in the cache between the user data stream and the page table entries.
 */
void thrash_initialize(void* cookie)
{
	struct mem_state* state = (struct mem_state*)cookie;
	size_t	i;
	size_t	j;
	size_t	cur;
	size_t	next;
	size_t	cpage;
	size_t	npage;
	char*	addr;

	base_initialize(cookie);
	addr = state->base;

	if (state->len % state->pagesize) {
		state->nwords = state->len / state->line;
		state->words = words_initialize(state->nwords, state->line);
		for (i = 0; i < state->nwords - 1; ++i) {
			*(char **)&addr[state->words[i]] = (char*)&addr[state->words[i+1]];
		}
		*(char **)&addr[state->words[i]] = addr;
		state->p = addr;
	} else {
		state->nwords = state->pagesize / state->line;
		state->words = words_initialize(state->nwords, state->line);
		for (i = 0; i < state->npages - 1; ++i) {
			cpage = state->pages[i];
			npage = state->pages[i + 1];
			for (j = 0; j < state->nwords; ++j) {
				cur = cpage + state->words[(i + j) % state->nwords];
				next = npage + state->words[(i + j + 1) % state->nwords];
				*(char **)&addr[cur] = (char*)&addr[next];
			}
		}
		cpage = state->pages[i];
		npage = state->pages[0];
		for (j = 0; j < state->nwords; ++j) {
			cur = cpage + state->words[(i + j) % state->nwords];
			next = npage + state->words[(j + 1) % state->nwords];
			*(char **)&addr[cur] = (char*)&addr[next];
		}
		state->p = (char*)&addr[state->pages[0]];
	}
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

  struct mem_state state;
  state.addr = addr;
  state.base = addr;
  state.width = 1;
  state.len = size;
  state.maxlen = size;
  state.line = stride;
  state.pagesize = PAGE;
  
  // Initialize
  thrash_initialize(&state);

  // Warmup
  benchmark_loads(warmup_loop, size, stride, state.base);
  
  // PMU Start
  sim_resetstats();

  // Bench
  benchmark_loads(bench_loop, size, stride, state.base);
  
  // PMU End
  sim_dumpstats();

  // Sim exit
  sim_exit();

  return 0;

}


