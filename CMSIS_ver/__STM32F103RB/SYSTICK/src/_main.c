#include "SysTick.h"

GPIO_InitTypeDef led;

int main(void)
{
    Init_SysTick();
	  
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
