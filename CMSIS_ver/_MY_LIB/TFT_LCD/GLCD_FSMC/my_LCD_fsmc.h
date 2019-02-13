#ifndef MY_LCD_FSMC__H
#define MY_LCD_FSMC__H

#define LCD_Port_Dir_Write 1
#define LCD_Port_Dir_Read 0
#define LCD_DATA_NUM 16

#define LCD_RA_SET_COLOR 0X22

#define BMP_COLOR_R 0
#define BMP_COLOR_G 1
#define BMP_COLOR_B 2
#define BMP_RGB_BIT_CHECK 28
#define BMP_PIXEL_START 10

#define BMP_PrintDir_horizontal 1
#define BMP_PrintDir_vertical 2

#define LCD_COLOR_RED 0XF800
#define LCD_COLOR_GREEN 0X7E0
#define LCD_COLOR_BLUE 0X1F
#define LCD_ADDR (*(volatile uint16_t*)0x6C000000) // BANK1-1
#define LCD_DATA (*(volatile uint16_t*)0x6C000002) 

#include "stm32f10x.h"
#include <math.h>


void LCD_FSMC_Init(void);
void LCD_Init(void);
void LCD_Write_MultiData(uint16_t Address, uint16_t data[], uint32_t dataSize);
void LCD_WriteData(uint16_t Address, uint16_t data);
uint16_t LCD_ReadData(uint16_t Address);
void LCD_ChangePort_Dir(int LCD_Port_Dir);
void LCD_GotoXY(uint32_t start_x, uint32_t start_y);

void LCD_Draw_Square(uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint16_t fill);
void LCD_Draw_Circle(uint32_t center_x, uint32_t center_y, uint32_t radius, uint16_t fill);

void LCD_PrintBMP(uint8_t bmp[], uint8_t BMP_PrintDir);

#endif
