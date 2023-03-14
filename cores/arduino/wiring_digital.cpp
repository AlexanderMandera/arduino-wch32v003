/*
  wiring_digital.cpp - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2018-2019 Arduino SA

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Arduino.h"
#include "ch32v003fun.h"
#include "wiring_private.h"

static uint8_t gpioForPin(pin_size_t pin)
{
    if (pin < 2) {
        return 0;
    } else if(pin < 10) {
        return 2;
    } else {
        return 3;
    }
}

static GPIO_TypeDef* gpioRegister(uint8_t gpio)
{
    if(gpio == 0) {
        return GPIOA;
    } else if(gpio == 2) {
        return GPIOC;
    } else {
        return GPIOD;
    }
}

static uint8_t gpioPin(uint8_t gpio, pin_size_t pin)
{
    if(gpio == 0) {
        return pin;
    } else if(gpio == 2) {
        return pin - 2;
    } else {
        return pin - 10;
    }
}

void pinMode(pin_size_t pin, PinMode mode)
{
    uint8_t gpio = gpioForPin(pin);
    GPIO_TypeDef* port = gpioRegister(gpio);
    uint8_t p = gpioPin(gpio, pin);

    // Enable GPIO
    RCC->APB2PCENR |= (0x04 << gpio);

    // Configure pin
    uint8_t pinConfig = 0;
    switch (mode) {
        case INPUT:
            pinConfig = GPIO_CNF_IN_FLOATING;
            break;
        case OUTPUT:
            pinConfig = GPIO_Speed_50MHz | GPIO_CNF_OUT_PP;
            break;
        case INPUT_PULLUP:
            pinConfig = GPIO_CNF_IN_PUPD;
            port->BSHR = (((uint32_t)0x01) << p);
            break;
        case INPUT_PULLDOWN:
        default:
            pinConfig = GPIO_CNF_IN_PUPD;
            port->BCR = (((uint32_t)0x01) << p);
            break;
    }

    port->CFGLR &= ~(0xf << (p * 4));
    port->CFGLR |= (pinConfig << (p * 4));
}


void digitalWrite(pin_size_t pin, PinStatus val)
{
    uint8_t gpio = gpioForPin(pin);
    GPIO_TypeDef* port = gpioRegister(gpio);
    uint8_t p = gpioPin(gpio, pin);

    if(val == HIGH) {
        port->BSHR = ((uint32_t)1 << p);
    } else {
        port->BCR = ((uint32_t)1 << p);
    }
}

PinStatus digitalRead(pin_size_t pin)
{
    uint8_t gpio = gpioForPin(pin);
    GPIO_TypeDef* port = gpioRegister(gpio);
    uint8_t p = gpioPin(gpio, pin);

    return (port->INDR & (1 << p)) != 0 ? HIGH : LOW;
}