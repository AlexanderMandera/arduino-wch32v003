/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#if !defined(Arduino_h)
#define Arduino_h

#define SYSTEM_CORE_CLOCK 48000000
#define APB_CLOCK SYSTEM_CORE_CLOCK
#define SYSTICK_USE_HCLK

#include "api/ArduinoAPI.h"
#include <pins_arduino.h>
#include "ch32v003fun.h"

#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs
#define abs(x) ((x)>0?(x):-(x))

#define interrupts() PFIC_EnableAllIRQ()
#define noInterrupts() PFIC_DisableAllIRQ()

// AVR compatibility macros...naughty and accesses the HW directly
#define digitalPinToPort(pin)       (0)
#define digitalPinToBitMask(pin)    (1UL << (pin))
#define digitalPinToTimer(pin)      (0)
#define digitalPinToInterrupt(pin)  (pin)
#define NOT_AN_INTERRUPT            (-1)
#define portOutputRegister(port)    ((volatile uint32_t*) sio_hw->gpio_out)
#define portInputRegister(port)     ((volatile uint32_t*) sio_hw->gpio_in)
#define portModeRegister(port)      ((volatile uint32_t*) sio_hw->gpio_oe)

// We provide analogReadResolution and analogWriteResolution APIs
void analogReadResolution(int bits);
void analogWriteResolution(int bits);

#endif