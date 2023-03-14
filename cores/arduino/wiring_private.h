#pragma once

#include "Arduino.h"

static uint8_t gpioForPin(pin_size_t pin);
static GPIO_TypeDef* gpioRegister(uint8_t gpio);
static uint8_t gpioPin(uint8_t gpio, pin_size_t pin);