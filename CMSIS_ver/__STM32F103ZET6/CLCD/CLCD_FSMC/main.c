#include "math.h"
#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "my_CLCD_fsmc.h"

#define HIGH_DENSITY

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;


int main()
{
//    int i;
//    uint8_t font[8]={0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x00, 0x00};
/*USART RCC_CLOCK CONFIG*/
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    SysTick_Init();
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);
    CLCD_Conofig();
	Delay_ms(1);
 


	
	CLCD_Print(1, 1, 'B');
	Delay_ms(10);
	
    while(1)
    {
        //CLCD_Print(1, 1, 'a');
        //CLCD_PrintString(1, 1, "hello!", 6);
        
    }
}

