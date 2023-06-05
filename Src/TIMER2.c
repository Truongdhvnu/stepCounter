#include <stm32f10x.h>
#include <stdio.h>
#include "TIMER2.h"
#include "MPU6050.h"
#include "lpfilter.h"
#include "LCD1.h"
#include "systick.h"

extern uint8_t mode;
extern uint16_t step_count;
uint8_t blink = 0;

void TIM2_Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
    TIM2->PSC = 3599;
    TIM2->ARR = 399;                    			// interrup 20 ms 72e6/(3.6e3x4e2)
    TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; 	// enable auto reload
		TIM2->EGR |= TIM_EGR_UG;  								// create an update event
		TIM2->DIER |= TIM_DIER_UIE;         			// enable TIM2 interrupt
		NVIC_SetPriority(TIM2_IRQn, 2);
		NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;  				// Clear the update interrupt flag
				
				if (mode == 0) {					
						double acc_in_g = MPU_read_Acc_total();
						acc_in_g = low_pass_filter(acc_in_g);
							if(is_peak() != 0) {
								step_count++;									
								LCD_1602A_SetCursor(0, 0);
								char buf[17];
								LCD_1602A_WriteString("Steps: ");
								sprintf(buf, "%d", step_count);
								LCD_1602A_WriteString(buf);
							}
						
						if (blink == 50) {
								GPIOC->ODR ^= GPIO_ODR_ODR13;	
								blink = 0;
						} else {
								blink++;
						}
				}
				
    }
}
