# Arduino Core for CH32V003

This is a new Arduino Core for the CH32V003 series of microcontrollers.

It utilizes some work from [CNLohr/ch32v003fun](https://github.com/CNLohr/ch32v003fun) and the Arduino CH58x Core.


## Overview
CH32V003 series is based on QingKe RISC-V2A core design of industrial-grade general-purpose microcontroller, support 48MHz system main frequency, with wide voltage, 1-wire serial debug interface, low-power consumption, ultra-small package, etc. CH32V003 series built-in a group of DMA controller, a group of 10-bit ADC, a group of op-amp comparators, multiple timers and standard communication interfaces USART, I2C, SPI, etc.

## System Block Diagram
<img src="https://github.com/openwch/ch32v003/raw/main/image/frame.jpg" alt="frame" style="zoom:50%;" />

### Features
- QingKe 32-bit RISC-V2A processor, supporting 2 levels of interrupt nesting
- Maximum 48MHz system main frequency
- 2KB SRAM, 16KB Flash
- Power supply voltage: 3.3/5V
- Multiple low-power modes: Sleep, Standby
- Power on/off reset, programmable voltage detector
- 1 group of 1-channel general-purpose DMA controller
- 1 group of op-amp comparator
- 1 group of 10-bit ADC
- 1×16-bit advanced-control timer, 1×16-bit general-purpose timer
- 2 WDOG, 1×32-bit SysTick
- 1 USART interface, 1 group of I2C interface, 1 group of SPI interface
- 18 I/O ports, mapping an external interrupt
- 64-bit chip unique ID
- 1-wire serial debug interface (SDI)
- Package: TSSOP20, QFN20, SOP16, SOP8

## State of Development

This core is currently in development and not yet ready for general use.
It is also not prepared for use with the Arduino IDE without installing all requirements manually.
In the future, the core will be available as a package for the Arduino IDE.

It is only tested in Windows for now, but will also be developed for Linux and macOS.

**What does work:**
* `delay`/`delayMicroseconds`
* `millis()` / `micros()` (untested)
* Digital IO: `pinMode`/`digitalWrite`/`digitalRead`
* USART implementation

**Work in Progress:**
* I2C implementation (currently no Wire implementation)

**To be Done:**
* Several peripherals (ADC, SPI...)
* Arduino IDE packaging
* and much more...

## Prerequisites

* [xPack GNU RISC-V Embedded GCC v12.2.0-3](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/)
  * `toolchain.path` in `platform.txt` must be changed if your path is different

## Installation

Clone this repository into the `hardware/wch` folder of your Arduino installation (preferably inside the sketchbook folder for now).

Use `git submodule update --init` to get the `ch32v003fun` submodule.

If you are developing inside this core before compiling any sketch,
use the `copy_sources.bat` or `copy_sources.sh` script inside the `misc` folder to copy the ch32v003 sources.
This script is called on every compilation inside Arduino.

## Licensing and Credits

Thanks to the following projects that make this project possible:

* [`ArduinoCore-API`](https://github.com/arduino/ArduinoCore-API) by the Arduino Team is licensed under the LGPL-2.1 License
* [`CH32V003 SDK`](https://github.com/openwch/ch32v003) by OpenWCH is used as a reference (and for information inside this README).
* [`ch32v003fun`](https://github.com/cnlohr/ch32v003fun) by CNLohr is used for the basic toolchain, bootstrap code and flashing utility.
* [`ArduinoCore-ch58x`](https://github.com/ElectronicCats/arduino-wch58x) by Electronic Cats as a reference on how to create an Arduino Core for WCH platform.