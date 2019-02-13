#include "my_LCD_gpio.h"
#include <stdio.h>
GPIO_InitTypeDef lcd;

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

GPIO_TypeDef* LCD_CS_Port= GPIOG;
uint16_t LCD_CS_Pin= GPIO_Pin_12;

GPIO_TypeDef* LCD_RS_Port= GPIOF;
uint16_t LCD_RS_Pin= GPIO_Pin_0;

GPIO_TypeDef* LCD_WR_Port= GPIOD;
uint16_t LCD_WR_Pin= GPIO_Pin_5;

GPIO_TypeDef* LCD_RD_Port= GPIOD;
uint16_t LCD_RD_Pin= GPIO_Pin_4;

GPIO_TypeDef* LCD_BackLight_Port= GPIOA;
uint16_t LCD_BackLight_Pin= GPIO_Pin_1;

void LCD_Init(void)
{
    int i;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    
    lcd.GPIO_Mode=GPIO_Mode_Out_PP;
    lcd.GPIO_Speed=GPIO_Speed_50MHz;
    
	for(i=0; i<LCD_DATA_NUM; i++)// data pin config
    {
        lcd.GPIO_Pin= LCD_DATA_Pin[i];
        GPIO_Init(LCD_DATA_Port[i], &lcd);
        
        GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
    }

    lcd.GPIO_Pin=LCD_CS_Pin;
    GPIO_Init(LCD_CS_Port, &lcd);
    GPIO_SetBits(LCD_CS_Port, LCD_CS_Pin);

    lcd.GPIO_Pin=LCD_RS_Pin;
    GPIO_Init(LCD_RS_Port, &lcd);
    GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);
    
    lcd.GPIO_Pin=LCD_WR_Pin;
    GPIO_Init(LCD_WR_Port, &lcd);
    GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
    
    lcd.GPIO_Pin=LCD_RD_Pin;
    GPIO_Init(LCD_RD_Port, &lcd);
    GPIO_SetBits(LCD_RD_Port, LCD_RD_Pin);
    
    lcd.GPIO_Pin=LCD_BackLight_Pin;
    GPIO_Init(LCD_BackLight_Port, &lcd);
    GPIO_SetBits(LCD_BackLight_Port, LCD_BackLight_Pin);
    
/*LCD BASE CONTROL*/
    LCD_WriteData(0x00, 0x0001);
    LCD_WriteData(0x01, 0x0100);
    LCD_WriteData(0x02, 0x0700);
    LCD_WriteData(0x03, 0x1030);
    LCD_WriteData(0x04, 0x0000);
    LCD_WriteData(0x08, 0x0202);
    LCD_WriteData(0x09, 0x0000);
    LCD_WriteData(0x0a, 0x0000);
    LCD_WriteData(0x0c, 0x0000);
    LCD_WriteData(0x0d, 0x0000);
    LCD_WriteData(0x0f, 0x0000);
/*POWER CONTROL*/
    LCD_WriteData(0x10, 0x0000);
    LCD_WriteData(0x11, 0x0000);
    LCD_WriteData(0x12, 0x0000);
    LCD_WriteData(0x13, 0x0000);
//    Delay_ms(2);
    LCD_WriteData(0x10, 0x17b0);
    LCD_WriteData(0x11, 0x0037);
//    Delay_ms(2);
    LCD_WriteData(0x12, 0x0138);
//    Delay_ms(2);
    LCD_WriteData(0x13, 0x1700);
    LCD_WriteData(0x29, 0x000d);
//    Delay_ms(2);
    LCD_WriteData(0x20, 0x0000);
    LCD_WriteData(0x21, 0x0000);
/*SET GAMMA*/
    LCD_WriteData(0x30, 0x0001);
    LCD_WriteData(0x31, 0x0606);
    LCD_WriteData(0x32, 0x0304);
    LCD_WriteData(0x33, 0x0202);
    LCD_WriteData(0x34, 0x0202);
    LCD_WriteData(0x35, 0x0103);
    LCD_WriteData(0x36, 0x011d);
    LCD_WriteData(0x37, 0x0404);
    LCD_WriteData(0x38, 0x0404);
    LCD_WriteData(0x39, 0x0404);
    LCD_WriteData(0x3c, 0x0700);
    LCD_WriteData(0x3d, 0x0a1f);
