# Optimization for memset() function.
# Currently, RISCV only.

/* void *memset(void *, int, size_t) */

#define REG_S sd
#define SZREG 8

  .text
  .globl memset
  .type  memset, @function
memset:
	move t0, a0  /* Preserve return value */

	/* Defer to byte-oriented fill for small sizes */
	sltiu a3, a2, 16
	bnez a3, .Lend_misalignment
#ifdef __riscv_zicboz
#define CACHE_LINE_SIZE 64
	bnez	a1, .Ldo_memset
	/*
	 * t1 will be the Zicboz block size.
	 * Zero means we're not using Zicboz, and we don't when a1 != 0
	 */
	li	t1, CACHE_LINE_SIZE

	/*
	 * Round to nearest Zicboz block-aligned address
	 * greater than or equal to the start address.
	 */
	addi a3, t0, CACHE_LINE_SIZE-1
	andi t3, a3, ~(CACHE_LINE_SIZE-1) /* t3 is Zicboz block aligned start */

	/* Did we go too far or not have at least one block? */
	add	a3, a0, a2
	andi	a3, a3, ~(CACHE_LINE_SIZE-1)
	bltu	a3, t3, .Ldo_memset

.Ldo_zero:
	beq	t3, t0, .Lzero
	/* Handle initial misalignment */
	sub a4, t3, t0

.Lzero_initial_misalignment:
	sb zero, 0(t0)
	addi t0, t0, 1
	bltu t0, t3, .Lzero_initial_misalignment
	sub a2, a2, a4  /* Update count */

.Lzero:
	/* Calculate end address */
	andi	a3, a2, ~(CACHE_LINE_SIZE-1)
	add	a3, t0, a3
	sub	a4, a3, t0

.Lzero_loop:
	cbo.zero 0(t0)
	addi	t0, t0, CACHE_LINE_SIZE
	bltu	t0, a3, .Lzero_loop

	sub	a2, a2, a4		/* Update count */
	j	.Lend_misalignment
#endif

.Ldo_memset:
	/*
	 * Round to nearest XLEN-aligned address
	 * greater than or equal to start address
	 */
	addi a3, t0, SZREG-1
	andi a3, a3, ~(SZREG-1)
	beq a3, t0, .Lduff_device  /* Skip if already aligned */
	/* Handle initial misalignment */
	sub a4, a3, t0
.Linitial_misalignment:
	sb a1, 0(t0)
	addi t0, t0, 1
	bltu t0, a3, .Linitial_misalignment
	sub a2, a2, a4  /* Update count */

.Lduff_device: /* Duff's device with 32 XLEN stores per iteration */
	/* Broadcast value into all bytes */
	andi a1, a1, 0xff
	slli a3, a1, 8
	or a1, a3, a1
	slli a3, a1, 16
	or a1, a3, a1
	slli a3, a1, 32
	or a1, a3, a1

	/* Calculate end address */
	andi a4, a2, ~(SZREG-1)
	add a3, t0, a4

	andi a4, a4, 31*SZREG  /* Calculate remainder */
	beqz a4, .Lduff_device_loop            /* Shortcut if no remainder */
	neg a4, a4
	addi a4, a4, 32*SZREG  /* Calculate initial offset */

	/* Adjust start address with offset */
	sub t0, t0, a4

	/* Jump into loop body */
	/* Assumes 32-bit instruction lengths */
	la a5, .Lduff_device_loop
	srli a4, a4, 1
	add a5, a5, a4
	jr a5
.Lduff_device_loop:
	REG_S a1,        0(t0)
	REG_S a1,    SZREG(t0)
	REG_S a1,  2*SZREG(t0)
	REG_S a1,  3*SZREG(t0)
	REG_S a1,  4*SZREG(t0)
	REG_S a1,  5*SZREG(t0)
	REG_S a1,  6*SZREG(t0)
	REG_S a1,  7*SZREG(t0)
	REG_S a1,  8*SZREG(t0)
	REG_S a1,  9*SZREG(t0)
	REG_S a1, 10*SZREG(t0)
	REG_S a1, 11*SZREG(t0)
	REG_S a1, 12*SZREG(t0)
	REG_S a1, 13*SZREG(t0)
	REG_S a1, 14*SZREG(t0)
	REG_S a1, 15*SZREG(t0)
	REG_S a1, 16*SZREG(t0)
	REG_S a1, 17*SZREG(t0)
	REG_S a1, 18*SZREG(t0)
	REG_S a1, 19*SZREG(t0)
	REG_S a1, 20*SZREG(t0)
	REG_S a1, 21*SZREG(t0)
	REG_S a1, 22*SZREG(t0)
	REG_S a1, 23*SZREG(t0)
	REG_S a1, 24*SZREG(t0)
	REG_S a1, 25*SZREG(t0)
	REG_S a1, 26*SZREG(t0)
	REG_S a1, 27*SZREG(t0)
	REG_S a1, 28*SZREG(t0)
	REG_S a1, 29*SZREG(t0)
	REG_S a1, 30*SZREG(t0)
	REG_S a1, 31*SZREG(t0)
	addi t0, t0, 32*SZREG
	bltu t0, a3, .Lduff_device_loop
	andi a2, a2, SZREG-1  /* Update count */

.Lend_misalignment:
	/* Handle trailing misalignment */
	beqz a2, .Lfinish
	add a3, t0, a2
.Lend_misalignment_loop:
	sb a1, 0(t0)
	addi t0, t0, 1
	bltu t0, a3, .Lend_misalignment_loop
.Lfinish:
	ret
