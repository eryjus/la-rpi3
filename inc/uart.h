//===================================================================================================================
//  uart.h -- These are the functions to interact with the UART device
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-15  Initial  v0.0.1   Initial version
//===================================================================================================================



#pragma once



//
// -- These function prototypes interact with the UART serial port
//    ------------------------------------------------------------
void UartPutC(unsigned char c);             // write a single character to the UART
unsigned char UartGetC(void);               // read a single character from the UART
void UartPutS(const char *str);             // write a NULL terminated string to the UART
void UartPutBin(const unsigned char *mem, size_t len);     // write a block of data to the UART

void UartInit(void);                        // initialize the UART to suit our specific needs

