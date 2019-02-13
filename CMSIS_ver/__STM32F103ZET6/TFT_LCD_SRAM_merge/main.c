#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "my_LCD_fsmc.h"
#include "my_LCD_font.h"
#include "fsmc_SRAM.h"

#define HIGH_DENSITY

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;


int main()
{
    int i;
//    uint8_t font[8]={0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x00, 0x00};
/*USART RCC_CLOCK CONFIG*/
    uint16_t color[3]={LCD_COLOR_RED, LCD_COLOR_GREEN, LCD_COLOR_BLUE};
    uint16_t readSRAM[3];

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    SysTick_Init();
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);


    LCD_Init();
    Delay_ms(1);
    SRAM_Init();
    Delay_ms(1);

    SRAM_WriteBuffer(color, 0x800, 3);
    SRAM_ReadBuffer(readSRAM, 0x800, 3);
    
    while(1)
    {
        for(i=0; i<3; i++)
        {
            LCD_Draw_Circle( (320/2), (240/2), 50, readSRAM[i]);
            Delay_ms(1000);
        }
        
    }
}

