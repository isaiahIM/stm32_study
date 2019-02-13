
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization

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
// PC.13 	: KEY1
// PA.8   	: LED0
// PD.2			: LED1
void port_init()
{
		
		//RCC->APB2ENR = 0x04;
	(*(volatile unsigned int*)0x40021018)=0X00;
	(*(volatile unsigned int*)0x40021018)|=(0X04);// PA CLOCK ENABLE
	(*(volatile unsigned int*)0x40021018)|=(0X04<<2);// PC CLOCK ENABLE
	(*(volatile unsigned int*)0x40021018)|=(0X04<<3);// PD CLOCK ENABLE
	
		
	//PC:4001 1000 PD:4001 1400 PA:4001 0800
	//GPIOx->CRH, CRL INIT
	(*(volatile unsigned int*)0x40010804)=0X00;
	(*(volatile unsigned int*)0X40011400)=0X00;
	(*(volatile unsigned int*)0X40011000)=0X00;
	(*(volatile unsigned int*)0X40011004)=0X00;
	
	//GPIOx->CRH, CRL
	(*(volatile unsigned int*)0x40010804)|=0X03;// GPIOA->CRH|=0X00000003; <<PA8 PORT INIT(OUTPUT_, ENERAL PUSH_PULL, 50MHZ MAX)
	(*(volatile unsigned int*)0X40011400)|=(0X03<<8);// GPIOD->CRL|=0X03<<8;
	(*(volatile unsigned int*)0X40011000)|=(0X08<<4);// GPIOC->CRL|=0X08<<4; <<PC1 PORT INIT(INPUT, pull_up)
	(*(volatile unsigned int*)0X40011004)|=(0X08<<20);// GPIOC->CRH|=0X08<<20; <<PC13 INIT(INPUT, pull_up) 
	
	//GPIOC->ODR SETTING(PULL-UP)
	(*(volatile unsigned int*)0X4001100C)=0x00;
	
	(*(volatile unsigned int*)0X4001100C)|= (0X01<<1);
	(*(volatile unsigned int*)0X4001100C)|= (0X01<<13);
	/*888
	GPIOA->CRL=0x33333333;
		GPIOB->CRL=0x33333333;
		GPIOD->CRL=0x33333333;
		GPIOA->CRH=0x33333333;
		GPIOB->CRH=0x33333333;
		GPIOD->CRH=0x33333333;
	
  	// PC : Input pull-up
	  GPIOC->CRL = 0x88888888;
	  GPIOC->CRH = 0x88888888;
	  GPIOC->ODR = 0x0000FFFF;
	*/
}


void gpioA_High(char pin_num)
{
	GPIOA->BSRR=0x01 << pin_num;
}

void gpioA_Low(char pin_num)
{
	GPIOA->BRR=0x01 << pin_num;
}

void gpioB_High(char pin_num)
{
	GPIOB->BSRR=0x01 << pin_num;
}

void gpioB_Low(char pin_num)
{
	GPIOB->BRR=0x01 << pin_num;
}

void gpioD_High(char pin_num)
{
	GPIOD->BSRR=0x01 << pin_num;
}

void gpioD_Low(char pin_num)
{
	GPIOD->BRR=0x01 << pin_num;
}

unsigned int gpioC_Read(char pin_num)
{
		return (GPIOC->IDR & (1<<pin_num));
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
	stm32_Init ();                      // STM32 setup
	port_init();
	//GPIOA->CRH |=0X0000000B;//PA8 output mode
	// LED1:PD2 KEY0:0C1, KEY1:PC13, LED0: PA8
	
	for(;;)
	{
		if( ((*(volatile unsigned int*)0x40011008)&(0x01<<13))!=0 )// GPIOC->IDR &=(0X01<<13);
		{
			(*(volatile unsigned int*)0X4001140c)|=(0X01<<2);// GPIOA->ODR|=(0X01<<2);
		}
		else
		{
			(*(volatile unsigned int*)0X4001140c) &= ~(0X01<<2);
		}
		
		if( (*(volatile unsigned int*)0x40011008)&(0x01<<1) )// GPIOC->IDR&=(0X01<<1);
		{
			(*(volatile unsigned int*)0x4001080c)|=(0x01<<8);// GPIOD->ODR |= (0X01<<8);
		}
		else
		{
			(*(volatile unsigned int*)0x4001080c) &= ~(0x01<<8);
		}
		
		
  	
		// gpioA_High(PIN_NUM_8);
		// gpioD_High(PIN_NUM_2);
		// Delay_ms(500);
//		
		// gpioA_Low(PIN_NUM_8);
		// gpioD_Low(PIN_NUM_2);
		// Delay_ms(500); 
		
		
		/*
				if(gpioC_Read(PIN_NUM_1) == 0) {
						gpioA_Low(PIN_NUM_8);
				}
				else {
					  gpioA_High(PIN_NUM_8);
				}
				
				if(gpioC_Read(PIN_NUM_13) == 0) {
						gpioD_Low(PIN_NUM_2);
				}
				else {
					  gpioD_High(PIN_NUM_2);
				}*/
		
		
	}
   
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
