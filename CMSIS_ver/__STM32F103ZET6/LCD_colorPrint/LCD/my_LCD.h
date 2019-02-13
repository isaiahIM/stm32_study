#ifndef MY_LCD__H
#define MY_LCD__H

#define LCD_Port_Dir_Write 1
#define LCD_Port_Dir_Read 0
#define LCD_DATA_NUM 16

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

#endif
