#ifndef MY_CLCD_FSMC__H
#define MY_CLCD_FSMC__H

#include "stm32f10x.h"
#include "SysTick.h"

#define CLCD_DATA_NUM 8
#define CLCD_ERR_CGRAM_ADDR_OVF -1

#define CLCD_REG_WRITE (*(volatile uint8_t*)0x64000000)
#define CLCD_DISPLAY_WRITE (*(volatile uint8_t*)0x64000001)


void CLCD_FSMC_Init(void);
void CLCD_Init(void);
void CLCD_Reg_Write(uint8_t data);
void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data);
void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size);
void CLCD_Clear(void);
int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[]);

#endif



