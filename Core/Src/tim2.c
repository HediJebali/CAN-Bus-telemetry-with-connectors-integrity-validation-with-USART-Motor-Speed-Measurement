#include "stm32f4xx.h"
#include "tim2.h"

void tim2_init(void){
   /*Enable TIM2 clock*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	 /*Set Prescaler to scale 16 MHz down to 1 kHz (1 ms ticks)*/
    TIM2->PSC = 16000 - 1;

    /*Set Auto-reload to 1000 ticks = 1000 ms = 1 second*/
    TIM2->ARR = 1000 - 1;

    /*Enable Update Interrupt*/
    TIM2->DIER |= TIM_DIER_UIE;

    /*Enable TIM2 Interrupt in NVIC*/
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);

    /*Start Timer*/
    TIM2->CR1 |= TIM_CR1_CEN;
}
