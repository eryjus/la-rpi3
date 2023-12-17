##===================================================================================================================
## Makefile -- wrapper for tup to make the assembler executable
##
##	Other targets will be added here to perform other functions.
##
##      Date     Tracker  Version  Description
##  -----------  -------  -------  ---------------------------------------------------------------------------------
##  2023-Dec-14  Initial  v0.0.1   Initial Version
##===================================================================================================================


all:
	tup

run: all
	qemu-system-aarch64 -machine raspi3b -serial stdio -kernel bin/kernel8.img

debug: all
	qemu-system-aarch64 -machine raspi3b -serial mon:stdio -kernel bin/kernel8.img -D ./debug.txt -d in_asm,int,exec,op,cpu,cpu_reset,unimp


