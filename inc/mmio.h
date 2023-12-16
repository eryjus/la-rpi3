//===================================================================================================================
//  mmio.h -- These are the MMIO address offsets.
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-15  Initial  v0.0.1   Initial version
//===================================================================================================================


#include <stdint.h>
#include <stddef.h>

#pragma once



//
// -- here is the complete collection of MMIO addresses (as needed so far)
//    --------------------------------------------------------------------
enum {
    // -- hard-code the MMIO starting address here -- specifically for rpi3
    MMIO_BASE       = 0x3f000000,


    // -- this is the general offset to the start of the register bank
    GPIO_BASE       = (MMIO_BASE + 0x200000),


    // -- controls the pull up/down for all GPIO pins
    GPPUD           = (GPIO_BASE + 0x94),


    // -- controls pull up/down by spefic pins
    GPPUDCLK0       = (GPIO_BASE + 0x98),


    // -- this is the offset to the start of the UART0 registers
    UART0_BASE      = (GPIO_BASE + 0x1000),


    // -- offsets for each register in the UART
    UART0_DR        = (UART0_BASE + 0x00),
    UART0_RSRECR    = (UART0_BASE + 0x04),
    UART0_FR        = (UART0_BASE + 0x18),
    UART0_ILPR      = (UART0_BASE + 0x20),
    UART0_IBRD      = (UART0_BASE + 0x24),
    UART0_FBRD      = (UART0_BASE + 0x28),
    UART0_LCRH      = (UART0_BASE + 0x2c),
    UART0_CR        = (UART0_BASE + 0x30),
    UART0_IFLS      = (UART0_BASE + 0x34),
    UART0_IMSC      = (UART0_BASE + 0x38),
    UART0_RIS       = (UART0_BASE + 0x3c),
    UART0_MIS       = (UART0_BASE + 0x40),
    UART0_ICR       = (UART0_BASE + 0x44),
    UART0_DMACR     = (UART0_BASE + 0x48),
    UART0_ITCR      = (UART0_BASE + 0x80),
    UART0_ITIP      = (UART0_BASE + 0x84),
    UART0_ITOP      = (UART0_BASE + 0x88),
    UART0_TDR       = (UART0_BASE + 0x8c),


    // -- Mailbox registers
    MBOX_BASE       = (MMIO_BASE + 0xb880),
    MBOX_READ       = (MBOX_BASE + 0x00),
    MBOX_STATUS     = (MBOX_BASE + 0x18),
    MBOX_WRITE      = (MBOX_BASE + 0x20),
};



//
// -- Write to an MMIO Address
//    ------------------------
inline void MmioWrite(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)(uint64_t)reg = data;
}



//
// -- Read from an MMIO Address
//    -------------------------
inline uint32_t MmioRead(uint32_t reg)
{
    return *(volatile uint32_t *)(uint64_t)reg;
}



//
// -- delay so that the compiler will not optimize away
//    -------------------------------------------------
inline void MmioDelay(int count)
{
    asm volatile("__delay_%=: subs %[count],%[count],#1; bne __delay_%=\n"
            : "=r"(count) : [count]"0"(count) : "cc");
}


