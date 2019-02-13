#include "stm32f10x.h"

GPIO_InitTypeDef ledPin;
TIM_TimeBaseInitTypeDef timer1;
volatile int toggleState=1;

int main()
{
/*RCC CLOCK INIT*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
/*NVIC INIT*/
    NVIC_EnableIRQ(TIM1_UP_IRQn);

/*GPIO INIT*/
    ledPin.GPIO_Pin=GPIO_Pin_8;
    ledPin.GPIO_Mode=GPIO_Mode_Out_PP;// GPIO OUTPUT PUSH-PULL
    ledPin.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &ledPin);

/*TIMER 1 INIT*/
    timer1.TIM_ClockDivision=0;// no use clock division
    timer1.TIM_CounterMode=TIM_CounterMode_Up;// up counter
    timer1.TIM_Prescaler=(SystemCoreClock/10000)-1;// 100us COUNT
    timer1.TIM_Period=9999;// 1s ovf
    TIM_TimeBaseInit(TIM1, &timer1);// TIMER base init;
    TIM_Cmd(TIM1, ENABLE);// TIMER count enable
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);// TIMER update Interrupt enable
    
    GPIO_ResetBits(GPIOA, ledPin.GPIO_Pin);
    while(1)
    {}
}

void TIM1_UP_IRQHandler(void)
{
    if(toggleState==1)
    {
        GPIO_SetBits(GPIOA, ledPin.GPIO_Pin);
        toggleState=0;
    }
    else
    {
        GPIO_ResetBits(GPIOA, ledPin.GPIO_Pin);
        toggleState=1;
    }

   TIM_ClearITPendingBit(TIM1, TIM_IT_Update);// timer pending clear
}



