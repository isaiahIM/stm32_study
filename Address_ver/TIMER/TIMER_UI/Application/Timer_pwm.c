
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
/*----------------------------------------------------------*\
 | Timer1 Update Interrupt Handler                          |
\*----------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

	ledLight = ~ledLight;
	if( ledLight )
    	GPIOB->ODR &= ~LED;                           // switch on LED
	else
    	GPIOB->ODR |=  LED;                           // switch off LED

    TIM1->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler

/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source

	ledLight2 = ~ledLight2;
	if( ledLight2 )
    	GPIOB->ODR &= ~LED1;                           // switch on LED
	else
    	GPIOB->ODR |=  LED1;                           // switch off LED

    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM2_UP_IRQHandler


void SysTick_Handler(void)
 {
	ledLight1 = ~ledLight1;
	if( ledLight1 )
    	GPIOB->ODR &= ~LED1;                           // switch on LED
	else
    	GPIOB->ODR |=  LED1; 				
		
	if(count++ == 1000){
		count=0;
		ledLight3 = ~ledLight3;
		if( ledLight3 )
	    	GPIOB->ODR &= ~(0x01 << 4);                           // switch on LED
		else
	    	GPIOB->ODR |=  (0x01 << 4); 
	}
			
		                          // switch off LED
 }

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
 
for(;;) {
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
