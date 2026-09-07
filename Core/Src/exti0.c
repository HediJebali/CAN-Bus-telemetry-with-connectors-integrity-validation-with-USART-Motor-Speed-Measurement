#include "stm32f4xx.h"
#include "exti0.h"
void exti0_init(void){

	/*enable SYSCFG clock*/
  RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /*Map EXTI0 line to GPIOB (PB0)*/	
	SYSCFG -> EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0);
	SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
	
	/*Configure Falling Edge Trigger*/
	EXTI -> FTSR |= EXTI_FTSR_TR0;
	
	/*Unmask EXTI0 interrupt*/
	EXTI -> IMR |= EXTI_IMR_MR0;
	
	/*Enable EXTI0 interrupt in NVIC*/
	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	

	
}
