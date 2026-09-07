#ifndef USART2_H
#define USART2_H
#include "stdint.h"
#include "stddef.h"


void strTransmit (const char * str , uint8_t size );
void USART2_GPIO_Config ( void);
void USART2_Init(void);
void USART2_Enable (void);



#endif