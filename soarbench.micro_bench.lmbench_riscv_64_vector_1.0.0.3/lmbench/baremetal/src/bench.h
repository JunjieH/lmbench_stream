// -----------------------------------------------------------------------------
// common bench code
// -----------------------------------------------------------------------------
#ifndef _BENCH_H
#define _BENCH_H

#ifndef WARMLOOP
#define WARMLOOP 4
#endif

#ifndef BENCHLOOP
#define BENCHLOOP 10000
#endif
// M5Op: resetstats
static void sim_resetstats(void) {
    __asm__ __volatile__("csrr x0, sstatus"); 
    __asm__ __volatile__("slti x0, x0, 0x44"); };

// M5Op: dumpstats
static void sim_dumpstats(void) { 
    __asm__ __volatile__("csrr x0, sstatus"); 
    __asm__ __volatile__("slti x0, x0, 0x45"); };

// M5Op: exit
static void sim_exit(void) { 
    __asm__ __volatile__("csrr x0, sstatus"); 
    __asm__ __volatile__("slti x0, x0, 0x24"); };

#define NOP_ONE "nop;"
#define NOP_FIVE NOP_ONE NOP_ONE NOP_ONE NOP_ONE NOP_ONE
#define NOP_TEN NOP_FIVE NOP_FIVE
#define NOP_FIFTY NOP_TEN NOP_TEN NOP_TEN NOP_TEN NOP_TEN
#define NOP_HUNDRED NOP_FIFTY NOP_FIFTY

static void dummy_code(void) { __asm__ __volatile__(NOP_HUNDRED); }

#endif
