#include "stm32f10x.h"

GPIO_InitTypeDef servo;
TIM_TimeBaseInitTypeDef tim1_base;
TIM_OCInitTypeDef tim1_oc;

volatile int cycleCount=0, duty=80, dutyRange=10;


int main(void)
{
/*NVIC Init*/
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    
/*RCC Clock enable*/	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);


/*TIMER1 Init*/
    tim1_base.TIM_ClockDivision=0;
    tim1_base.TIM_CounterMode=TIM_CounterMode_Up;
    tim1_base.TIM_Prescaler=(SystemCoreClock/100000)-1;// 10us Count
    tim1_base.TIM_Period=1999;// 20ms Cyclee
    TIM_TimeBaseInit(TIM1, &tim1_base);

    tim1_oc.TIM_OCMode=TIM_OCMode_PWM1;
    tim1_oc.TIM_OCPolarity=TIM_OCPolarity_High;
    tim1_oc.TIM_OutputState=TIM_OutputState_Enable;
    tim1_oc.TIM_Pulse=duty;
    TIM_OC1Init(TIM1, &tim1_oc);// Output Compare data init

    TIM_Cmd(TIM1, ENABLE);// TIMER1 enable
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);// not use H/W TIMER
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);// Capture/Compare enable
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);// update Interrupt enable

    
/*GPIO Init*/
    servo.GPIO_Pin=GPIO_Pin_8;
    servo.GPIO_Mode=GPIO_Mode_Out_PP;
    servo.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &servo);

    
    GPIO_ResetBits(GPIOA, servo.GPIO_Pin);
    while(1)
    {
        
    }
}

void TIM1_UP_IRQHandler(void)
{
    GPIO_SetBits(GPIOA, servo.GPIO_Pin);
    if(duty>=229 || duty<=79)
    {
        dutyRange*=-1;
    }
    
    duty+=dutyRange;
    if(duty>79 && duty<=229)
    {
        TIM_SetCompare1(TIM1, duty);
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

void TIM1_CC_IRQHandler(void)
{
    GPIO_ResetBits(GPIOA, servo.GPIO_Pin);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}
