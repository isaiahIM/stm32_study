#include "stm32f10x.h"

GPIO_InitTypeDef led;
TIM_TimeBaseInitTypeDef timer1_base;
TIM_OCInitTypeDef timer1_oc;

volatile uint32_t __led_toggle=1;

int main(void)
{
/*NVIC Init*/
    NVIC_EnableIRQ(TIM1_CC_IRQn);

/*RCC Clock Init*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
/*GPIO Init*/
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);

/*TIMER1 Init*/
    timer1_base.TIM_ClockDivision=0;
    timer1_base.TIM_CounterMode=TIM_CounterMode_Up;
    timer1_base.TIM_Prescaler=(SystemCoreClock/10000)-1;
    timer1_base.TIM_Period=9999;
    TIM_TimeBaseInit(TIM1, &timer1_base);
    
    timer1_oc.TIM_OCMode=TIM_OCMode_PWM1;
    timer1_oc.TIM_OCPolarity=TIM_OCPolarity_High;
    timer1_oc.TIM_OutputState=TIM_OutputState_Enable;
    timer1_oc.TIM_Pulse=9999;// 1ms capture/compare
    TIM_OC1Init(TIM1, &timer1_oc);
    TIM_Cmd(TIM1, ENABLE);// TIMER1 enable
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);// TIMER1 Capture/Compare Interrupt enable


    GPIO_ResetBits(GPIOA, led.GPIO_Pin);
    while(1)
    {
    }
}


void TIM1_CC_IRQHandler(void)
{
    if(__led_toggle==1)
    {
        GPIO_SetBits(GPIOA, led.GPIO_Pin);
        __led_toggle=0;
    }
    else
    {
        GPIO_ResetBits(GPIOA, led.GPIO_Pin);
        __led_toggle=1;
    }

TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}