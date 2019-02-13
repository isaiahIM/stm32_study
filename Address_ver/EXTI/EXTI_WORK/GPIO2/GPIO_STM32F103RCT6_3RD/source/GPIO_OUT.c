
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization

int TMP=0;

void Delay_us(unsigned int nCount);
void Delay_ms(unsigned int count_ms);


/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay_us (unsigned int nCount)
{
  for(; nCount != 0; nCount--)
  {
  } 
}
//Delay_us(15000); => About 1 ms
//Delay_us(1500); => About 0.1 ms
//Delay_us(150); => About 10 us
//Delay_us(15); => About 1 us

void Delay_ms(unsigned int count_ms){
  for(; count_ms != 0; count_ms--) Delay_us(15000);
}



// PC.1 		: KEY0
// PA.8   	: LED0
void port_init()
{
		//RCC->APB2ENR = 0x04;
	(*(volatile unsigned int*)0x40021018)=0X00;
	(*(volatile unsigned int*)0x40021018)|=(0X04);// RCC->APB2ENR|=0X04; << PA CLOCK ENABLE
	(*(volatile unsigned int*)0x40021018)|=(0X04<<2);// RCC->APB2ENR|=0X04<<2 << PC CLOCK ENABLE
	
		
	//PC:4001 1000 PA:4001 0800
	//GPIOx->CRL INIT
	(*(volatile unsigned int*)0x40010800)=0X00;
	(*(volatile unsigned int*)0X40011000)=0X00;

	//GPIOx->CRL
	(*(volatile unsigned int*)0x40010800)|=0X03;// GPIOA->CRL|=0X00000003; <<PA0 PORT INIT(OUTPUT_, ENERAL PUSH_PULL, 50MHZ MAX);
	(*(volatile unsigned int*)0x40010800)|=(0X03<<4);// GPIOA->CRL|=(0X03<<4); <<PA1 PORT INIT(OUTPUT_, ENERAL PUSH_PULL, 50MHZ MAX);
	(*(volatile unsigned int*)0x40010800)|=(0X03<<8);// GPIOA->CRL|=(0X03<<8); <<PA2 PORT INIT(OUTPUT_, ENERAL PUSH_PULL, 50MHZ MAX);
	(*(volatile unsigned int*)0x40010800)|=(0X03<<12);// GPIOA->CRL|=(0X03<<12); <<PA3 PORT INIT(OUTPUT_, ENERAL PUSH_PULL, 50MHZ MAX);
	
	(*(volatile unsigned int*)0X40011000)|=(0X04);// GPIOC->CRL|=0X08; <<PC0 PORT INIT(INPUT, FLOATING)
	(*(volatile unsigned int*)0X40011000)|=(0X04<<4);// GPIOC->CRL|=0X08<<4; <<PC1 PORT INIT(INPUT, FLOATING)
	(*(volatile unsigned int*)0X40011000)|=(0X04<<8);// GPIOC->CRL|=0X08<<8; <<PC2 PORT INIT(INPUT, FLOATING)
	

}

void Init_Interrupt(void)
{
	(*(volatile unsigned int*)0x40021018)|=0X01;// RCC->APB2ENR|=0X01; << AFIO CLOCK ENABLE
	
	(*(volatile unsigned int*)0x40010008)|=0x02;// AFIO->EXTICR[0]|=0X02; << PC0 IS EXTI0
	(*(volatile unsigned int*)0x40010008)|=(0x02<<4);// AFIO->EXTICR[0]|=(0X02<<4); << PC1 IS EXTI1
	(*(volatile unsigned int*)0x40010008)|=(0x02<<8);// AFIO->EXTICR[0]|=(0X02<<8); << PC2 IS EXTI2
	
	(*(volatile unsigned int*)0x40010400)|=0X01;// EXTI->IMR|=0X01; << EXTI0 MASK(USE)
	(*(volatile unsigned int*)0x40010400)|=(0X01<<1);// EXTI->IMR|=(0X01<<1); << EXTI1 MASK(USE)
	(*(volatile unsigned int*)0x40010400)|=(0X01<<2);// EXTI->IMR|=(0X01<<2); << EXTI2 MASK(USE)
	
	(*(volatile unsigned int*)0x4001040C)|=0X01;// EXTI->FTSR|=0X01; << EXTI0 Falling Interrupt
	(*(volatile unsigned int*)0x4001040C)|=(0X01<<1);// EXTI->FTSR|=(0X01<<1); << EXTI1 Falling Interrupt
	(*(volatile unsigned int*)0x4001040C)|=(0X03<<2);// EXTI->FTSR|=(0X01<<2); << EXTI2 Falling Interrupt
	
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<6);// NVIC->ISER[0]|=(0X01<<6); << EXTI0 ENABLE
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<7);// NVIC->ISER[0]|=(0X01<<7); << EXTI1 ENABLE
	(*(volatile unsigned int*)0xE000E100)|=(0X01<<8);// NVIC->ISER[0]|=(0X01<<8); << EXTI2 ENABLE
	
	(*(volatile unsigned int*)0xE000ED0C)=(0x00);
	(*(volatile unsigned int*)0xE000ED0C)=(0x05FA<<16) | (0X05<<8);// SCB->AIRCR|=(0X5FA<<16); << GROUP Prlorlty Key
	//(*(volatile unsigned int*)0xE000ED0C)|=(0X05<<8);// SCB->AIRCR|=(0X05<<8); << GROUP Prlorlty (2 group, 2 sub)
	
	(*(volatile unsigned int*)0xE000E404)|= (0X30<<16);// NVIC->IPR[1]|=(0X30<<16); << INTERRUPT 6 TABLE PRLORLTY IS 0011
	(*(volatile unsigned int*)0xE000E404)|= (0X20<<24);// NVIC->IPR[1]|=(0X20<<24); << INTERRUPT 7 TABLE PRLORLTY IS 0010
	(*(volatile unsigned int*)0xE000E408)|= (0X20);// NVIC->IPR[2]|=(0X10); << INTERRUPT 8 TABLE PRLORLTY IS 0001
}

