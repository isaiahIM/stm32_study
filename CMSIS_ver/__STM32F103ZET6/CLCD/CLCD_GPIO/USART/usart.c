#include "usart.h"

#define HIGH_DENSITY

#ifdef MIDIUM_DENSITY

#define USART_SIZE 3
GPIO_TypeDef* USART_PORT[USART_SIZE]= {GPIOA, GPIOA, GPIOB};
uint16_t USART_TX_PIN[USART_SIZE]= {GPIO_Pin_9, GPIO_Pin_2, GPIO_Pin_10};
uint16_t USART_RX_PIN[USART_SIZE]= {GPIO_Pin_10, GPIO_Pin_3, GPIO_Pin_11};

#endif


#ifdef HIGH_DENSITY

#define USART_SIZE 1

GPIO_TypeDef* USART_PORT[USART_SIZE]= {GPIOA};
uint16_t USART_TX_PIN[USART_SIZE]= {GPIO_Pin_9};
uint16_t USART_RX_PIN[USART_SIZE]= {GPIO_Pin_10};

#endif

void USART_BaseConfig(int USASRT_NUMx, USART_TypeDef *USARTx, uint16_t bps, uint16_t USART_WordLength, uint16_t USART_Mode, uint16_t USART_Parity, uint16_t USART_StopBits)
{
    GPIO_InitTypeDef rx, tx;
    USART_InitTypeDef serial;
	
    rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    rx.GPIO_Pin=USART_RX_PIN[USASRT_NUMx];
    GPIO_Init(USART_PORT[USASRT_NUMx], &rx);

    tx.GPIO_Mode=GPIO_Mode_AF_PP;
    tx.GPIO_Speed=GPIO_Speed_50MHz;
    tx.GPIO_Pin=USART_TX_PIN[USASRT_NUMx];
    GPIO_Init(USART_PORT[USASRT_NUMx], &tx);
    
    
    serial.USART_BaudRate=bps;
    serial.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    serial.USART_Parity=USART_Parity;
    serial.USART_StopBits=USART_StopBits;
    serial.USART_WordLength=USART_WordLength;
    serial.USART_Mode=USART_Mode;

    USART_Init(USARTx, &serial);
    USART_Cmd(USARTx, ENABLE);
} 

void USART_InterruptConfig(USART_TypeDef *USARTx, uint16_t USART_IT)
{
	USART_ITConfig(USARTx, USART_IT, ENABLE);
}
