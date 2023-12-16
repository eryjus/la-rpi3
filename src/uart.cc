//===================================================================================================================
//  uart.cc -- These are the functions to interact with the UART device
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
// -- perform the UART initialization -- this is specific to the Logic Analyzer!
//
//    The UART needs to be placed into a known state.  There may be some things which are initialized properly
//    by default.  However, we cannot rely on that behavior.  Everything must be set up properly just in case.
//    --------------------------------------------------------------------------------------------------------
void UartInit(void)
{
    // -- disable the UART
    MmioWrite(UART0_CR, 0x00000000);

    // -- disable pull up/down for all GPIO pins & wait
    MmioWrite(GPPUD, 0x00000000);
    MmioDelay(150);

    // -- make sure pins 14/15 pull up/down are disabled
    MmioWrite(GPPUDCLK0, (1<<14) | (1<<15));
    MmioDelay(150);

    // -- write 0 to make sure the above changes take effect
    MmioWrite(GPPUDCLK0, 0x00000000);

    // -- clear any pending interrupts
    MmioWrite(UART0_ICR, 0x7ff);

    // -- set up the baud rate: 115200
    //    the rpi3 is system clock dependent, so let's set it to be 3MHz so we can get the baud we want
    //    ---------------------------------------------------------------------------------------------
    volatile unsigned int __attribute__((aligned(16))) mbox[9] = {
        9 * 4,
        0,
        0x38002,
        12,
        8,
        2,
        3000000,                        // -- desired clock frequency?
        0,
        0,
    };

    unsigned int r = (unsigned int)(((((unsigned long)(&mbox) & 0xffffffff)) & ~0xf) | 8);

    // -- wait until we the VC is available
    while (MmioRead(MBOX_STATUS) & 0x80000000) {}

    // -- send our message and wait for a response
    MmioWrite(MBOX_WRITE, r);
    while ((MmioRead(MBOX_STATUS) & 0x40000000) || MmioRead(MBOX_READ) != r) {}


    // -- 3000000 / (16 * 115200) = 1.627; (0.627 * 64) + 0.5 = 40.6
    MmioWrite(UART0_IBRD, 1);
    MmioWrite(UART0_FBRD, 40);


    // -- enable FIFO with 8-N-1
    MmioWrite(UART0_LCRH, (1<<4) | (1<<5) | (1<<6));

    // -- mask all interrupts
    MmioWrite(UART0_IMSC, (1<<1) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8) | (1<<9) | (1<<10));

    // -- Enable UART for Tx and Rx
    MmioWrite(UART0_CR, (1<<0) | (1<<8) | (1<<9));
}


//
// -- write a character to the serial port
//    ------------------------------------
void UartPutC(unsigned char c)
{
    // -- the serial port needs to be available to send
    while (MmioRead(UART0_FR) & (1<<5)) {}
    MmioWrite(UART0_DR, c);
}


//
// -- read a character from the serial port
//    -------------------------------------
unsigned char UartGetC(void)
{
    // -- serial port needs to have something available to read
    while (MmioRead(UART0_FR) & (1<<4)) {}
    return (unsigned char)(MmioRead(UART0_DR) & 0xff);
}


//
// -- write a string to the serial port
//    ---------------------------------
void UartPutS(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i ++) {
        if (str[i] == '\n') UartPutC('\r');
        UartPutC((unsigned char)str[i]);
    }
}


//
// -- write a block of binary data to the serial port
//    -----------------------------------------------
void UartPutBin(const unsigned char *mem, size_t len)
{
    for (size_t i = 0; i < len; i ++) {
        UartPutC(mem[i]);
    }
}

