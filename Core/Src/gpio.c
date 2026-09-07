#include "stm32f4xx.h"
#include "gpio.h"

void gpio_init(void){
	
 /*Enable GPIOA and GPIOB and GPIOC */
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN ;

 /*Configure PA5 as Output */
 GPIOA->MODER &= ~(GPIO_MODER_MODER5);
 GPIOA->MODER |= (1 << 10);	
	
 /*Configure PB0, PB5, PB6, PB7, PB8 as Inputs , configure PB1, PB2, PB3, PB4, PB9 as Outputs */
 GPIOB -> MODER &= ~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 |GPIO_MODER_MODER3 |GPIO_MODER_MODER4 |GPIO_MODER_MODER5 |GPIO_MODER_MODER6 | GPIO_MODER_MODER7 |GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
 GPIOB -> MODER |= (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 18);
	
	/*PB0 as Input with Internal Pull-up*/
 GPIOB -> PUPDR &= ~(GPIO_PUPDR_PUPD0);
 GPIOB -> PUPDR |= (1 << 0);
	/*PB5, PB6, PB7, PB8 as Inputs with Internal Pull-down*/
 GPIOB -> PUPDR &= ~ (GPIO_PUPDR_PUPD5 |GPIO_PUPDR_PUPD6 |GPIO_PUPDR_PUPD7 |GPIO_PUPDR_PUPD8 );
 GPIOB -> PUPDR |= (2 << 10) | (2 << 12) | (2 << 14) | (2 << 16);
 /*Configure PC13 as Input */
 GPIOC->MODER &= ~(GPIO_MODER_MODER13);
}