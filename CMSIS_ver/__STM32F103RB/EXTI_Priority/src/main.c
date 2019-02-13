#include "stm32f10x.h"

GPIO_InitTypeDef led;
GPIO_InitTypeDef btn;
GPIO_InitTypeDef btn2;

EXTI_InitTypeDef btnInterrupt;
NVIC_InitTypeDef btnInterruptEvent;

EXTI_InitTypeDef btnInterrupt2;
NVIC_InitTypeDef btn2InterruptEvent;


volatile unsigned int i, j;

int main(void)
{  
/*RCC CLOCK Setting*/
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);// GPIOA CLOCK ENABLE
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN, ENABLE);// GPIOC CLOCK ENABLE
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);// AFIO CLOCK ENABLE


/*GPIO Setting*/
    /*LED PA8, OUTPUT PUSHPULL, 50MHz*/
    led.GPIO_Pin=GPIO_Pin_8;
    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &led);
	
    /*button PC1, INPUT_PULL-UP*/
    btn.GPIO_Pin=GPIO_Pin_1;
    btn.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &btn);

    /*buttonn2 PC13, INPUT_PULL_UP*/
    btn2.GPIO_Pin=GPIO_Pin_13;
    btn2.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &btn2);

    
/*Interrupt Setting*/
    /*btn1- Interrupt falling, enavle, interruptMode, line is 1(PORTx.1)*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);// Interrupt Pinaching
    btnInterrupt.EXTI_Line=EXTI_Line1;
    btnInterrupt.EXTI_Mode=EXTI_Mode_Interrupt;
    btnInterrupt.EXTI_Trigger=EXTI_Trigger_Falling;
    btnInterrupt.EXTI_LineCmd=ENABLE;
    EXTI_Init(&btnInterrupt);

    /*btn2- Interrupt falling, enavle, interruptMode, line is 13(PORTx.13)*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);// Interrupt Pin maching
    btnInterrupt2.EXTI_Line=EXTI_Line13;
    btnInterrupt2.EXTI_Mode=EXTI_Mode_Interrupt;
    btnInterrupt2.EXTI_Trigger=EXTI_Trigger_Falling;
    btnInterrupt2.EXTI_LineCmd=ENABLE;
    EXTI_Init(&btnInterrupt2);
    

/*NVIC Setting*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// Proirity group is 2
    /*
 The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
    */
    /*btn- Pre-motion Priority:1, sub Priority:0, Interrupt Vect: EXTI1*/
    btnInterruptEvent.NVIC_IRQChannel=EXTI1_IRQn;
    btnInterruptEvent.NVIC_IRQChannelPreemptionPriority=1;// pre-emotionn priority is 1
    btnInterruptEvent.NVIC_IRQChannelSubPriority=0;// sub priority is 0
    btnInterruptEvent.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&btnInterruptEvent);

    /*btn2- Pre-motion Priority:0, sub Priority:0, Interrupt Vect: EXTI15_10*/
    btn2InterruptEvent.NVIC_IRQChannel=EXTI15_10_IRQn;
    btn2InterruptEvent.NVIC_IRQChannelPreemptionPriority=0;
    btn2InterruptEvent.NVIC_IRQChannelSubPriority=0;
    btn2InterruptEvent.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&btn2InterruptEvent);
    
    GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_SET);

    while(1)
    {
    }
		//return 0;
}

/*button interrupt event*/
void EXTI1_IRQHandler(void)
{
    GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_RESET);
    for(i=0;i<100;i++)
    for(j=0;j<60000;j++);

    GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_SET);
    
    EXTI_ClearITPendingBit(btnInterrupt.EXTI_Line);
}

void EXTI15_10_IRQHandler(void)
{
    uint32_t extiActiveState;
    if(EXTI_GetFlagStatus(EXTI_Line13)==SET)//  if(EXTI->PR&(0X01<<13) ) << Check what Interrupt Active
    {
        GPIO_WriteBit(GPIOA, led.GPIO_Pin, Bit_SET);
    }
    EXTI_ClearITPendingBit(btnInterrupt2.EXTI_Line);
}

