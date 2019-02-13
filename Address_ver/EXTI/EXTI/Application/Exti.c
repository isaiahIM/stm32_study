
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

int ledLight = 0;
int ledLight1 = 0;
int Alarm = 0;
int count=0;
volatile int i;
volatile unsigned char toggle_State=0;// I/O toggle value

#define __WFI                             __wfi

#define  PWR_CR_PDDS                         ((unsigned short)0x0002)     /*!< Power Down Deepsleep */
#define  PWR_CR_CWUF                         ((unsigned short)0x0004)     /*!< Clear Wakeup Flag */
#define  SCB_SCR_SLEEPDEEP                   ((unsigned char)0x04)               /*!< Sleep deep bit */

void PWR_EnterSTANDBYMode(void)
{
  /* Clear Wake-up flag */
  PWR->CR |= PWR_CR_CWUF;
  /* Select STANDBY mode */
  PWR->CR |= PWR_CR_PDDS;
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}


void Delay(vu32 nCount) {
		for(; nCount != 0; nCount--);
  }

/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | EXTI0 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI0_IRQHandler(void)
{
  int cnt=0;
	for(cnt=0; cnt<100; cnt++)
	{  
		GPIOB->BSRR|=0X01<<13;                           // switch on LED
			Delay(10000);
			GPIOB->BRR|=0X01<<13;                           // switch off LED
			Delay(10000);
	}
	EXTI->PR = (1<<0);
}

/*----------------------------------------------------------*\
 | EXTI1 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI1_IRQHandler(void)
{
	int cnt=0; 
	for(cnt=0; cnt<100; cnt++)
	{
    GPIOB->BSRR |=0X01<<14; 
		Delay(10000);		// switch on LED
    GPIOB->BRR|=0X01<<14;                           // switch off LED
		Delay(10000);
	}
	EXTI->PR = (1<<1);
}

void EXTI2_IRQHandler(void)
{
	int cnt=0;
	for(cnt=0; cnt<100; cnt++)
	{ 	
		GPIOB->BRR|=0X01<<15;
			Delay(10000);
			GPIOB->BSRR|=0X01<<15;
			Delay(10000);
	}
	EXTI->PR = (1<<2);
}



/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI9_5_IRQHandler(void)
{
  if (EXTI->PR & (1<<6)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOB->ODR &= ~LED;                           // switch on LED
    else
    	GPIOB->ODR |=  LED;                           // switch off LED
		
		
    EXTI->PR |= (1<<6);                          // clear pending interrupt
  }
}


/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOB->ODR &= ~LED;                           // switch on LED
    else
    	GPIOB->ODR |=  LED;                           // switch off LED

    EXTI->PR |= (1<<13);                          // clear pending interrupt
  }
}


	unsigned int tmp_data=66500;
	short tmp_data_1=1500;
	
	
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	
	//int a=100;
	
  //stm32_Init ();                                // STM32 setup

// Interrupt Group Priority
//SCB->AIRCR =0x05FA0500; 
// =>  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup; 

//NVIC->IPR[1] |=0x00;

// Interrupt enable
//NVIC->ISER[0]=0x00;

//Port enable
//RCC->APB2ENR |= 0x00;

// Port Select
//AFIO->EXTICR[0]=0x00;

// External Interrupt
//EXTI->IMR = 0x00;
//EXTI->RTSR      |= 0x00;
//EXTI->FTSR      |= 0x00;

		for(;;) {
			
			GPIOB->BSRR|=0X01<<12;
			Delay(10000);
			GPIOB->BRR|=0X01<<12;
			Delay(10000);
			
#if 0 
//      GPIOB->ODR &= ~(0x01 << 5);                           // switch on LED
//    GPIOB->ODR &= ~(0x01 << 8);                          // switch on LED
//    GPIOB->ODR |= (0x01 << 9);                         // switch on LED
//	Delay(2000000);
//    GPIOB->ODR |=  (0x01 << 5);                           // switch off LED
//    GPIOB->ODR |=  (0x01 << 8);                           // switch on LED
//    GPIOB->ODR &=  ~(0x01 << 9);                            // switch on LED
//	Delay(2000000);

//	if(count == 1){
//		count = 0;
//  		//__WFI();  // Sleep Mode Test
//		PWR_EnterSTANDBYMode();	  // Standby Mode Test
	 }

#endif
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
