//===================================================================================================================
//  boot.S -- This is the entry point for all cores into the Logic Analyzer
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-14  Initial  v0.0.1   Initial version
//===================================================================================================================


// -- This section is mapped to the first portion of the binary
.section    ".text.boot"


// -- make the start global
.global     _start0
.global     _start1
.global     _start2
.global     _start3


// -- set the starting point for this code
.org        0x80000


//
// -- Entry point for the kernel
//    x0 -> 32 bit pointer to DTB (primary core only; 0 otherwise)
//    x1 -> 0
//    x2 -> 0
//    x3 -> 0
//    x4 -> 32 bit kernel entry point, _start location
//    ------------------------------------------------------------
_start0:
    // -- set stack before this code
    ldr     x5,=_start0
    mov     sp,x5


    // -- clear the bss
    ldr     x5,=__bss_start                     // load from memory into a register
    ldr     w6,=__bss_size                      // load from memory into a register
1:  cbz     w6,2f                               // if 0, jump to the kernel load
    str     xzr,[x5],#8                         // store 0 into memory at x5
    sub     w6,w6,#1                            // decrement w6
    cbnz    w6,1b                               // if non-zero, repeat

    // -- jump to the C code, should never return
2:  b       Core0Main                           // direct jump to kernel_main

    // -- just in case, halt this core
halt:
    wfe                                         // wait for event



//
// -- This is the entry point for core 1
//    ----------------------------------
_start1:
    mov     x5,#0x70000
    mov     sp,x5

    b       Core1Main
    b       halt



//
// -- This is the entry point for core 2
//    ----------------------------------
_start2:
    mov     x5,#0x60000
    mov     sp,x5

    b       Core2Main
    b       halt



//
// -- This is the entry point for core 3
//    ----------------------------------
_start3:
    mov     x5,#0x50000
    mov     sp,x5

    b       Core3Main
    b       halt



