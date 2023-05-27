#include <stm32f10x.h>
#include <stdio.h>
#include "TIMER2.h"
#include "MPU6050.h"
#include "lpfilter.h"
#include "LCD1.h"
#include "systick.h"

extern uint8_t mode;
extern uint16_t step_count;
int8_t sign_accZ = -1;
double old_accZ_in_g = 1;
uint8_t blink = 0;

void TIM2_Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
    TIM2->PSC = 3599;
    TIM2->ARR = 399;                    			// interrup one second
    TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; 	//enable auto reload
		TIM2->EGR |= TIM_EGR_UG;  								// create an update event
		TIM2->DIER |= TIM_DIER_UIE;         			// enable TIM2 interrupt
		NVIC_SetPriority(TIM2_IRQn, 2);
		NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;  				// Clear the update interrupt flag
				
				if (mode == 0) {					
						double accZ_in_g = MPU_read_Acc_total();
						accZ_in_g = low_pass_filter(accZ_in_g);
						if((sign_accZ > 0) && (accZ_in_g < old_accZ_in_g) && (old_accZ_in_g > 1.5)) {
								step_count++;									
								LCD_1602A_SetCursor(0, 0);
								char buf[17];
								sprintf(buf, "%d", step_count);
								LCD_1602A_WriteString(buf);
						}
						sign_accZ = accZ_in_g > old_accZ_in_g ? 1 : -1;
						old_accZ_in_g = accZ_in_g;
						
						if (blink == 50) {
								GPIOC->ODR ^= GPIO_ODR_ODR13;	
								blink = 0;
						} else {
								blink++;
						}
				}
				
    }
}
