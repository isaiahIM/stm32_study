#include "math.h"
#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "my_CLCD_gpio.h"

//#define HIGH_DENSITY
//#define LCD_DATA_NUM 16

//#define CLCD_ERR_CGRAM_ADDR_OVF -1

//void CLCD_Conofig(void);
//void CLCD_Reg_Write(uint8_t data);
//void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data);
//void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size);
//int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[]);

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;

//GPIO_InitTypeDef clcd;
//
//GPIO_TypeDef* LCD_DATA_Port[LCD_DATA_NUM]=
//{
//    GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE,
//    GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOD, GPIOD, GPIOD
//};
//uint16_t LCD_DATA_Pin[LCD_DATA_NUM]=
//{
//    GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_0, GPIO_Pin_1,
//    GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, 
//    GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14,
//    GPIO_Pin_15, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10
//};
//
//GPIO_TypeDef* CLCD_RW_Port=GPIOG;
//uint16_t CLCD_RW_Pin=GPIO_Pin_10;
//
//GPIO_TypeDef* CLCD_EN_Port=GPIOD;
//uint16_t CLCD_EN_Pin=GPIO_Pin_5;
//
//GPIO_TypeDef* CLCD_RS_Port=GPIOF;
//uint16_t CLCD_RS_Pin=GPIO_Pin_0;

int main()
{
    int i;
    uint8_t font[8]={0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x00, 0x00};
/*USART RCC_CLOCK CONFIG*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
//    
//    /*data pin*/
//    clcd.GPIO_Mode=GPIO_Mode_Out_PP;
//    clcd.GPIO_Speed=GPIO_Speed_50MHz;
//    for(i=0; i<8; i++)
//    {
//        clcd.GPIO_Pin=LCD_DATA_Pin[i];
//        GPIO_Init(LCD_DATA_Port[i], &clcd);
//    }
//
//    clcd.GPIO_Pin=CLCD_RW_Pin;
//    GPIO_Init(CLCD_RW_Port, &clcd);// RW
//    GPIO_ResetBits(CLCD_RW_Port, CLCD_RW_Pin);
//
//    clcd.GPIO_Pin=CLCD_EN_Pin;
//    GPIO_Init(CLCD_EN_Port, &clcd);// EN
//    GPIO_ResetBits(CLCD_EN_Port, CLCD_EN_Pin);
//
//    clcd.GPIO_Pin=CLCD_RS_Pin;
//    GPIO_Init(CLCD_RS_Port, &clcd);// RS
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);

    
    SysTick_Init();
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);

    CLCD_Conofig();
    
    CLCD_MKFont(0x00, font);

    while(1)
    {
        //CLCD_Print(1, 1, 0x00);
        CLCD_PrintString(1, 1, "hello!", 6);
        
    }
}


//void CLCD_Reg_Write(uint8_t data)
//{
//    int i;
//    
//    GPIO_SetBits(CLCD_EN_Port, CLCD_EN_Pin);// enable
//    Delay_ms(1);
//    for(i=0; i<8; i++)
//    {
//        if( (data&(0x01<<i)) !=0 )
//        {
//            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
//        }
//        else
//        {
//            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
//        }
//    }
//    Delay_ms(1);
//    GPIO_ResetBits(CLCD_EN_Port, CLCD_EN_Pin);// enable
//}
//
//void CLCD_Conofig(void)
//{
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS
//
//    Delay_ms(1);
//    /*Functionn set -8bit data, 5*11 character size, 2 line use*/
//    CLCD_Reg_Write(0x38);
//    Delay_ms(1);
//    CLCD_Reg_Write(0x06);// address increment
//    Delay_ms(1);
//    /*DISPLAY ON/OFF -entire on, cursor on, cursor position on*/
//    CLCD_Reg_Write(0x0f);
//    Delay_ms(1);
//}
//
//void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data)
//{
//    uint8_t displayPos;
//
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
//    if(column==1)
//    {
//        displayPos=0x80;
//    }
//    else
//    {
//        displayPos=0xc0;
//    }
//    displayPos|=row-1;
//    CLCD_Reg_Write(displayPos);
//
//	
//    Delay_ms(1);
//    GPIO_SetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS
//    Delay_ms(1);
//    CLCD_Reg_Write(data);
//    Delay_ms(1);
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
//}
//
//void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size)
//{
//    uint8_t i;
//    for(i=0; i<size; i++)
//    {
//        CLCD_Print(startCol, startRow+i, string[i]);
//    }
//}
//
//int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[])
//{
//    uint8_t i, cgRamAddr;
//    
//    cgRamAddr=0x40+(8*CLCD_storeAddress);
//    
//    if(CLCD_storeAddress>8)
//    {
//        return CLCD_ERR_CGRAM_ADDR_OVF;// CGRAM Address OverFlow
//    }
//
//    for(i=0; i<8; i++)
//    {
//        CLCD_Reg_Write(cgRamAddr);
//        
//        GPIO_SetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS
//        Delay_ms(1);
//        CLCD_Reg_Write(font[i]);
//        Delay_ms(1);
//        GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
//        cgRamAddr++;
//    }
//    
//    return 1;
//}



