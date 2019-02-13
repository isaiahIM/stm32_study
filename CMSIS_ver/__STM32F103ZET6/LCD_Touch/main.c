#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "my_LCD_fsmc.h"

#define HIGH_DENSITY

#define LCD_TOUCH_READ_X 0X91
#define LCD_TOUCH_READ_Y 0XD0
SPI_InitTypeDef touch;
GPIO_InitTypeDef spi_gpio;// use spi2


#define TIME_OUT 5000

void SPI_WriteByte(uint8_t data);
uint8_t SPI_ReadByte(void);

void Touch_Config(void);
void LCD_Read_Touch_Pos(uint16_t *Xpos, uint16_t *Ypos);

uint16_t LCD_Read_Touch_X(void);
uint16_t LCD_Read_Touch_Y(void); 

int main()
{
    uint16_t touch_x, touch_y;
    
    uint16_t mosi_pin=GPIO_Pin_15, miso_pin=GPIO_Pin_14, clk_pin=GPIO_Pin_13, cs_pin=GPIO_Pin_12;
/*USART RCC_CLOCK CONFIG*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
    
/*SPI RCC_CLOCK ENABLE*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    spi_gpio.GPIO_Mode=GPIO_Mode_AF_PP;
    spi_gpio.GPIO_Speed=GPIO_Speed_50MHz;
    spi_gpio.GPIO_Pin=mosi_pin|miso_pin|clk_pin;
    GPIO_Init(GPIOB, &spi_gpio);

    spi_gpio.GPIO_Mode=GPIO_Mode_Out_PP;
    spi_gpio.GPIO_Pin=cs_pin;
    GPIO_Init(GPIOB, &spi_gpio);

    GPIO_SetBits(GPIOB, cs_pin);
    
    touch.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    touch.SPI_DataSize=SPI_DataSize_8b;
    touch.SPI_Mode=SPI_Mode_Master;
    touch.SPI_CPHA=SPI_CPHA_1Edge;
    touch.SPI_CPOL=SPI_CPOL_Low;
    touch.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    touch.SPI_FirstBit=SPI_FirstBit_MSB;

    SPI_Init(SPI2, &touch);
    SPI_Cmd(SPI2, ENABLE);

    SysTick_Init();
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, 
                                   (USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);
    LCD_Init();
    Delay_ms(1);

    while(1)
    {
        LCD_Read_Touch_Pos(&touch_x, &touch_y);
        printf("Touch pos: %d, %d\n", touch_x, touch_y);
        LCD_Draw_Circle(touch_x, touch_y, 30, 0X3FF);
        
    }
}

void SPI_WriteByte(uint8_t data)
{
    int time=TIME_OUT;
    
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)==1 )// CONNECT ERROR
    {
        time--;
        if(time<=0)
        {
//            while(1);
        }
    }

    SPI_I2S_SendData(SPI2, data);
    Delay_ms(5);
    
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==0 )// DATA SEND ERROR
    {
        time--;
        if(time<=0)
        {
            break;
//            while(1);
        }
    }
}

uint8_t SPI_ReadByte(void)
{
    uint8_t read;
    int time=TIME_OUT;
    
    SPI_WriteByte(0x00);

    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)==1 )
    {
        time--;
        if(time<=0)
        {
//            while(1);
        }
    }

    read=SPI_I2S_ReceiveData(SPI2);
    Delay_ms(5);
    
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==1 )
    {
        time--;
        if(time<=0)
        {
            break;
//            while(1);
        }
    }
    return read;
}


void LCD_Read_Touch_Pos(uint16_t *Xpos, uint16_t *Ypos)
{
    uint16_t z1, z2;
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);// CS

/*TOUCH CHECK*/
    SPI_WriteByte(0XB1);
    z1=(SPI_ReadByte()<<5);
    z1|=(SPI_ReadByte()>>3);

    SPI_WriteByte(0XC1);
    z2=(SPI_ReadByte()<<5);
    z2|=(SPI_ReadByte()>>3);

    printf("z1: %d, z2: %d\n", z1, z2);

    
    if( (z2-z1)<3000)
    {
    SPI_WriteByte(LCD_TOUCH_READ_X);
    
    *Xpos= SPI_ReadByte();
    *Xpos<<=8;
    *Xpos|=SPI_ReadByte();
    
    *Xpos>>=3;
    Delay_ms(5);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_12);// CS

    Delay_ms(5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);// CS
    
    SPI_WriteByte(LCD_TOUCH_READ_Y);
    
    *Ypos= SPI_ReadByte();
    *Ypos<<=8;
    *Ypos|=SPI_ReadByte();
    
    *Ypos>>=3;
    }
    
    GPIO_SetBits(GPIOB, GPIO_Pin_12);// CS
}

uint16_t LCD_Read_Touch_X(void)
{
    uint16_t data;

    GPIO_ResetBits(GPIOB, GPIO_Pin_12);// CS
    
    SPI_WriteByte(LCD_TOUCH_READ_X);
    
    data= SPI_ReadByte();
    data<<=8;
    data|=SPI_ReadByte();
    data>>=3;
    
    GPIO_SetBits(GPIOB, GPIO_Pin_12);// CS
    
    return data;
}

uint16_t LCD_Read_Touch_Y(void)
{
    uint16_t data;
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);// CS
    
    SPI_WriteByte(LCD_TOUCH_READ_Y);
    
    data= SPI_ReadByte();
    data<<=8;
    data|=SPI_ReadByte();
    data>>=3;
    GPIO_SetBits(GPIOB, GPIO_Pin_12);// CS
    
    return data;
}


