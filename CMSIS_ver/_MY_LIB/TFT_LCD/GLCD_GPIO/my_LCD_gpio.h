#ifndef MY_LCD_GPIO__H
#define MY_LCD_GPIO__H

#define LCD_Port_Dir_Write 1
#define LCD_Port_Dir_Read 0
#define LCD_DATA_NUM 16

#define LCD_RA_SET_COLOR 0X22

#define BMP_COLOR_R 0
#define BMP_COLOR_G 1
#define BMP_COLOR_B 2
#define BMP_RGB_BIT_CHECK 28
#define BMP_PIXEL_START 10

#include "stm32f10x.h"
#include <math.h>

void LCD_Init(void);
void LCD_Write_MultiData(uint16_t Address, uint16_t data[], uint32_t dataSize);
void LCD_WriteData(uint16_t Address, uint16_t data);
uint16_t LCD_ReadData(uint16_t Address);
void LCD_ChangePort_Dir(int LCD_Port_Dir);
void LCD_GotoXY(uint32_t start_x, uint32_t start_y);

void LCD_Draw_Square(uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint16_t fill);
void LCD_Draw_Circle(uint32_t center_x, uint32_t center_y, uint32_t radius, uint16_t fill);

void LCD_PrintBMP(uint8_t bmp[]);
#endif
