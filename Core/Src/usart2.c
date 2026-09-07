#include "stm32f4xx.h"
#include "usart2.h"


void strTransmit (const char * str , uint8_t size ){
/*check NULL pointers */ 
	if (NULL != str){
		/*send all string characters */
		for (int idx =0 ; idx <size ; idx++){
		
		/*check USART status register*/
			while (!(USART2 ->SR & USART_SR_TXE)){
				/*wait for transmission buffer empty flag*/
		}
		
		
		/* write data into transmit data register*/
		USART2 ->DR = str[idx];
	}
}
else {
/* null pointers , do nothing */
}
} 



/** 
* @brief configure 	GPIO with AF7 , USART2 CONNECTED to APB1 with clock (fck)= 45 MHz 
* @note  PA2 --> USART2_TX , PA3 --> USART2_RX
*/
	
void USART2_GPIO_Config ( void){
/* Enable Port A  clock */
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
/* Select alternate function mode */
	GPIOA ->MODER &= ~ (GPIO_MODER_MODER2 | GPIO_MODER_MODER3 );
	GPIOA ->MODER |= (2 << 4 | 2 << 6 );

	
/*select output type push-pull for TX ( PA2)*/
 GPIOA -> OTYPER &= ~ (GPIO_OTYPER_OT2);
	
/* select ouptput speed medium for TX ( PA2) */
GPIOA -> OSPEEDR &= ~(3 << 4);
GPIOA -> OSPEEDR |= (1 << 4);

	
/* select pull-up */
	GPIOA ->PUPDR &= ~ (GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3 );
	GPIOA ->PUPDR |= (1 << 4) | (1 << 6);


/* select AF7 */
  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
  GPIOA ->AFR[0] |= (7<<8) | (7<< 12);
}



void USART2_Init(void){
/* Enable USART2 clock */
RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	
/* select Oversampling by 16 mode */
USART2 ->CR1 &= ~(USART_CR1_OVER8);

/* select One sample bit method */
USART2 ->CR3 |= ( USART_CR3_ONEBIT);

/* select 1 start bit , 9 data bits , n stop bits */
USART2 ->CR1 |= USART_CR1_M;

/* select 1 stop bit */
USART2 ->CR2 &= ~(USART_CR2_STOP);

/* enable parity control */
USART2 ->CR1 |= USART_CR1_PCE;

/* select odd parity */
USART2 ->CR1 |= USART_CR1_PS;

/* set baud_rate = 115200 Bps
  * USARTDIV = fck / 16 * baud_rate
	           = 16000000 / (16 * 115200 ) = 8.68
	
	* DIV_Fraction = 16 * 0.68 = 10.88 = 11 = 0xB
	* DIV_Mantissa = 8 = 0x8
	
	*BRR = 0x8B   */
USART2 -> BRR = 0x8B;
}




void USART2_Enable (void){
/* Enable USART2*/
  USART2 ->CR1 |= USART_CR1_UE;
	
/* Enable TX */ 
	USART2 ->CR1 |= USART_CR1_TE;

/* Enable RX */ 
	USART2 ->CR1 |= USART_CR1_RE;

/* Enable reception buffer not empty flag interrpt */
	USART2 ->CR1 |= USART_CR1_RXNEIE;
	
/* Enable Parity error interrupt */ 
	USART2 ->CR1 |= USART_CR1_PEIE;

}