#include "stm32f10x.h"

GPIO_InitTypeDef led;

volatile uint32_t __DelayCount, __DelayActive=0;

void Delay(uint32_t time_ms);

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);// 1ms Interrupt
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/*GPIO Init*/
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);

    while(1)
    {
        GPIO_ResetBits(GPIOA, led.GPIO_Pin);
        Delay(1000);
        GPIO_SetBits(GPIOA, led.GPIO_Pin);
        Delay(1000);
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


