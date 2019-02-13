#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "my_CLCD_fsmc.h"
#include "my_LCD_fsmc.h"
#include "my_LCD_font.h"

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


    LCD_Init();
    Delay_ms(1);
    CLCD_Init();
    Delay_ms(1);
    
    while(1)
    {
        CLCD_Clear();
        CLCD_PrintString(1, 1, "fuck you", 8);
        
        LCD_GotoXY(0, 0);
        LCD_PrintBMP( (uint8_t*)fuck_for_you, BMP_PrintDir_vertical);
    }
}

