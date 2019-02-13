#include "stm32f10x.h"
#include "my_LCD.h"
#include "my_LCD_font.h"

#define HIGH_DENSITY

int main()
{    
    LCD_Init();
    LCD_WriteData(0x03, 0x1038);
    while(1)
    {
        LCD_GotoXY(0, 0);
        LCD_PrintBMP( (uint8_t*)tiger);
    }
}


