#ifndef MY_CLCD_GPIO__H
#define MY_CLCD_GPIO__H

#include "stm32f10x.h"
#define LCD_DATA_NUM 16
#define CLCD_ERR_CGRAM_ADDR_OVF -1

void CLCD_Conofig(void);
void CLCD_Reg_Write(uint8_t data);
void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data);
void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size);
int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[]);

GPIO_InitTypeDef clcd;

GPIO_TypeDef* LCD_DATA_Port[LCD_DATA_NUM]=
{
    GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE,
    GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOD, GPIOD, GPIOD
};
uint16_t LCD_DATA_Pin[LCD_DATA_NUM]=
{
    GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_0, GPIO_Pin_1,
    GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, 
    GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14,
    GPIO_Pin_15, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10
};

GPIO_TypeDef* CLCD_RW_Port=GPIOG;
uint16_t CLCD_RW_Pin=GPIO_Pin_10;

GPIO_TypeDef* CLCD_EN_Port=GPIOD;
uint16_t CLCD_EN_Pin=GPIO_Pin_5;

GPIO_TypeDef* CLCD_RS_Port=GPIOF;
uint16_t CLCD_RS_Pin=GPIO_Pin_0;

#endif



