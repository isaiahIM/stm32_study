#ifndef USART__H
#define USART__H
#include "stm32f10x.h"
void USART_BaseConfig(USART_TypeDef *USARTx, uint16_t bps, uint16_t USART_WordLength, uint16_t USART_Mode, uint16_t USART_Parity, uint16_t USART_StopBits);
void USART_InterruptConfig(USART_TypeDef *USARTx, uint16_t USART_IT);

#endif
