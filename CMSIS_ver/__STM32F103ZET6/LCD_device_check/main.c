#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"

#define LCD_DATA_NUM 16
#define HIGH_DENSITY


GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;
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


void LCD_WriteData(uint16_t data);
uint16_t LCD_ReadData(void);

int main()
{
    int i, j;
    uint16_t data=0x00, readData;
    
/*LCD RCC_CLOCK CONFIG*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

/*USART RCC_CLOCK CONFIG*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    lcd.GPIO_Mode=GPIO_Mode_Out_PP;
    lcd.GPIO_Speed=GPIO_Speed_50MHz;
	
/*LCD GPIO CONFIG*/
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
    
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);
    SysTick_Init();

    while(1)
    {
        GPIO_ResetBits(LCD_CS_Port, LCD_CS_Pin);
        GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
        Delay_ms(1);
        GPIO_ResetBits(LCD_WR_Port, LCD_WR_Pin);// write mode
        Delay_ms(1);

        /*Register Send*/
        LCD_WriteData(data);// register address send
        GPIO_SetBits(LCD_WR_Port, LCD_WR_Pin);
        Delay_ms(1);
        GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);

        /*Register Read*/
        lcd.GPIO_Mode=GPIO_Mode_IN_FLOATING;// mode change
        for(j=0; j<LCD_DATA_NUM; j++)// data pin config
        {
            lcd.GPIO_Pin= LCD_DATA_Pin[j];
            GPIO_Init(LCD_DATA_Port[j], &lcd);
        }

        GPIO_ResetBits(LCD_RD_Port, LCD_RD_Pin);
        Delay_ms(1);
        GPIO_SetBits(LCD_RD_Port, LCD_RD_Pin);
        Delay_ms(1);
        readData=LCD_ReadData();

        Delay_ms(2);
        GPIO_SetBits(LCD_CS_Port, LCD_CS_Pin);

        printf("LCD device data: %x\n", readData);
    }
}


void LCD_WriteData(uint16_t data)
{
    int i, j;

    lcd.GPIO_Mode=GPIO_Mode_Out_PP;
    lcd.GPIO_Speed=GPIO_Speed_50MHz;
    
    for(j=0; j<LCD_DATA_NUM; j++)// data pin config
    {
        lcd.GPIO_Pin= LCD_DATA_Pin[j];
        GPIO_Init(LCD_DATA_Port[j], &lcd);        
    }
    
    for(i=0; i<LCD_DATA_NUM; i++)
    {    
        if( (data&(0x01<<i) )==1)// if data is 1
        {
            GPIO_SetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
        else
        {
            GPIO_ResetBits(LCD_DATA_Port[i], LCD_DATA_Pin[i]);
        }
    }
}

uint16_t LCD_ReadData(void)
{
    int j;
    uint16_t data=0x00;

    for(j=LCD_DATA_NUM-1; j>=0; j--)
    {
        data<<=1;
        data|=GPIO_ReadInputDataBit(LCD_DATA_Port[j], LCD_DATA_Pin[j]);
    }
    return data;
}
