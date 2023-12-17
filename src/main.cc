//===================================================================================================================
//  kernel_main.cc -- This is the kernel entry point
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-15  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "kernel.h"



//
// -- some local variables to track boot progress
//    -------------------------------------------
static volatile bool core0Booted = false;
static volatile bool core1Booted = false;
static volatile bool core2Booted = false;
static volatile bool core3Booted = false;



//
// -- The kernel high level entry point; it an undecorated functions name thish does not return
//    -----------------------------------------------------------------------------------------
extern "C"
__attribute__((noreturn))
void Core0Main(void)
{
    extern uint8_t *_start1;
    extern uint8_t *_start2;
    extern uint8_t *_start3;

    uint64_t volatile *spin_cpu1 = (uint64_t *)0xe0;
    uint64_t volatile *spin_cpu2 = (uint64_t *)0xe8;
    uint64_t volatile *spin_cpu3 = (uint64_t *)0xf0;

    UartInit();

    UartPutS("Welcome to the RPi3 Logic Analyzer Device\n");
    extern uint8_t *_start1;
    extern uint8_t *_start2;
    extern uint8_t *_start3;


    UartPutS("Initializing...\n");
    MmioWrite(GPIO_GPFSEL0, 0);
    MmioWrite(GPIO_GPFSEL1, 0);             // -- need to be careful here as the UART in on pins 14/15
    MmioWrite(GPIO_GPFSEL2, 0);             // -- included in these pins are 2 additional external trigger pins


    // -- mark that core0 is booted
    core0Booted = true;

    // -- boot core 1 & wait for confirmation
    UartPutS(".. Starting core 1 ");

    // -- the compiler is going to complain about this, but I know it is OK; disable the warning for this line
#pragma GCC diagnostic ignored "-Warray-bounds"
    *spin_cpu1 = (uint64_t)&_start1;
#pragma GCC diagnostic warning "-Warray-bounds"

    asm volatile("sev");
    while (!core1Booted) {}

    // -- boot core 2 & wait for confirmation
    UartPutS(".. Starting core 2 ");

    // -- the compiler is going to complain about this, but I know it is OK; disable the warning for this line
#pragma GCC diagnostic ignored "-Warray-bounds"
    *spin_cpu2 = (uint64_t)&_start2;
#pragma GCC diagnostic warning "-Warray-bounds"

    while (!core2Booted) {}

    // -- boot core 3 & wait for confirmation
    UartPutS(".. Starting core 3 ");

    // -- the compiler is going to complain about this, but I know it is OK; disable the warning for this line
#pragma GCC diagnostic ignored "-Warray-bounds"
    *spin_cpu3 = (uint64_t)&_start3;
#pragma GCC diagnostic warning "-Warray-bounds"

    while (!core3Booted) {}

    while (true) {
        UartPutC(UartGetC());
    }
}



//
// -- Core 1 main entry point; does not return
//    ----------------------------------------
extern "C"
__attribute__((noreturn))
void Core1Main(void)
{
    UartPutS("   core 1 booted\n");
    core1Booted = true;
    while (true) {}
}



//
// -- Core 2 main entry point; does not return
//    ----------------------------------------
extern "C"
__attribute__((noreturn))
void Core2Main(void)
{
    UartPutS("   core 2 booted\n");
    core2Booted = true;
    while (true) {}
}



//
// -- Core 3 main entry point; does not return
//    ----------------------------------------
extern "C"
__attribute__((noreturn))
void Core3Main(void)
{
    UartPutS("   core 3 booted\n");
    core3Booted = true;
    while (true) {}
}


