#ifndef MY_CLCD_FSMC__H
#define MY_CLCD_FSMC__H

#include "stm32f10x.h"
#include "SysTick.h"

#define CLCD_DATA_NUM 8
#define CLCD_ERR_CGRAM_ADDR_OVF -1

typedef struct
{
  vu8 LCD_REG;
  vu8 LCD_RAM;
} LCD_TypeDef;

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE    ((u32)(0x60000000 | 0x00000000))
#define LCD         ((LCD_TypeDef *) LCD_BASE)


#define CLCD_REG_WRITE (*(volatile uint8_t*)0x60000000)
#define CLCD_DISPLAY_WRITE (*(volatile uint8_t*)0x60000001)


void CLCD_FSMC_Init(void);
void CLCD_Conofig(void);
void CLCD_Reg_Write(uint8_t data);
void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data);
void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size);
int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[]);

#endif



