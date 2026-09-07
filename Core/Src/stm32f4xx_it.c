/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "string.h"
#include "usart2.h"
#include "can1.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ENCODER_SLOTS 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */ 
volatile uint32_t pulse_count = 0;
volatile uint16_t speed = 0;  
char buffer[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
void EXTI15_10_IRQHandler(void) {
    /* Check if EXTI line 13 triggered the interrupt */
    if (EXTI->PR & EXTI_PR_PR13) {

        /* ------------------ CONNECTOR 1 TEST ------------------ */
        // Reset output pins first (PB1..PB4)
        GPIOB->BSRR = (GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3 | GPIO_BSRR_BR4 );

        // Step 1: Test PB1 -> PB5 link
        GPIOB->BSRR = GPIO_BSRR_BS1;                    // Set PB1 High
        __NOP(); __NOP();                               // Stabilization delay
        uint8_t pb5_state = (GPIOB->IDR >> 5) & 1U;
        GPIOB->BSRR = GPIO_BSRR_BR1;                    // Reset PB1 Low

        // Step 2: Test PB2 -> PB6 link
        GPIOB->BSRR = GPIO_BSRR_BS2;                    // Set PB2 High
        __NOP(); __NOP();                               // Stabilization delay
        uint8_t pb6_state = (GPIOB->IDR >> 6) & 1U;
        GPIOB->BSRR = GPIO_BSRR_BR2;                    // Reset PB2 Low

        // Evaluate Connector 1
        if (pb5_state && pb6_state) {
            char msg[] = "CONNECTOR 1 connection test is VALID\r\n\r\n\r\n\r\n\r\n\r\n";
            strTransmit(msg, (uint8_t)(sizeof(msg) - 1));
        } else {
            char msg[] = "CONNECTOR 1 connection test is INVALID\r\n\r\n\r\n\r\n\r\n\r\n";
            strTransmit(msg, (uint8_t)(sizeof(msg) - 1));
        }

        /* ------------------ CONNECTOR 2 TEST ------------------ */
        // Step 3: Test PB3 -> PB7 link
        GPIOB->BSRR = GPIO_BSRR_BS3;                    // Set PB3 High
        __NOP(); __NOP();                               // Stabilization delay
        uint8_t pb7_state = (GPIOB->IDR >> 7) & 1U;
        GPIOB->BSRR = GPIO_BSRR_BR3;                    // Reset PB3 Low

        // Step 4: Test PB4 -> PB8 link
        GPIOB->BSRR = GPIO_BSRR_BS4;                    // Set PB4 High
        __NOP(); __NOP();                               // Stabilization delay
        uint8_t pb8_state = (GPIOB->IDR >> 8) & 1U;
        GPIOB->BSRR = GPIO_BSRR_BR4;                    // Reset PB4 Low

        // Evaluate Connector 2
        if (pb7_state && pb8_state) {
            char msg[] = "CONNECTOR 2 connection test is VALID\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
            strTransmit(msg, (uint8_t)(sizeof(msg) - 1));
        } else {
            char msg[] = "CONNECTOR 2 connection test is INVALID\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
            strTransmit(msg, (uint8_t)(sizeof(msg) - 1));
        }

        /* Clear interrupt flag by writing 1 to PR13 */
        EXTI->PR = EXTI_PR_PR13;
    }
}
			


void EXTI0_IRQHandler(void){

/* Check if EXTI line 0 triggered the interrupt*/
    if (EXTI->PR & EXTI_PR_PR0) {
        
			  /* Increment count on every sensor trigger*/
			  pulse_count ++;
        /* Toggle onboard LED PA5 on every pulse detection**/
        GPIOA->ODR ^= (1 << 5);

        /*Clear interrupt flag by writing 1 to PR0*/
        EXTI->PR = EXTI_PR_PR0;
		}

}

void TIM2_IRQHandler(void){
	if (TIM2->SR & TIM_SR_UIF) {

	/*Calculate speed in tr/min using pulses collected over 1 sec*/
speed = (pulse_count * 60) / ENCODER_SLOTS;
		
	/*Envoi par USART2 */
		int len = snprintf(buffer, sizeof(buffer), "USART2 Transmitter :     Speed =  %u tr/min\r\n",speed);
  strTransmit(buffer, (uint8_t)len);
		
	/* Envoi par CAN1 */
	 CAN1_Transmit(speed);
		
		/* Envoi un signal vers le PLC pour le comptage d'implutions*/
	GPIOB->ODR ^= GPIO_ODR_OD9;	
		
  /*Reset pulse counter for the next 1-second interval*/
	pulse_count = 0;
	
	/*Clear timer interrupt flag*/
        TIM2->SR &= ~TIM_SR_UIF;
	}
}


void CAN1_RX0_IRQHandler(void) {
    /* Check if a message is pending in FIFO 0 */
    if ((CAN1->RF0R & CAN_RF0R_FMP0) != 0) {
        
        /* Read the received speed payload (Lower 16 bits of Data Low Register) */
        uint16_t rx_speed = (uint16_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFFFF);

        /* Print verification to HTerm via USART2 */
        char log_buffer[64];
			int len = snprintf(log_buffer, sizeof(log_buffer), "[CAN1 Reciever LOOPBACK]: Speed = %u tr/min\r\n\r\n\r\n", rx_speed);
        strTransmit(log_buffer, (uint8_t)len);

        /* Release FIFO 0 mailbox */
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}
/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
