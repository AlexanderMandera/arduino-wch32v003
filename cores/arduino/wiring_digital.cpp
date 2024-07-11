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
int analogRead(uint8_t pin)
{
    /*
        GPIO -> ADC Channel
        PA2 -> A0
        PA1 -> A1
        PC4 -> A2
        PD2 -> A3
        PD3 -> A4
        PD5 -> A5
        PD6 -> A6
        PD4 -> A7
        ADC_CH_REF -> A8
        ADC_CH_CAL -> A9
    */
    static uint8_t last = 0xff;
    if (last != pin)
    {
        uint32_t peripheral = 0;
        uint8_t pin_index = 0;   // Pin num
        uint8_t adc_channel = 0; // Channel
        GPIO_TypeDef *gpio;
        switch (pin)
        {
        case A2: // A0
            peripheral = RCC_APB2Periph_GPIOA;
            gpio = GPIOA;
            pin_index = 2;
            adc_channel = 0;
            break;

        case A1: // A1
            peripheral = RCC_APB2Periph_GPIOA;
            gpio = GPIOA;
            pin_index = 1;
            adc_channel = 1;
            break;

        case C4: // A2
            peripheral = RCC_APB2Periph_GPIOC;
            gpio = GPIOC;
            pin_index = 4;
            adc_channel = 2;
            break;

        case D2: // A3
            peripheral = RCC_APB2Periph_GPIOD;
            gpio = GPIOD;
            pin_index = 2;
            adc_channel = 3;
            break;

        case D3: // A4
            peripheral = RCC_APB2Periph_GPIOD;
            gpio = GPIOD;
            pin_index = 3;
            adc_channel = 4;
            break;

        case D5: // A5
            peripheral = RCC_APB2Periph_GPIOD;
            gpio = GPIOD;
            pin_index = 5;
            adc_channel = 5;
            break;

        case D6: // A6
            peripheral = RCC_APB2Periph_GPIOD;
            gpio = GPIOD;
            pin_index = 6;
            adc_channel = 6;
            break;

        case D4: // A7
            peripheral = RCC_APB2Periph_GPIOD;
            gpio = GPIOD;
            pin_index = 4;
            adc_channel = 7;
            break;

        case ADC_CH_REF: // A8
            peripheral = 0;
            gpio = NULL;
            pin_index = 8;
            adc_channel = 8;
            break;

        case ADC_CH_CAL: // A9
            peripheral = 0;
            gpio = NULL;
            pin_index = 9;
            adc_channel = 9;
            break;
        default:
            return 0;
            break;
        }

        // ADCCLK = 24 MHz => RCC_ADCPRE = 0: divide by 2
        RCC->CFGR0 &= ~(0x1F << 11);
        RCC->APB2PCENR |= peripheral | RCC_APB2Periph_ADC1;

        // PXi is analog input chl 7
        if (gpio)
            gpio->CFGLR &= ~(0xf << (4 * pin_index)); // CNF = 00: Analog, MODE = 00: Input

        // Reset the ADC to init all regs
        RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
        RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;

        // Set up single conversion on chl 7
        ADC1->RSQR1 = 0;
        ADC1->RSQR2 = 0;
        ADC1->RSQR3 = adc_channel; // A0通道

        // set sampling time for chl 7
        ADC1->SAMPTR2 &= ~(ADC_SMP0 << (3 * 7));
        ADC1->SAMPTR2 |= 7 << (3 * 7); // 0:7 => 3/9/15/30/43/57/73/241 cycles

        // turn on ADC and set rule group to sw trig
        ADC1->CTLR2 |= ADC_ADON | ADC_EXTSEL;

        // Reset calibration
        ADC1->CTLR2 |= ADC_RSTCAL;
        while (ADC1->CTLR2 & ADC_RSTCAL)
            ;

        // Calibrate
        ADC1->CTLR2 |= ADC_CAL;
        while (ADC1->CTLR2 & ADC_CAL)
            ;
        last = pin;
    }

    // should be ready for SW conversion now

    ADC1->CTLR2 |= ADC_SWSTART;

    // wait for conversion complete
    while (!(ADC1->STATR & ADC_EOC))
        ;

    // get result
    return ADC1->RDATAR;
}