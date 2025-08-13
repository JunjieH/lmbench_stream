
.section .text

.global _reset

_reset:
    # Initialize GPR
    li  x1, 0
    li  x2, 0
    li  x3, 0
    li  x4, 0
    li  x5, 0
    li  x6, 0
    li  x7, 0
    li  x8, 0
    li  x9, 0
    li  x10,0
    li  x11,0
    li  x12,0
    li  x13,0
    li  x14,0
    li  x15,0
    li  x16,0
    li  x17,0
    li  x18,0
    li  x19,0
    li  x20,0
    li  x21,0
    li  x22,0
    li  x23,0
    li  x24,0
    li  x25,0
    li  x26,0
    li  x27,0
    li  x28,0
    li  x29,0
    li  x30,0
    li  x31,0

    # Initialize FPR
    li      t0, (5 << 11)
    csrw    mstatus, t0

    fscsr   x0
    fmv.s.x f0, x0
    fmv.s.x f1, x0
    fmv.s.x f2, x0
    fmv.s.x f3, x0
    fmv.s.x f4, x0
    fmv.s.x f5, x0
    fmv.s.x f6, x0
    fmv.s.x f7, x0
    fmv.s.x f8, x0
    fmv.s.x f9, x0
    fmv.s.x f10,x0
    fmv.s.x f11,x0
    fmv.s.x f12,x0
    fmv.s.x f13,x0
    fmv.s.x f14,x0
    fmv.s.x f15,x0
    fmv.s.x f16,x0
    fmv.s.x f17,x0
    fmv.s.x f18,x0
    fmv.s.x f19,x0
    fmv.s.x f20,x0
    fmv.s.x f21,x0
    fmv.s.x f22,x0
    fmv.s.x f23,x0
    fmv.s.x f24,x0
    fmv.s.x f25,x0
    fmv.s.x f26,x0
    fmv.s.x f27,x0
    fmv.s.x f28,x0
    fmv.s.x f29,x0
    fmv.s.x f30,x0
    fmv.s.x f31,x0    

    # Read hardware thread id
    csrr    t0, mhartid
    # Only run hart0, halt the others
    bnez    t0, halt

    # Initialize stack pointer
    la      sp, stack_top
    # Initialize global pointer
    la      gp, global_pointer
    
    # Setup SATP, Sv39
    la      t0, pt_start
    srli    t0, t0, 12
    li      t1, (0x8 << 60)
    or      t0, t0, t1
    csrrw   x0, satp, t0

    # Setup PMP
    li      x31, 0xffffffffffffffff
    csrw    0x3b0, x31
    li      x31, 0xf
    csrw    0x3a0, x31

    # Switch_to_supervisor
    la      t0, enter_main
    csrw    mepc, t0
    mret

    # Jump to C main
enter_main:
    jal     main

    # For simulation env
    li t5, 0x900d900d900d900d
    li t6, 0x407ffe0
    li a0, 1
write_tohost:
    sw a0, tohost, t5
    sw a0, 0(t6)
    j write_tohost

    # Enter the infinite loop
halt:
    wfi
    j       halt

    # For simulation env
.align 6
.global tohost
tohost: .dword 0
.align 6
.global fromhost
fromhost: .dword 0


