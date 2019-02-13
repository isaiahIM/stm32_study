
#include <stdio.h>

#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/


unsigned short  analog [3];


/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar (int ch)  {
  while (!(USART1->SR & USART_FLAG_TXE));
 USART1->DR = (ch & 0x1FF);

  return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey (void)  {

  while (!(USART1->SR & USART_FLAG_RXNE));

  return ((int)(USART1->DR & 0x1FF));
}


void USART1_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART1->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart 1 : %c",receive_char);

    }
 /*
    if (IIR & USART_FLAG_TXE) {
      USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt
	//USART1->DR = '5';
	printf("sent data is %c..\n\r",USART1->DR);
    }
 */
}


void USART2_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART2->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart2 : %c",receive_char);

    }
}


void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & (1<<0)) {                        
	printf("in exti0 irq handler...\r\n");
    EXTI->PR |= (1<<0);                           // clear pending interrupt
  }
}

void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        
	printf("in exti0 irq handler...\r\n");
	PWR_EnterSTANDBYMode();

    EXTI->PR |= (1<<1);                           // clear pending interrupt
  }
}


void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                   /* set PIN1 analog input (see stm32_Init.c) */

  RCC->AHBENR |= (1<<0);                       /* enable periperal clock for DMA       */

//  DMA1_Channel1->CMAR  = (unsigned long)&analog; /* set chn1 memory address     */
  DMA1_Channel1->CMAR  = (unsigned long)analog; /* set chn1 memory address     */
  DMA1_Channel1->CPAR  = (unsigned long)&(ADC1->DR);     /* set chn1 peripheral address */
  DMA1_Channel1->CNDTR = 0x03;                     /* transmit 3 words                     */
//  DMA_Channel1->CCR   = 0x00002520;            /* configure DMA channel 1              */
  DMA1_Channel1->CCR   = 0x000025A0;            /* configure DMA channel 1              */
                                               /* circular mode, memory increment mode */
                                               /* memory & peripheral size 16bit       */
                                               /* channel priotity high                */
  DMA1_Channel1->CCR  |= (1 << 0);              /* enable DMA Channe1                   */

  RCC->APB2ENR |= (1<<9);                      /* enable periperal clock for ADC1      */

  ADC1->SQR1  = 0x00200000;                    /* three conversions                    */
  ADC1->SQR3  = (3<<10) | (2<<5) | (1<<0);     /* set order to chn1 - chn2 - chn3      */
  ADC1->SMPR2 = (5<< 9) | (5<<6) | (5<<3);     /* set sample time (55,5 cycles)        */ 

  ADC1->CR1   =  0x00000100;                   /* use independant mode, SCAN mode      */
  ADC1->CR2   =  0x000E0103;                   /* data align right, cont. conversion   */
                                               /* EXTSEL = SWSTART                     */ 
                                               /* enable ADC, DMA mode                 */
  ADC1->CR2  |=  0x00500000;                   /* start SW conversion                  */
  }

	void Init_DMA_ADC(void)// DMA: 0x40020000  DAC: 0x40012400
{
/*DMA1*/
	(*(volatile unsigned int*)0X40021014)|=0X01;// RCC->AHBENR=0X01; << DMA1 Clock on
	
	(*(volatile unsigned int*)0x40020010)=0x4001244C;// DMA1_Channel1->CPAR=&(DMA1->DR); << CAPTURE ADDRESS
	(*(volatile unsigned int*)0x40020014)=0X20002000;// DMA1_Channel1->CMAR=0X20002000; << STORE ADDRESS
	(*(volatile unsigned int*)0x4002000C)=0X03;// DMA1_Channel1->CNDTR=0X02; << 2 WORD WRITE
	
	(*(volatile unsigned int*)0x40020008)=(0x01<<5);// DMA1_Channel1->CCR=(0X01<<5); << DMA LOOP
	(*(volatile unsigned int*)0x40020008)|=(0x01<<7);// DMA1_Channel1->CCR|=(0X01<<7); << DMA Memoy increment
	(*(volatile unsigned int*)0x40020008)|=(0x01<<8);// DMA1_Channel1->CCR|=(0X01<<58; << DMA memory size is 16 bit
	(*(volatile unsigned int*)0x40020008)|=(0x01<<10);// DMA1_Channel1->CCR|=(0X01<<10); << DMA prepheral size is 16 bit
	(*(volatile unsigned int*)0x40020008)|=(0x02<<12);// DMA1_Channel1->CCR|=(0X02<<12); << DMA Channel priority is HIGH
	
	(*(volatile unsigned int*)0x40020008)|=(0x01);// DMA1_Channel1->CCR|=(0X01); << DMA enable
	
/*ADC1*/
	(*(volatile unsigned int*)0X40021018)=(0X01<<9);// RCC->APB2ENR=(0X01<<9); << ADC1 Clock on
	
	(*(volatile unsigned int*)0x40012408)=(0X01);// ADC1->CR2=0X01; <<ADC ON
	
	(*(volatile unsigned int*)0x40012404)=(0x01<<8);// ADC1->CR1=(0X01<<8); << ADC1 SCAN
	
	(*(volatile unsigned int*)0x40012408)|=(0X01<<1);// ADC1->CR2|=(0X01<<1); <<ADC LOOP
	(*(volatile unsigned int*)0x40012408)|=(0X01<<8);// ADC1->CR2|=(0X01<<8); <<ADC DMA MODE
	(*(volatile unsigned int*)0x40012408)|=(0X01<<20);// ADC1->CR2|=(0X01<<20); <<ADC External trigger conversion mode
	(*(volatile unsigned int*)0x40012408)|=(0X07<<17);// ADC1->CR2|=(0X07<<17); <<ADC EVENT for swStart
	
	(*(volatile unsigned int*)0x40012410)|=(0x05<<3);// ADC1->SMPR2=(0X05<<3); << ADC1 ch1 Sampling freqency is 55.5 cycle
	(*(volatile unsigned int*)0x40012410)|=(0x05<<6);// ADC1->SMPR2=(0X05<<6); << ADC1 ch2 Sampling freqency is 55.5 cycle
	(*(volatile unsigned int*)0x40012410)|=(0x05<<9);// ADC1->SMPR2=(0X05<<9); << ADC1 ch3 Sampling freqency is 55.5 cycle
	
	(*(volatile unsigned int*)0x4001242C)=(0X02<<20);// ADC1->SQR1=(0X03<20); << ADC1 use Channel 3
	
	(*(volatile unsigned int*)0x40012434)=(0X01);// ADC1->SQR3=(0X01); << ADC1 Sequence1 is channel1
	(*(volatile unsigned int*)0x40012434)|=(0X02<<5);// ADC1->SQR3|=(0X02<<5); << ADC1 Sequence1 is channel2
	(*(volatile unsigned int*)0x40012434)|=(0X03<<10);// ADC1->SQR3|=(0X01<<10); << ADC1 Sequence1 is channel1
	
	
	(*(volatile unsigned int*)0x40012408)|=(0X01<<22);// ADC1->CR2|=(0X01<<22); << ADC1 CH1 START
}

void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  	char s[20];
	int i;
	
	stm32_Init ();   
	// STM32 setup
Init_DMA_ADC();
  //adc_Init();
	

	for(;;) {
		printf("% 4.2fV \r\n", (float)(analog[0] * 3.3 / 0xFFF));
		//printf("% 4.2fV , p=%p\r\n", (float)(analog[0] * 3.3 / 0xFFF), &analog[0]);
	}


  }
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
