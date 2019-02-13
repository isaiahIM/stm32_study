#include "stm32f10x.h"

GPIO_InitTypeDef led;
GPIO_InitTypeDef btn;
EXTI_InitTypeDef btnInterrupt;
NVIC_InitTypeDef btnInterruptEvent;

volatile int ledToggle=0;

int main(void)
{  
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);// GPIOA CLOCK ENABLE
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN, ENABLE);// GPIOC CLOCK ENABLE
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);// AFIO CLOCK ENABLE

    /*LED PA8, OUTPUT PUSHPULL, 50MHz*/
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);
	
    /*button PC2, INPUT_PULL-UP*/
    btn.GPIO_Pin=GPIO_Pin_1;
    btn.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &btn);

    /*Interrupt falling, enavle, interruptMode, line is 2(PORTx.2)*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    btnInterrupt.EXTI_Line=EXTI_Line1;
    btnInterrupt.EXTI_Mode=EXTI_Mode_Interrupt;
    btnInterrupt.EXTI_Trigger=EXTI_Trigger_Falling;
    btnInterrupt.EXTI_LineCmd=ENABLE;
    EXTI_Init(&btnInterrupt);

    btnInterruptEvent.NVIC_IRQChannel=EXTI1_IRQn;
    btnInterruptEvent.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&btnInterruptEvent);


    while(1)
    {
    }
		//return 0;
}

/*button interrupt event*/
void EXTI1_IRQHandler(void)
{
    if(ledToggle==1)
    {
        GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_RESET);
    }
	
    ledToggle++;
    ledToggle%=2;
    
    EXTI_ClearITPendingBit(btnInterrupt.EXTI_Line);
}

