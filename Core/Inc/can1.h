#ifndef __CAN1_H
#define __CAN1_H

#include "stm32f4xx.h"

void CAN1_Init(void);
void CAN1_Transmit(uint16_t speed_val);
void CAN1_Enable_RX0_Interrupt(void);
#endif