/*SET GRAM AREA*/
    LCD_WriteData(0X50, 0X0000);
    LCD_WriteData(0X51, 0X00EF);
    LCD_WriteData(0X52, 0X0000);
    LCD_WriteData(0X53, 0X013F);
    LCD_WriteData(0X60, 0X2700);
    LCD_WriteData(0X61, 0X0001);
    LCD_WriteData(0X6A, 0X0000);
/*Partial DISPLAY*/
    LCD_WriteData(0X90, 0X0010);
    LCD_WriteData(0X92, 0X0000);
    LCD_WriteData(0X93, 0X0003);
    LCD_WriteData(0X95, 0X0101);
    LCD_WriteData(0X97, 0X0000);
    LCD_WriteData(0X98, 0X0000);
/*Plan Control*/
    LCD_WriteData(0X07, 0X0021);
//    Delay_ms(1);
    LCD_WriteData(0X07, 0X0031);
//    Delay_ms(1);
    LCD_WriteData(0X07, 0X0173);
//    Delay_ms(10);
    
}

void LCD_WriteData(uint16_t Address, uint16_t data)
{
     int i;

    GPIO_ResetBits(LCD_CS_Port, LCD_CS_Pin);
    GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);
    GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);// write mode
//    Delay_ms(1);

    /*Address*/
    for(i=0; i<LCD_DATA_NUM; i++)
    {    
        if( (Address&(0x01<<i) ) !=0)// if data is 1
        {
            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
        else
        {
            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
    }

    GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
//    Delay_ms(1);
    GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);

    GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);
    for(i=0; i<LCD_DATA_NUM; i++)
    {       
        if( ( data&(0x01<<i) )  !=0)
        {
            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
        else
        {
            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
    }
    GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
//    Delay_ms(1);
    GPIO_SetBits(LCD_CS_Port, LCD_CS_Pin);
}

void LCD_Write_MultiData(uint16_t Address, uint16_t data[], uint32_t dataSize)
{
    uint32_t i, j;
    
    GPIO_ResetBits(LCD_CS_Port, LCD_CS_Pin);
    GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);
    GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);// write mode
//    Delay_ms(1);
    
    LCD_ChangePort_Dir(LCD_Port_Dir_Write);

    /*Address*/
    for(i=0; i<8; i++)
    {    
        if( (Address&(0x01<<i) ) !=0)// if data is 1
        {
            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
        else
        {
            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
    }

    GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
//    Delay_ms(1);
    GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);

    for(i=0; i<dataSize; i++)
    {
        GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);
        for(j=0; j<LCD_DATA_NUM; j++)
        {
            if( ( (data[i])&(0x01<<j) )  !=0)
            {
                GPIO_SetBits(LCD_DATA_Port[j], LCD_DATA_Pin[j]);
            }
            else
            {
                GPIO_ResetBits(LCD_DATA_Port[j], LCD_DATA_Pin[j]);
            }
        }
        GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
    }
    GPIO_SetBits(LCD_CS_Port, LCD_CS_Pin);
}

uint16_t LCD_ReadData(uint16_t Address)
{
    int i, j;
    uint16_t data=0x00;

    GPIO_ResetBits(LCD_CS_Port, LCD_CS_Pin);
    GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);
    GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);// write mode
//    Delay_ms(1);

    /*Address*/
    for(i=0; i<8; i++)
    {    
        if( (Address&(0x01<<i) ) !=0)// if data is 1
        {
            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
        else
        {
            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
    }
    GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
//    Delay_ms(1);
    GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);
//    Delay_ms(1);

    LCD_ChangePort_Dir(LCD_Port_Dir_Read);
    GPIO_ResetBits(LCD_RD_Port, LCD_RD_Pin);
//    Delay_ms(1);
    GPIO_SetBits(LCD_RD_Port, LCD_RD_Pin);
    
    for(j=LCD_DATA_NUM-1; j>=0; j--)
    {
        data<<=1;
        data|=GPIO_ReadInputDataBit(LCD_DATA_Port[j], LCD_DATA_Pin[j]);
    }
    return data;
}

