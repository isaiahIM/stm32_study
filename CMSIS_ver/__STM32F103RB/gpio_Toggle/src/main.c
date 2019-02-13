#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;


int main(void)
{
  volatile int i, j;
	
	GPIO_InitTypeDef ledPin;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	ledPin.GPIO_Pin=GPIO_Pin_8;
	ledPin.GPIO_Mode=GPIO_Mode_Out_PP;
	ledPin.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &ledPin);
	
	while(1)
	{
		GPIO_SetBits(GPIOA, ledPin.GPIO_Pin);
		for(i=0;i<100;i++)
	  	for(j=0;j<60000;j++);
		
		GPIO_ResetBits(GPIOA, ledPin.GPIO_Pin);
		for(i=0;i<100;i++)
	  	for(j=0;j<60000;j++);
		
	}
}
