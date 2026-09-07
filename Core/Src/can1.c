#include "stm32f4xx.h"
#include "can1.h"

void CAN1_Init(void) {
    /* 1. Activation des horloges pour GPIOA et CAN1 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    /* 2. Configuration des broches PA11 (CAN1_RX) et PA12 (CAN1_TX) */
    GPIOA->MODER &= ~(GPIO_MODER_MODER11 | GPIO_MODER_MODER12) ;
    GPIOA->MODER |=  ((2 << (22)) | (2 << (24))); 

    /* AF9 pour PA11 et PA12 (CAN1) */
    GPIOA->AFR[1] &= ~((0xF << ((11 - 8) * 4)) | (0xF << ((12 - 8) * 4)));
    GPIOA->AFR[1] |=  ((9  << ((11 - 8) * 4)) | (9  << ((12 - 8) * 4)));

    /* 3. Entrer en mode d'initialisation CAN */
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0); // Attendre l'entrée en mode Init

    CAN1->MCR &= ~CAN_MCR_SLEEP; // Sortir du mode Sleep

    /* 4. Configuration du Baud Rate à 250 kbit/s (@ APB1 = 16 MHz)
       Tq = (BRP+1) / fCK = 4 / 16MHz = 0.25 us
       Nominal Bit Time = 1 + TS1 + TS2 = 1 + 12 + 3 = 16 Tq
       Baud Rate = 1 / (16 * 0.25 us) = 250 kbit/s */
    CAN1->BTR = (1U << 30) |              // Mode Loupback de test (Mettre 0U << 24 pour Mode Normal )
                ((3U - 1U) << 20) |       // TS2 = 3 Tq
                ((12U - 1U) << 16) |      // TS1 = 12 Tq
                ((4U - 1U) << 0);         // Prescaler (BRP) = 4

    /* 5. Sortir du mode d'initialisation (Passer en mode Normal) */
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) != 0);

    /* 6. Configuration simple des Filtres (Tout accepter dans FIFO 0) */
    CAN1->FMR |= CAN_FMR_FINIT;           // Mode Init filtre
    CAN1->FA1R &= ~(1U << 0);             // Désactiver filtre 0
    CAN1->FS1R |= (1U << 0);              // Registre 32 bits
    CAN1->FM1R &= ~(1U << 0);             // Mode masque
    CAN1->sFilterRegister[0].FR1 = 0x00000000;    // ID = 0
    CAN1->sFilterRegister[0].FR2 = 0x00000000;    // Masque = 0 (Accepte tous les ID)
    CAN1->FA1R |= (1U << 0);              // Activer filtre 0
    CAN1->FMR &= ~CAN_FMR_FINIT;          // Sortir mode Init filtre
		
	}



void CAN1_Transmit(uint16_t speed_val) {
    /* 1. Vérifier si la Mailbox 0 est libre */
    if ((CAN1->TSR & CAN_TSR_TME0) != 0) {
        
        /* 2. Configurer l'ID Standard (ex: 0x123) */
        CAN1->sTxMailBox[0].TIR = (0x123 << 21); // Standard ID sur bits [31:21]

        /* 3. Définir la longueur de la donnée (2 Octets pour uint16_t) */
        CAN1->sTxMailBox[0].TDTR = (2 & 0x0F);

        /* 4. Placer la valeur speed (16 bits) dans les données */
        CAN1->sTxMailBox[0].TDLR = (uint32_t)speed_val; // Octets 0 et 1

        /* 5. Demander la transmission */
        CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    }
}


void CAN1_Enable_RX0_Interrupt(void){
		/* Enable FIFO 0 Message Pending Interrupt */
CAN1->IER |= CAN_IER_FMPIE0;

/* Enable CAN1 RX0 IRQ in NVIC */
NVIC_EnableIRQ(CAN1_RX0_IRQn);
}