void LCD_ChangePort_Dir(int LCD_Port_Dir)
{
    int j;
    if(LCD_Port_Dir==LCD_Port_Dir_Write)
    {
        lcd.GPIO_Mode=GPIO_Mode_Out_PP;
        lcd.GPIO_Speed=GPIO_Speed_50MHz;
        
        for(j=0; j<LCD_DATA_NUM; j++)// data pin config
        {
            lcd.GPIO_Pin= LCD_DATA_Pin[j];
            GPIO_Init(LCD_DATA_Port[j], &lcd);        
        }
    }
    else
    {
        lcd.GPIO_Mode=GPIO_Mode_IN_FLOATING;// mode change
        for(j=0; j<LCD_DATA_NUM; j++)// data pin config
        {
            lcd.GPIO_Pin= LCD_DATA_Pin[j];
            GPIO_Init(LCD_DATA_Port[j], &lcd);
        }
    }
}

void LCD_GotoXY(uint32_t start_x, uint32_t start_y)
{
    LCD_WriteData(0x20, start_x);
    LCD_WriteData(0x21, start_y);
}


void LCD_Draw_Square(uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint16_t fill)
{
    int x, y;
    for(x=start_x; x<end_x; x++)
    {
        for(y=start_y; y<end_y; y++)
        {
            LCD_GotoXY(x, y);
            LCD_WriteData(LCD_RA_SET_COLOR, fill);
        }
    }
}

void LCD_Draw_Circle(uint32_t center_x, uint32_t center_y, uint32_t radius, uint16_t fill)
{
    uint32_t r, x, y, angle;
    double radian;

    for(r=0; r<radius; r++)
    {
        for(angle=0; angle<360; angle++)
        {
            radian=angle*(3.14159/180);
            x=r*cos(radian)+center_x;
            y=r*sin(radian)+center_y;
            LCD_GotoXY((uint32_t)x, (uint32_t)y);
            LCD_WriteData(LCD_RA_SET_COLOR, fill);
        }
    }
}

void LCD_PrintBMP(uint8_t bmp[])
{
    uint32_t i;
    uint8_t colorCount=0, data, r, g, b;
    uint8_t RGB_Bit_Format, Pixel_start;
    uint16_t color=0x00;
    
    RGB_Bit_Format=bmp[BMP_RGB_BIT_CHECK];
    Pixel_start=bmp[BMP_PIXEL_START];
    
    if(RGB_Bit_Format==24)// if RGB data is 3byte
    {
        for(i=0; i<(240*320*3); i++)
        {
            data=bmp[Pixel_start+i];
            /*convert 2byte*/
            if(colorCount==BMP_COLOR_R)
            {
                r=(uint8_t)( (float)data*0x75);
            }
            
            else if(colorCount==BMP_COLOR_G)
            {
                g=(uint8_t)( (float)data*0x75);
            }

            else if(colorCount==BMP_COLOR_B)
            {
                b=(uint8_t)( (float)data*0x75);
            }
            colorCount+=1;
            
            if(colorCount==3)
            {
                color=b;
                color|=(g<<6);
                color|=(r<<12);
                
                LCD_WriteData(LCD_RA_SET_COLOR, color);
                colorCount=0;
                color=0x00;
            }
        }
    }

    else if(RGB_Bit_Format==16)// if RGB data is 2byte 
    {
        for(i=0; i<(240*320*2); i+=2)
        {
        	/*
			bitmap is r-g-b color sequence
			this algorithm is 565 format. 565 formaat is
			B: 5bit
			G: 6bit
			R: 5bit
			
			Color: MSB RRRRRGGGGGGBBBBB LSB
			
			and mask r, g, b
			*/
            r= (bmp[Pixel_start+i+1])&0XF8;// mask R
            g= ( ( (bmp[Pixel_start+i])&0XE0)>>5) | ( (bmp[Pixel_start+i+1])&0X07)<<3;// mask G
            b= ( (bmp[Pixel_start+i])&0X1F);// mask B

            color=(r<<8);// r
            color|=(g<<5);// g
            color|=b;// b
            LCD_WriteData(LCD_RA_SET_COLOR, color);
        }
    }
}



