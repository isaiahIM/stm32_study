
#include "stm32f10x.h"

GPIO_InitTypeDef usart1_rx;
GPIO_InitTypeDef usart1_tx;

USART_InitTypeDef serial1;


volatile int toggleState=1;
volatile uint32_t __SYS_delay_decrement;// system value

void Delay(uint32_t time);


int main()
{
/*RCC CLOCK INIT*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

/*GPIO INIT*/
/*
  USART1 PIN : 
  RX:PA10
  TX:PA9
*/
    usart1_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    usart1_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &usart1_rx);

    usart1_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    usart1_tx.GPIO_Pin=GPIO_Pin_9;
    usart1_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,  &usart1_tx);


   
/*USART1 INIT*/
    serial1.USART_BaudRate=9600;// 9600bps
    serial1.USART_StopBits=USART_StopBits_1;// 1bit stop bit
    serial1.USART_WordLength=USART_WordLength_8b;// 1Bbte send
    serial1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;// NO control hardware(no CTS, RTS control)
    serial1.USART_Parity=USART_Parity_No;// No parity control
    serial1.USART_Mode=(USART_Mode_Rx | USART_Mode_Tx);// RX, TX use
    USART_Init(USART1, &serial1);
    USART_Cmd(USART1, ENABLE);

    
/*SYSTICK INIT*/
    SysTick_Config(SystemCoreClock/1000);// 0.1ms Interrupt
    
    while(1)
    {
        USART_SendData(USART1, 'a');
        Delay(100);
    }
}

void Delay(uint32_t time)
{
    __SYS_delay_decrement=time;
    while(__SYS_delay_decrement!=0);
}

void SysTick_Handler(void)
{
    __SYS_delay_decrement--;
    
}




