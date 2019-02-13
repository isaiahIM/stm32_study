void USART_BaseConfig(USART_TypeDef *USARTx, uint16_t bps, uint16_t USART_WordLength, uint16_t USART_Mode, uint16_t USART_Parity, uint16_t USART_StopBits)
{
	USART_InitTypeDef serial;
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
