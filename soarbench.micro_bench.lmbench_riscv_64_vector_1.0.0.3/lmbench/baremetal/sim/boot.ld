ENTRY(_reset)

SECTIONS
{
    . = 0x80000000;
    .text : { *(.text) }
    .data : { *(.data) }
    . = ALIGN(0x100000);
    .rodata : { *(.rodata) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . = . + 0x100000;
    stack_top = .;
    global_pointer = . + (0x1000/2);
    .sdata : { *(.sdata) }
    .sbss : { *(.sbss) }
    .tdata : { *(.tdata) }
    .tbss : { *(.tbss) }
    _end = .;

    PROVIDE (program_start = 0x80000000);
    PROVIDE (program_end = . + 0x10000000);

    PROVIDE (mem_start = 0x90000000);
    PROVIDE (mem_end = . + 0x40000000);
}


