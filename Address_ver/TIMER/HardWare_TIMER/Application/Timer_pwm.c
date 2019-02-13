
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


unsigned int count=0;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2

/*
TIM3:
CH:
PA6
PA7
PB0 
PB1
REMAP:
PC6~9

TIM3_CH3

Address:
RCC: 0x4002 1000
GPOIB: 0X40010C00
TIM3: 0x40000400
(*(volatile unsigned int*)
*/
void Init_Port(void)
{
	(*(volatile unsigned int*)0X40021018)=(0X01<<3);// RCC->APB2ENR=(0X01<<3); << GPIO.B CLOCK Enable
	(*(volatile unsigned int*)0X4002101C)=(0X01<<1);// RCC->PAB1ENR=(0X01<<1); <<  TIMER3 CLOCK Enable
	(*(volatile unsigned int*)0X40010C00)=0X0B;// GPIOB->CRL=X00B; << PORTB.0 OUTPUT, ALTERNATE PUSH-PULL
}

void Init_HW_Timer()
{
	(*(volatile unsigned int*)0X4002101C)=(0X01<<1);// RCC->APB1ENR=(0X01<<1); << TIMER3 Clock Enable
	(*(volatile unsigned int*)0x40000428)=7199;// TIM3->PSC=7999; << TIMER3 Pre-Scale is 7200
	(*(volatile unsigned int*)0x4000042C)=9999;// TIM3->CR1=0X01; << TIMER3 Update at 10000
	(*(volatile unsigned int*)0x40000400)|=(0X01<<7);// TIM3->CR1=(0X01<<7); << TIMER3 ARPE ENABLE
	(*(volatile unsigned int*)0x40000400)|=0X01;// TIM3->CR1=0X01; << TIMER3 COUNT ENABLE
	
	(*(volatile unsigned int*)0x4000043C)=4999;// TIM3->CCR3=4999; << TIMER3 Capture/Compare is 5000
	(*(volatile unsigned int*)0x4000041C)|=(0X01<<3);// TIM3->CCMR2|=(0X01<<3); << TIMER3 	PreLoad Enable
	(*(volatile unsigned int*)0x4000041C)|=(0X06<<4);// TIM3->CCMR2|=(0X06<<4); << TIMER3 	FastPWM mode
	(*(volatile unsigned int*)0x40000420)|=(0X01<<8);// TIM3->CCER|=(0X01<<8); << TIMER3 Capture/Compare3 Enable
}

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
	
	Init_Port();
	Init_HW_Timer();
for(;;) {
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
