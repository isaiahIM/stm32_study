#ifndef USART__H
#define USART__H

#include "stm32f10x.h"

#define USART_NUM_1 0
#define USART_NUM_2 1
#define USART_NUM_3 2

void USART_BaseConfig(int USASRT_NUMx, USART_TypeDef *USARTx, uint16_t bps, uint16_t USART_WordLength, uint16_t USART_Mode, uint16_t USART_Parity, uint16_t USART_StopBits);
void USART_InterruptConfig(USART_TypeDef *USARTx, uint16_t USART_IT);

#endif
