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

#define SERVO_MIN_PULSE 100
#define SERVO_MAX_PULSE 220


GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;
GPIO_InitTypeDef servo1;

USART_InitTypeDef serial1;

TIM_TimeBaseInitTypeDef servo1_base;
TIM_OCInitTypeDef servo1_oc;

volatile int SYS_readData=0;// system value

int USART_ReadData(int *data);
int GetReadData(void);

int main(void)
{
    
/*RCC Clock Init*/
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1EN, ENABLE);
    
/*NVIC Init*/
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_EnableIRQ(TIM1_CC_IRQn);

/*GPIO INIT*/
    uart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    uart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &uart_rx);

    uart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    uart_tx.GPIO_Pin=GPIO_Pin_9;
    uart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart_tx);

    servo1.GPIO_Mode=GPIO_Mode_Out_PP;
    servo1.GPIO_Pin=GPIO_Pin_8;
    servo1.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &servo1);
    
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


/*TIMER1 Init*/
    servo1_base.TIM_ClockDivision=0;
    servo1_base.TIM_CounterMode=TIM_CounterMode_Up;
    servo1_base.TIM_Prescaler=(SystemCoreClock/100000)-1;// 10us count
    servo1_base.TIM_Period=1999;// 20ms Cycle
    TIM_TimeBaseInit(TIM1, &servo1_base);

    servo1_oc.TIM_OCMode=TIM_OCMode_PWM1;
    servo1_oc.TIM_OutputState=TIM_OutputState_Enable;
    servo1_oc.TIM_Pulse=SERVO_MIN_PULSE;
    servo1_oc.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &servo1_oc);

    TIM_Cmd(TIM1, ENABLE);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

    
    GPIO_SetBits(GPIOA, servo1.GPIO_Pin);
    while(1)
    {
    }
    
}

int USART_ReadData(int *data)
{
  char buffer=USART_ReceiveData(USART1);
  if(buffer=='f')
  {
    return 1;
  }
  
  (*data)*=10;
  (*data)+=(buffer-48);
  return 0;
}

int GetReadData(void)
{
    return SYS_readData;
}

/**********************IRQ HANDLER LINE*****************************/
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        GPIO_SetBits(GPIOA, servo1.GPIO_Pin);
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

void TIM1_CC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
    {
        GPIO_ResetBits(GPIOA, servo1.GPIO_Pin);
    }
     
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}


void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) )
    {

        if(USART_ReadData(&SYS_readData)==1)
        {
            TIM_SetCompare1(TIM1, SYS_readData);
            printf("%d\n", SYS_readData);
            SYS_readData=0;
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}