#include "stm32f10x.h"

GPIO_InitTypeDef led;

void Delay(uint32_t time_ms);

volatile uint32_t __DelayCount, __DelayActive=0;

int main()
{
    SysTick_Config(SystemCoreClock/1000);// 1ms Interrupt
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);
    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    IWDG_SetReload(800);// 1second watchDog
    IWDG_ReloadCounter();// Reload counter enable
    IWDG_Enable();// watch Dog enable
    
    while(1)
    {
        GPIO_ResetBits(GPIOA, led.GPIO_Pin);
        Delay(600);
        GPIO_SetBits(GPIOA, led.GPIO_Pin);
        Delay(600);
        IWDG_ReloadCounter();
    }
}

void Delay(uint32_t time_ms)
{
    SysTick->VAL=0;// Current value reset
    
    __DelayActive=1;
    __DelayCount=time_ms;
    
    while(__DelayCount!=0);
    
    __DelayActive=0;
}

void SysTick_Handler(void)
{
    if(__DelayActive==1)
    {
        __DelayCount--;
    }
}

