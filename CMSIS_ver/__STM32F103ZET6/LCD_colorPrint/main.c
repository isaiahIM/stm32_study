#include "math.h"
#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "my_LCD.h"

#define HIGH_DENSITY


#define BLUE_SIZE 20000
GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;

uint16_t blueScreen[10];


int main()
{
    int i;
    for(i=0; i<BLUE_SIZE; i++)
    {
        blueScreen[i]=0x3f;
    }
    
/*USART RCC_CLOCK CONFIG*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


    
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);
    SysTick_Init();

    
    LCD_Init();
    while(1)
    {
//        LCD_Draw_Square(120-25, 160-25, 120+25, 160+25, blueScreen[0]);
        LCD_Draw_Circle(120, 160, 80, blueScreen[0]);
        
//        LCD_GotoXY(120, 160);
//        LCD_Write_MultiData(0x22, blueScreen, 10);
        
//        LCD_Write_MultiData(0x22, blueScreen, BLUE_SIZE);
    }
}



