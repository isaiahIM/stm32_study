
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

unsigned int count=0;
volatile unsigned int g_delayCount=0;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2

/*
Systick: 0xE000E010
*/
void Init_Port(void)
{
	(*(volatile unsigned int*)0X40021018)|=(0X01<<3);
	(*(volatile unsigned int*)0X40010C00)=(0X03<<28);
}
void Init_Systick(void)
{
	(*(volatile unsigned int*)0xE000E014)|=0X1193F;// SysTick->LOAD=0X1193F; << 72000 count Interrupts(72MHz clock)
	(*(volatile unsigned int*)0xE000E010)|=0X07;// SysTick->CTRL=0X07 << ClockSource(72mHz), Initerrupt, Counter Enable
}
void Delay_Decrement(void)
{
	g_delayCount--;
}
void Delay(unsigned int time)
{
	g_delayCount=time;
	
	while(g_delayCount>0);
}

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init (); 
	Init_Port();
	Init_Systick();

  for(;;) {
		GPIOB->ODR|=(0X01<<7);
		Delay(100);
		GPIOB->ODR&=~(0X01<<7);
		Delay(100);
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/

void TIM1_UP_IRQHandler(void)
{
	(*(volatile unsigned int*)0X40012C10)&=~(0X01);// stm32_Init()에서 TIM1_OVF_INT 생성후 무한루프 방지.
}


void SysTick_Handler(void)// 1ms Systick
 {
	Delay_Decrement();
	/*	
	if(count++ == 1000){
		count=0;
		ledLight3 = ~ledLight3;
		if( ledLight3 )
	    	GPIOB->ODR &= ~(0x01 << 4);                           // switch on LED
		else
	    	GPIOB->ODR |=  (0x01 << 4); 
	}
	*/		
		                          // switch off LED
 }