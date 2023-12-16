##===================================================================================================================
## Makefile -- wrapper for tup to make the assembler executable
##
##	Other targets will be added here to perform other functions.
##
##      Date     Tracker  Version  Description
##  -----------  -------  -------  ---------------------------------------------------------------------------------
##  2023-Feb-28  Initial  v0.0.1   Initial Version
##===================================================================================================================


all:
	tup

run: all
	qemu-system-aarch64 -M raspi3b -serial stdio -kernel bin/kernel8.img
