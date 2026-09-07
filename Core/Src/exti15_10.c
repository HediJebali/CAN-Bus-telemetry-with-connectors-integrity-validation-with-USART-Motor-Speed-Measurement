#include "stm32f4xx.h"
#include "exti0.h"
void exti15_10_init(void){

	/*enable SYSCFG clock*/
  RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /*Map EXTI13 line to GPIOC (PC13)*/	
	SYSCFG -> EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13);
	SYSCFG -> EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	/*Configure Falling Edge Trigger*/
	EXTI -> FTSR |= EXTI_FTSR_TR13;
	
	/*Unmask EXTI0 interrupt*/
	EXTI -> IMR |= EXTI_IMR_MR13;
	
	/*Enable EXTI0 interrupt in NVIC*/
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	

	
}
