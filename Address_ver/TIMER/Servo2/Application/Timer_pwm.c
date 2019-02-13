
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


/*Define:*/
#define SERVO1_PIN (1<<0)// PB.0: SERVO1

#define SERVO_MIN 70
#define SERVO_MAX 250

#define RCC_RESET (0X00)
#define RCC_AFIO_ENABLE (1<<0)
#define RCC_GPIOB_ENABLE (1<<3)
#define RCC_TIMER1_ENABLE (1<<11)
#define RCC_TIMER2_ENABLE (1<<0)

#define TIMER_CNT_ENABLE 0X01
#define TIMER_UPDATE_INT_ENABLE 0X01
#define TIMER_CC1_ENABLE 0X01
#define TIMER_CC1_INT_ENABLE (0X01<<1)
#define TIMER_CC1_MODE_OUTPUT 0X00
#define TIMER_CC1_MODE_PWM1 (0X06<<4)

#define NVIC_TIMER1_UPDATE_ENABLE (0X01<<25)
#define NVIC_TIMER1_CAP_COMP_ENABLE (0X01<<27)
#define NVIC_TIM2_ENABLE (0X01<<28)

/*
Address:
RCC: 0X40021000
GPOIB: 0X40010C00
TIMER1: 0X40012C00
TIMER2: 0X40000000
EXTI: 0X40010400

Interrupt Vector table:
EXTI0: 6
EXTI1: 7
TIMER1_UPDATE: 25
TIMER1_CC: 27
TIMER2: 28
NVIC: 0xE000E100
*/

volatile unsigned long g_time_ms=0;
/*Functions:*/
void Init_GPIO(void)
{
	(*(volatile unsigned int*)0X40021018)=RCC_RESET;// RCC->APB2ENR=RCC_RESET;
	(*(volatile unsigned int*)0X40021018)|=RCC_GPIOB_ENABLE;// RCC->APB2ENR|=RCC_GPIOB_ENABLE;
	(*(volatile unsigned int*)0X40010C00)|=0X03;// GPIOB->CRL|=0x03; << PORTB.0 INIT(OUTPUT, PUSH-PULL)
}

void Init_Timer(void)
{
/*TIMER1*/// (*(volatile unsigned int*))
	/*BASE INIT*/
	(*(volatile unsigned int*)0X40021018)|=RCC_TIMER1_ENABLE;// RCC->APB2ENR|=RCC_TIMER1_ENABLE;
	
	(*(volatile unsigned int*)0X40012C28)=0X2CF;// TIM1->PSC=0X2CF; << Pre-Scale is 700
	(*(volatile unsigned int*)0x40012C2C)=0X7CF;// TIM1->ARR=0X7CF; << Update interrupt by 2000count
	(*(volatile unsigned int*)0X40012C00)|=TIMER_CNT_ENABLE;// TIM1->CR1|=TIMER_CNT_ENABLE; << count enable
	
	/*UPDATE INIT*/
	(*(volatile unsigned int*)0xE000E100)|=NVIC_TIMER1_UPDATE_ENABLE;// NVIC->ISER[0]|=NVIC_TIMER1_UPDATE_ENABLE;
	(*(volatile unsigned int*)0X40012C0C)|=TIMER_UPDATE_INT_ENABLE;// TIM1->DIER|=TIMER_UPDATE_INT_ENABLE;
	
	/*Capture_Compare INIT*/
	(*(volatile unsigned int*)0X40012C20)|=TIMER_CC1_ENABLE;// TIM1->CCER|=TIMER_CC1_ENABLE;
	(*(volatile unsigned int*)0X40012C18)|=TIMER_CC1_MODE_OUTPUT;// TIM1->CCMR1|=TIMER_CC1_MODE_OUTPUT;
	(*(volatile unsigned int*)0X40012C18)|=TIMER_CC1_MODE_PWM1;// TIM1->CCMR1|=TIMER_CC1_MODE_PWM1;
	(*(volatile unsigned int*)0X40012C34)=SERVO_MIN;
	
	(*(volatile unsigned int*)0X40012C0C)|=TIMER_CC1_INT_ENABLE;// TIM1->DIER|=TIMER_CC1_INT_ENABLE;
	(*(volatile unsigned int*)0xE000E100)|=NVIC_TIMER1_CAP_COMP_ENABLE;// NVIC->ISER[0]|=NVIC_TIMER1_CAP_COMP_ENABLE;
	
	
	
/*TIMER2*/
	(*(volatile unsigned int*)0X4002101C)|=RCC_TIMER2_ENABLE;// RCC->APB1ENR|=RCC_TIMER2_ENABLE;
	(*(volatile unsigned int*)0X40000000)|=TIMER_CNT_ENABLE;// TIM2->CR1|=TIMER_CNT_ENABLE;
	(*(volatile unsigned int*)0X40000028)=0X1C1F;// TIM2->PSC=0X1C1F; << PRE-SCALE IS 7200
	(*(volatile unsigned int*)0X40000028)=0X09;// TIM2->ARR=0X09; << UPDATE COUNT IS 9
	(*(volatile unsigned int*)0X4000000C)|=TIMER_UPDATE_INT_ENABLE;// TIM2->DIER|=TIMER_UPDATE_INT_ENABLE;
	(*(volatile unsigned int*)0xE000E100)|=NVIC_TIM2_ENABLE;// NVIC->ISER[0]|=NVIC_TIM2_ENABLE;

	(*(volatile unsigned int*)0x40012C30)=(0X00);// TIM1->RCR=(0X00); << TIMER1 Repetition clear
	(*(volatile unsigned int*)0x40000030)=(0X00);// TIM2->RCR=(0X00); << TIMER2 Repetition clear
}

void SetServoPos(int angle)
{
	(*(volatile unsigned int*)0X40012C34)=angle+70;
}

unsigned long GetMillis(void)
{
	return g_time_ms;
}

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	unsigned long curTime, prevTime=0;
	int servoPos=0, servoVelocity=1;
	
  stm32_Init ();
	Init_GPIO();
	Init_Timer();
	
		while(1)
		{
			curTime=GetMillis();
			
			if(curTime-prevTime>=1)
			{
				prevTime=curTime;
				if(servoPos>=180 || servoPos<0)
				{
					servoVelocity*=-1;
				}
				servoPos+=servoVelocity;
			}
			SetServoPos(servoPos);
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/

void TIM1_UP_IRQHandler(void)// servo cycle
{
	if( ((*(volatile unsigned int*)0X40012C10)&(0x01))!=0 )// TIM1->SR&=0x01;
	{
		(*(volatile unsigned int*)0X40010C0C)|=0x01;// GPIOB->ODR|=0X01;
		
		(*(volatile unsigned int*)0X40012C10)&=~(0X01);
	}
}

void TIM1_CC_IRQHandler(void)// duty
{
	if( ((*(volatile unsigned int*)0X40012C10)&(0x01<<1))!=0 )// TIM1->SR&=(0x01<<1);
	{
		(*(volatile unsigned int*)0X40010C0C)&=~0x01;// GPIOB->ODR&=~(0X01);
		(*(volatile unsigned int*)0X40012C10)&=~(0X01<<1);
	}
}

void TIM2_IRQHandler(void)// millis
{
	if( ((*(volatile unsigned int*)0X40000010)&0X01)!=0 )// TIM2->SR&=0X01;
	{
		g_time_ms+=1;
		(*(volatile unsigned int*)0X40000010)&=~(0X01);
	}
}