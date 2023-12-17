# Raspberry Pi Logic Analyzer

This repository is the software for a Logic Analyzer built on the Raspberry Pi 3 platform.


## License

See [LICENSE.md](LICENSE.md).


---

## Build System

I use `tup` as my primary build system.  I usually will wrap `tup` in `make` commands.  You can find `tup` [here](https://gittup.org/tup/).  I simply find `tup` to more reliable detect changed sources with less work.


---

## Theory of Operation

This device is built on a Raspberry Pi 3b -- a 64-bit 4-core ARMv8 CPU.  This is a bare-metal type device which has no base operating system on which it runs.  Therefore, all of the functionality is contained in this boot image.

The Raspberry Pi 3B comes with a 40-pin GPIO header, several of which are available for logic signals.  Specifically, the following pins are found on this header:
* 2 X 3v3 power
* 2 X 5v power
* 8 X Ground
* 28 X GPIO pins


The GPIO pins are numbered from 0-27.  GPIO pins 14 and 15 are used for serial interfacing.  Pins 9-11 also appear to be used for SPI interfacing, which will likely be of interest to the output display.  I also expect that there will be 4 buttons of interest that will be hooked into the GPIO pins.  From the initial 28 pins available, 9 are earmarked for other operations -- leaving up to 19 pins for Logic use.  I expect to use 16 of these.

That said, all the pins states can be read with a single MMIO read operation, returning a single 32-bit value.  The pins of interest can be masked off from the ones of non-interest and a 32-bit value can be sent to the host system.


### Core 0

Core 0 is responsible for booting the system and performing the fundamental initialization.  It will output a welcome message, which will be on the LCD display.  This core will be responsible for handling all user interface output.  3 pins will be needed for handling the LCD output.


### Core 1

Core 1 is responsible for all user interface push-button input.  It will poll the button GPIO pins and make the necessary updates to software configuration options.  4 push-buttons (and GPIO pins) will be used for the interface: up, down, enter, escape.  Messaging will be handed off to Core 0 for display to the users.


### Core 2

Core 2 handles polling all the GPIO pins for logic states.  These will be fed into a circular (ring) buffer.  Since the reads need to happen as quickly as possible, not much processing will happen with these values -- just read the state and store it in the buffer and wait for the next time-slice or trigger to pass.  If the buffer fills, data collection will stop.


### Core 3

Core 3 will be responsible for reading the circular buffer, masking the relevant bits, and then passing the data to the host system.  2 pins are used for the serial communication.

---

It should be noted that each core has a dedicated function and there is no operating system overhead.  Reasonably good performance should be expected.

Of the 28 available pins, 9 are going to be used for other things.  This will leave 19 available for polling logic.  16 will be used for actual logic, keeping the width a multiple of 8.

The remaining 3 pins may be able to be selected for external triggering.


Here are the pins with their intended uses:

| GPIO Pin | Designated Use     |
|:--------:|:-------------------|
| GPIO  0  | Logic Pin  0       |
| GPIO  1  | Logic Pin  1       |
| GPIO  2  | Logic Pin  2       |
| GPIO  3  | Logic Pin  3       |
| GPIO  4  | Logic Pin  4       |
| GPIO  5  | Logic Pin  5       |
| GPIO  6  | Logic Pin  6       |
| GPIO  7  | Logic Pin  7       |
| GPIO  8  | Logic Pin  8       |
| GPIO  9  | Logic Pin  9       |
| GPIO 10  | Display MOSI       |
| GPIO 11  | Display MISO       |
| GPIO 12  | Display SCLK       |
| GPIO 13  | External Trigger 1 |
| GPIO 14  | UART TxD           |
| GPIO 15  | UART RxD           |
| GPIO 16  | Push-Button Up     |
| GPIO 17  | Push-Button Down   |
| GPIO 18  | Push-Button Enter  |
| GPIO 19  | Push-Button Escape |
| GPIO 20  | Logic Pin 10       |
| GPIO 21  | Logic Pin 11       |
| GPIO 22  | Logic Pin 12       |
| GPIO 23  | Logic Pin 13       |
| GPIO 24  | Logic Pin 14       |
| GPIO 25  | Logic Pin 15       |
| GPIO 26  | External Trigger 2 |
| GPIO 27  | External Trigger 3 |






### Memory

Memory is split by the binary loaded at boot.  The binary is loaded into address 0x80000 and top of memory is at 0x40000000.  Peripeherals are loaded at 0x3f000000, right below the memory limit.

Stacks will be set at 0x70000, 0x60000, 0x50000, and 0x40000 for each core.  This should be far more than needed.

Memory from 0x00000000 and 0x0003fffff will be for additional scratch memory if required.

Memory from the end of the binary up to 0x3f000000 will be used for the ring buffer.


