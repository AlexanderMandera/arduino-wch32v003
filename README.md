# Arduino Core for CH32V003

This is a new Arduino Core for the CH32V003 series of microcontrollers.

It is based on the work from [CNLohr/ch32v003fun](https://github.com/CNLohr/ch32v003fun).


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

## Installation

Use the following board manager URL under Preferences &rarr; Additional Board Manager URLs to install the core:

https://alexandermandera.github.io/arduino-wch32v003/package_ch32v003_index.json

Install the "WCH Boards" platform inside the Board Manager.

For Windows users that use the WCH-LinkE programmers, follow the additional steps in the [Wiki](https://github.com/AlexanderMandera/arduino-wch32v003/wiki/Additional-Installation-Steps).

## State of Development

This core is currently in development and is not prepared to handle
every aspect that Arduino Core provides. Several peripheral implementations
are work-in-progress but examples from `ch32v003fun` can always be used inside the Arduino IDE.

All major operating systems (Windows, Linux, macOS) for most CPU architectures (amd64, arm64, armhf)
should be supported.

The version of the Arduino Board Manager package will be updated after major additions or changes
to the Core which will correspond to the `master` branch.
Pre-release or untested changes are made inside the `develop` branch.

**What does work:**
* `delay`/`delayMicroseconds`
* `millis()` / `micros()` (untested)
* Digital IO: `pinMode`/`digitalWrite`/`digitalRead`
* USART implementation
* I2C Implementation (w/o Wire implementation)

**Work in Progress:**
* I2C `Wire.h` implementation

**To be Done:**
* Optimization
* Several peripherals (ADC, SPI...)
* and much more...

## Licensing and Credits

Thanks to the following projects that make this project possible:

* [`ArduinoCore-API`](https://github.com/arduino/ArduinoCore-API) by the Arduino Team is licensed under the LGPL-2.1 License
* [`CH32V003 SDK`](https://github.com/openwch/ch32v003) by OpenWCH is used as a reference (and for information inside this README).
* [`ch32v003fun`](https://github.com/cnlohr/ch32v003fun) by CNLohr is used for the basic toolchain, bootstrap code and flashing utility.
* [`ArduinoCore-ch58x`](https://github.com/ElectronicCats/arduino-wch58x) by Electronic Cats as a reference on how to create an Arduino Core for WCH platform.
