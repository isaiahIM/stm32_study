#include "stm32f10x.h"
#include<stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
 
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
 
  return ch;
}

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;
GPIO_InitTypeDef led;

USART_InitTypeDef serial1;

int main(void)
{
/*RCC Clock Init*/
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN, ENABLE);

/*NVIC Init*/
    NVIC_EnableIRQ(USART1_IRQn);


/*GPIO INIT*/
    uart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    uart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &uart_rx);

    uart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    uart_tx.GPIO_Pin=GPIO_Pin_9;
    uart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart_tx);

    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);
    
/*USART Init*/
    serial1.USART_BaudRate=9600;
    serial1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    serial1.USART_Parity=USART_Parity_No;
    serial1.USART_StopBits=USART_StopBits_1;
    serial1.USART_WordLength=USART_WordLength_8b;
    serial1.USART_Mode=(USART_Mode_Rx | USART_Mode_Tx);
    USART_Init(USART1, &serial1);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
		
    GPIO_SetBits(GPIOA, led.GPIO_Pin);
    
		while(1)
    {
    }
    
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) )
    {
        if(USART_ReceiveData(USART1) == '1')
        {
            GPIO_SetBits(GPIOA, led.GPIO_Pin);
            printf("%s", "LED OFF\n");
        }
        else if(USART_ReceiveData(USART1) == '0')
        {
            GPIO_ResetBits(GPIOA, led.GPIO_Pin);
            USART_SendData(USART1, 'f');
						printf("%s", "LED ON\n");
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}




