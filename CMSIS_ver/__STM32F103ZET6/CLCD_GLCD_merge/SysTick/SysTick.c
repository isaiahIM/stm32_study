#include "SysTick.h"
volatile uint32_t __DelayCount, __DelayActive=0;

void SysTick_Init()
{
    SysTick_Config(SystemCoreClock/1000);// 1ms Interrupt
    
}

void Delay_ms(uint32_t time_ms)
{
    SysTick->VAL=0;// Current value reset
    
    __DelayActive=1;
    __DelayCount=time_ms;
    
    while(__DelayCount!=0);
    
    __DelayActive=0;
}


/*Private define(helper function)*/
void SysTick_Handler(void)
{
    if(__DelayActive==1)
    {
        __DelayCount--;
    }
}