#define PIN_NUM_0		0
#define PIN_NUM_1		1
#define PIN_NUM_2		2
#define PIN_NUM_3		3
#define PIN_NUM_4		4
#define PIN_NUM_5		5
#define PIN_NUM_6		6
#define PIN_NUM_7		7
#define PIN_NUM_8		8
#define PIN_NUM_9		9
#define PIN_NUM_10		10
#define PIN_NUM_11		11
#define PIN_NUM_12  	12
#define PIN_NUM_13		13
#define PIN_NUM_14		14
#define PIN_NUM_15		15



/*----------------------------------------------------------*
*  MIAN ENTRY                                               |
*----------------------------------------------------------*/

int main (void) {
	port_init();
	Init_Interrupt();
	
	for(;;)
	{
		(*(volatile unsigned int*)0x4001080C)|=(0X01<<3);// GPIOA->ODR|=(0X01<<3);
		Delay_ms(1);
		(*(volatile unsigned int*)0x4001080C)&=~(0X01<<3);// GPIOA->ODR|=(0X01<<3);
		Delay_ms(1);
	}
   
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/

	
	
	
	void EXTI0_IRQHandler(void)
	{
		int i;
		if(EXTI->PR& (1<<0))
		{
			TMP++;
					for(i=0; i<1000; i++)
					{
						GPIOA->ODR|=(0X01<<0);
						//(*(volatile unsigned int*)0x4001080C)|=(0X01);// GPIOA->ODR|=(0X01<<0);
						Delay_ms(1);
						GPIOA->ODR&=~(0X01<<0);
						//(*(volatile unsigned int*)0x4001080C)&=~(0X01);// GPIOA->ODR|=(0X01<<0);
						Delay_ms(1);
						
					}
					
					(*(volatile unsigned int*) 0X40010414)=0X01;// EXTI->PR=0X01; << Interrupt0 pending(stop)
		}
	}
	
	void EXTI1_IRQHandler()
	{
		int i;
		for(i=0; i<1000; i++)
		{
			(*(volatile unsigned int*)0x4001080C)|=(0X01<<1);// GPIOA->ODR|=(0X01<<0);
			Delay_ms(1);
			(*(volatile unsigned int*)0x4001080C)&=~(0X01<<1);// GPIOA->ODR|=(0X01<<0);
			Delay_ms(1);
		}
		
		(*(volatile unsigned int*) 0X40010414)=(0X01<<1);// EXTI->PR=0X01<<1; << Interrupt1 pending(stop)
	}
	
	void EXTI2_IRQHandler()
	{
		int i;
		for(i=0; i<1000; i++)
		{
			(*(volatile unsigned int*)0x4001080C)|=(0X01<<2);// GPIOA->ODR|=(0X01<<0);
			Delay_ms(1);
			(*(volatile unsigned int*)0x4001080C)&=~(0X01<<2);// GPIOA->ODR|=(0X01<<0);
			Delay_ms(1);
		}
		
		(*(volatile unsigned int*) 0X40010414)=(0X01<<2);// EXTI->PR=0X01<<2; << Interrupt2 pending clear(stop)
	}