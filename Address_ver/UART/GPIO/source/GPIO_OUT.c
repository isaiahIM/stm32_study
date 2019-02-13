
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
volatile unsigned char temp;

void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*
	uart1: 0x40013800
	uart2: 0x40004400
	rcc: 0x40021000
	nvic: 0xE000E100
	gpioa: 0x40010800
	
	pa2: uart2 tx
	pa3: uart2 rx
	
	pa9: uart1 tx
	pa10: uart1 rx
	(*(volatile unsigned int*) )
	*/
	
void Write_UART1(char data)
{
	while(!(USART1->SR & (1<<7)));
	
	(*(volatile unsigned int*)0x40013804)=data;
	
}

char Read_UART1(void)
{
	return (*(volatile unsigned int*)0x40013804);
}

void Write_UART2(char data)
{
	while(!(USART2->SR & (1<<7)));
	
	(*(volatile unsigned int*)0x40004404)=data;
	
}

char Read_UART2(void)
{
	return (*(volatile unsigned int*)0x40004404);
}
	
void Init_UART1(void)
{
	
	(*(volatile unsigned int*)0x40021018)=(0x01<<2);// RCC->APB2ENR=(0X01<<2); << GPIOA CLOCK ENABLE
	(*(volatile unsigned int*)0x40021018)|=(0x01<<14);// RCC->APB2ENR|=(0X01<<14); << USART1 CLOCK ENABLE

	(*(volatile unsigned int*)0x40010804)=(0x04<<8);// GPIOA->CRH=(0X04<<8); << PA.10 floating input
	(*(volatile unsigned int*)0x40010804)|=(0x0B<<4);// GPIOA->CRH|=(0X0B<<4); << PA.9 Alternate function output Push-pull
	
	(*(volatile unsigned int*)0x40013808)=0X1D4C;// USART1->BRR=0X1D4C; << USART1 bps is 9600
	
	(*(volatile unsigned int*)0x4001380C)=(0X01<<2);// USART1->CR1=(0X01<<2); << USART1 RX enable
	(*(volatile unsigned int*)0x4001380C)|=(0x01<<3);// USART1->CR1|=(0X01<<3); << USART1 TX  enable
	
	(*(volatile unsigned int*)0x4001380C)|=(0X01<<13);// USART1->CR1|=(0X01<<13); <<USART1 enable		
}
	
void Init_UART2(void)
{
	(*(volatile unsigned int*)0x4002101C)=(0x01<<17);// RCC->APB1ENR|=(0X01<<17); << USART2 CLOCK ENABLE
	
	(*(volatile unsigned int*)0x40010800)|=(0x04<<12);// GPIOA->CRL=(0X04<<12); << PA.3 floating input
	(*(volatile unsigned int*)0x40010800)|=(0x0B<<8);// GPIOA->CRL|=(0X0B<<8); << PA.2 Alternate function output Push-pull
	
	(*(volatile unsigned int*)0x40004408)=0X0EA6;// USART2->BRR=0X0EA6; << USART2 bps is 9600

	(*(volatile unsigned int*)0x4000440C)=(0X01<<2);// USART2->CR1=(0X01<<2); << USART2 RX enable
	(*(volatile unsigned int*)0x4000440C)|=(0x01<<3);// USART2->CR1|=(0X01<<3); << USART2 TX  enable
	(*(volatile unsigned int*)0x4000440C)|=(0X01<<5);// USART2->CR1|=(0X01<<5); << USART2 RX interrupt enable
	
	(*(volatile unsigned int*)0x4000440C)|=(0X01<<13);// USART1->CR1|=(0X01<<13); <<USART2 enable		
	
	(*(volatile unsigned int*)0xE000E104)|=(0x01<<6);// NVIC->ISER[1]=(0X01<<6); << UART2 interrupt vecter enable
}
		
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	
//	unsigned int a[3][3];
//	char b[4];
	unsigned char data='a', data2='b';
	
  stm32_Init ();                                  // STM32 setup
	Init_UART1();
	Init_UART2();
	
	//Delay(100000);
	//Write_UART1(data2);
  for(;;) {
		Write_UART1(data);
		Delay(1000000);
		
	}
   
 }

void USART2_IRQHandler(void)
{
	char data;
	
	if( (USART2->SR&(0X01<<5) ) !=0 )
	{
		data=Read_UART2();
		temp=data;
		Write_UART2(data);
		USART2->SR&=~(0X01<<5);
		//(*(volatile unsigned int*)0x40004400)&=~(0x01<<5);// USART2->SR&=~(0X01<<5);
	}
}
