/*
SERVO CONTROL.
SERVO: PA.8
*/
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

unsigned int count=0;

//int ledLight1 = 0;
//int ledLight2 = 0;
//int ledLight3 = 0;
volatile int g_dutyFlag=1;

/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 8 )                // PA8: LED D0
#define LED1             ( 1 << 2 )                // PD2: LED D1
#define SERVO_MIN_COUNT 70
#define SERVO_MAX_COUNT 230

/*
Address Info:
GPIOA: 0x4001 0800
GPIOD: 0x4001 1400
TIMER1: 0x4001 2C00
TIMER2: 0x4000 0000
RCC: 0x4002 1000

CORE:
NVIC:0xE000 EF00

*/
void Init_IO(void)
{
	(*(volatile unsigned int*)0X40021018)=0X00;// RCC->APB2ENR=0X00; << Clear RCC->APB2ENR
	
	(*(volatile unsigned int*)0X40021018)|=0X04;// RCC->APB2ENR|=0X04; << PORT A CLOCK ENABLE
	(*(volatile unsigned int*)0X40021018)|=(0X04<<3);// RCC->APB2ENR=(0X04<<3); << PORT D CLOCK ENABLE
	
	
	(*(volatile unsigned int*)0X40010804)=(0X00);// GPIOA->CRH=0X00; << PA CLEAR
	(*(volatile unsigned int*)0X40011400)=(0X00);// GPIOD->CRL=(0X00); << PD CLEAR
	
	(*(volatile unsigned int*)0X40010804)|=(0X03);// GPIOA->CRH|=(0X03); << PA.8 SETTING(50MHz max clock, push-pull)
	(*(volatile unsigned int*)0X40011400)|=(0X03<<8);// GPIOD->CRL|=(0X03<<(4*2)); << PD2 SETTING(50MHz max clock, push-pull)
}
void Init_Timer(void)
{
/*timer 1*/
	/*Common Set*/
	(*(volatile unsigned int*)0X40021018)|=(0X01<<11);// RCC->APB2ENR|=(0X01<<11); << TIMER1 CLOCK ENABLE
	(*(volatile unsigned int*)0x40012C28)=(0X2CF);// TIM1->PSC=(0x2CF); << TIMER1 Prescale is 720
	(*(volatile unsigned int*)0x40012C2c)=(0X7CF);// TIM1->ARR=(0X7CF); << TIMER1 Interrupt count is 2000(2000번 count되면 업데이트 인터럽트 발생)
	(*(volatile unsigned int*)0x40012C00)|=(0X01);// TIM1->CR1|=(0X01); << TIMER1 COUNTER ENABLE
	
	/*Update Interrupt set*/
	(*(volatile unsigned int*)0x40012C0C)|=(0X01);// TIM1->DIER|=(0X01); << TIMER1 Update Inerrupt Enable
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<25);// NVIC->ISER[0]|=(0X01<<25); << TIMER1 update Interrupt VECTOR enable
	
	/*Capture/Compare Interrupt Set*/
	(*(volatile unsigned int*)0x40012C20)|=(0x01);// TIM1->CCER|=0X01; << TIMER1 Capture/Compare Output Enable
	(*(volatile unsigned int*)0x40012C18)|=(0x00);// TIM1->CCMR1|=0X00; << TIMER1 Capture/Compare Output mode
	(*(volatile unsigned int*)0x40012C18)|=(0x06<<4);// TIM1->CCMR1|=(0X06<<4); << TIMER1 PWM mode 1
	
	(*(volatile unsigned int*)0x40012C34)=SERVO_MIN_COUNT;// TIM1->CCR1=START_DUTY; << Compare number set
	(*(volatile unsigned int*)0x40012C0C)|=(0X01<<1);// TIM1->DIER|=(0X01<<1); << TIMER1 Capture/Compare Inerrupt Enable
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<27);// NVIC->ISER[0]|=(0X01<<27); << TIMER1 Capture/Compare Interrupt enable
	
	
/*timer 2*/
	
	(*(volatile unsigned int*)0X4002101C)=(0X00);// RCC->APB1ENR=(0X00); << RCC->APB1ENR CLEAR
	(*(volatile unsigned int*)0X4002101C)|=(0X01);// RCC->APB1ENR|=(0X01<<1); << TIMER2 CLOCK ENABLE
	
	(*(volatile unsigned int*)0X40000028)=(0X1C1F);// TIM2->PSC=(0X1C1F); << TIMER1 Prescale is 7200
	(*(volatile unsigned int*)0X4000002C)=(0x63);// TIM2->ARR=(0x63); << TIMER2 Interrupt count is 100(100번 count되면 업데이트 인터럽트 발생)
	
	(*(volatile unsigned int*)0x40000000)|=(0X01);// TIM2->CR1|=(0X01); << TIMER2 COUNTER ENABLE
	
	(*(volatile unsigned int*)0x4000000C)=(0X01);// TIM2->DIER|=(0X01); << TIMER2 Update Inerrupt Enable
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<28);// NVIC->ISER[0]|=(0X01<<28); << TIMER2 Interrupt enable
	
	
	
	(*(volatile unsigned int*)0x40012C30)=(0X00);// TIM1->RCR=(0X00); << TIMER1 Repetition clear
	(*(volatile unsigned int*)0x40000030)=(0X00);// TIM2->RCR=(0X00); << TIMER2 Repetition clear
}

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
	Init_IO();
	Init_Timer();
 
for(;;) 
	{
  }
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/

	
	
	/*----------------------------------------------------------*\
 | Timer1 Update Interrupt Handler-
\*----------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) 
	{                 // check interrupt source
		(*(volatile unsigned int*)0X4001080C)|=(0X01<<8);// GPIOA->ODR|=(0X01<<8); << PA.8 HIGH(LED ON)
		
		TIM1->SR &= ~(1<<0);                          // clear UIF flag
	}
} // end TIM1_UP_IRQHandler

/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source
		if( (*(volatile unsigned int*)0X40012C34)>=SERVO_MAX_COUNT || (*(volatile unsigned int*)0X40012C34)<SERVO_MIN_COUNT)// if(TIM1->CCR1>=0X270F || TIM1->CCR1<=0)
		{
			g_dutyFlag*=-1;
		}
		(*(volatile unsigned int*)0X40012C34)+=g_dutyFlag;// TIM1->CCR1+=g_dutyFlag; << TIMER1 Capture/Compare time change
		
	TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM2_UP_IRQHandler


void TIM1_CC_IRQHandler(void)// CAPTURE/COMPARE ISR << MAKE PWM of Duty
{
	if ((TIM1->SR & (0x01<<1)) != 0)// Check Interrupt State
	{
		(*(volatile unsigned int*)0X4001080C)&=~(0X01<<8);// GPIOA->ODR&=~(0X01<<8); << PA.8 HIGH(LED OFF)
		TIM1->SR &= ~(1<<1);                          // clear CC1 flag
	}
}