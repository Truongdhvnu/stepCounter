#include <stm32f10x.h>
#include "systick.h"

uint32_t usTick = 0;
uint32_t msTick = 0;

void SysTick_Init(void) {
		SysTick->LOAD |= SystemCoreClock / 1000 - 1;
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT;
		SysTick->CTRL |= SysTick_CTRL_ENABLE;
		NVIC_SetPriority(SysTick_IRQn, 1);
		NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void) {
		SysTick->CTRL |= SysTick_CTRL_COUNTFLAG;
		msTick++;
}

uint32_t millis(void) {
		return msTick;
}

void delay(uint32_t ms) {
		ms += millis();
		while(millis() < ms);
}

void delay_us(uint32_t us) {
		uint32_t ms = us / 1000;
		if(ms == 0) {
				us = SysTick->VAL - 72 * us;
				while(SysTick->VAL > us);
		} else {
				uint32_t tick = millis() + ms;
				us = us % 1000;
				while(millis() < tick);
				us = SysTick->VAL - 72 * us;
				while(SysTick->VAL > us);
		}
}
