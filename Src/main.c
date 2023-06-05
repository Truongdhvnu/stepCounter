#include <stm32f10x.h>
#include <math.h>
#include <stdio.h>
#include "I2C1.h"
#include "MPU6050.h"
#include "LCD1.h"
#include "systick.h"
#include "TIMER2.h"

#define BUTTON_PAUSE (1u<<6)
#define BUTTON_RESET (1u<<7)
#define LED_RED_PIN (1u<<5)

void GPIO_Init(void);
void Init(void);

uint8_t mode = 0;
uint32_t step_count = 0;

int main(void) {
		Init();
		while(1) {
		}
}

void GPIO_Config(void) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
		
		//Init LED_GREEN at PC13	  
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
		
		//Init LED_RED at PA5
		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= GPIO_CRL_MODE5_0;
	
		//Init for BUTTON_PAUSE pin
		GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);  // clear bits
		GPIOA->CRL |= GPIO_CRL_CNF6_1;  									// input push-pull
		GPIOA->ODR |= GPIO_ODR_ODR6;  										// PA6 is in Pull UP mode
		//Interrupt configyre for BUTTON_PAUSE
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI6_PA;					// Interrupt at port A
		EXTI->IMR |= BUTTON_PAUSE; 												// Disable Int mask
		EXTI->FTSR |= BUTTON_PAUSE;												// Trigger at Falling Edge
		
		//Init for BUTTON_RESET pin
		GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);  // clear bits
		GPIOA->CRL |= GPIO_CRL_CNF7_1;  									// input push-pull
		GPIOA->ODR |= GPIO_ODR_ODR7;  										// PA6 is in Pull UP mode
		//Interrupt configyre for BUTTON_PAUSE
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PA;					// Interrupt at port A
		EXTI->IMR |= BUTTON_RESET; 												// Disable Int mask
		EXTI->FTSR |= BUTTON_RESET;												// Trigger at Falling Edge
		
		//Enable Interrupt
		NVIC_SetPriority(EXTI9_5_IRQn, 3);								
		NVIC_EnableIRQ (EXTI9_5_IRQn);		
}

void EXTI9_5_IRQHandler(void) {	
	if (EXTI->PR & BUTTON_PAUSE) {
			if (mode == 0) {
					mode = 1;
					GPIOC->ODR |= GPIO_ODR_ODR13;  // noi trc voi nguon
					GPIOA->ODR |= LED_RED_PIN;			// noi truoc voi dat
			} else {
					GPIOA->ODR &= ~LED_RED_PIN;
					GPIOC->BRR &= GPIO_ODR_ODR13;
					mode = 0;
			}
			delay(200);
			EXTI->PR |= BUTTON_PAUSE;
	} else if (EXTI->PR & BUTTON_RESET) {
					step_count = 0;
					LCD_1602A_SetCursor(0, 0);
					LCD_1602A_ClearScrean();
					LCD_1602A_WriteString("Steps: 0");
			delay(200);
			EXTI->PR |= BUTTON_RESET;
	}
}

void Init() {
		GPIO_Config();
		SysTick_Init();
		I2C_Init();
		MPU6050_init();				//MPU6050 va LCD can cac ham cua I2C va Systick nen can Init theo thu tu
		LCD_1602A_Init();
		TIM2_Config();				//Trong TIM2 dung cac ngoai vi tren nen can Init sau cung
		char buf[17];
		sprintf(buf, "%d", step_count);
		LCD_1602A_SetCursor(0, 0);
		LCD_1602A_WriteString("Steps: 0");
}
