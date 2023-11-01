/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "ch32v003fun.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYSTICK_SR_CNTIF (1<<0)
#define SYSTICK_CTLR_STE (1<<0)
#define SYSTICK_CTLR_STIE (1<<1)
#define SYSTICK_CTLR_STCLK (1<<2)
#define SYSTICK_CTLR_STRE (1<<3)
#define SYSTICK_CTLR_SWIE (1<<31)

volatile uint32_t systick_counter;

unsigned long millis()
{
	return systick_counter;
}

unsigned long micros()
{
	uint32_t currentSysTick = systick_counter;
	uint32_t ticksDifference = (SysTick->CNT + DELAY_MS_TIME) - SysTick->CMP;

	return currentSysTick * 1000u + ticksDifference / DELAY_US_TIME;
}

void delay(unsigned long ms)
{
	Delay_Ms(ms);
}

void delayMicroseconds(unsigned int us)
{
	Delay_Us(us);
}

void init( void )
{
	SystemInit48HSI();

	SysTick->CTLR = 0;
	NVIC_EnableIRQ(SysTicK_IRQn);

	SysTick->CMP = DELAY_MS_TIME - 1;
	SysTick->CNT = 0;
	systick_counter = 0;

	SysTick->CTLR = SYSTICK_CTLR_STE | SYSTICK_CTLR_STIE |
					SYSTICK_CTLR_STCLK;
}

void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void)
{
	SysTick->CMP += DELAY_MS_TIME;
	SysTick->SR = 0;
	systick_counter++;
}

#ifdef __cplusplus
}
#endif