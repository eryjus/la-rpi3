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
// -- The kernel high level entry point; it an undecorated functions name thish does not return
//    -----------------------------------------------------------------------------------------
extern "C"
__attribute__((noreturn))
void kernel_main(void)
{
    UartPutS("Welcome to the Rpi3 Logic Analyzer Device\n");
    while (true) {
        UartPutC(UartGetC());
    }
}



