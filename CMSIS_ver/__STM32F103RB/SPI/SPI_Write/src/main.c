#include "stm32f10x.h"
#include<stdio.h>
#define TIME_OUT_MAX 2000

SPI_InitTypeDef SPI_EEPROM;
GPIO_InitTypeDef SPI_PIN;
GPIO_InitTypeDef spi_out;

USART_InitTypeDef serial1;
GPIO_InitTypeDef usart_rx;
GPIO_InitTypeDef usart_tx;

void SPI_Write(uint16_t data);
    
int main()
{
uint8_t data=0x01;
volatile int i, j;

/*RCC CLOCK ENABLE*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
/*SPI GPIO INIT*/
    SPI_PIN.GPIO_Mode=GPIO_Mode_AF_PP;
    SPI_PIN.GPIO_Speed=GPIO_Speed_50MHz;
    
    SPI_PIN.GPIO_Pin=GPIO_Pin_5;
    GPIO_Init(GPIOA, &SPI_PIN);// SCK

    SPI_PIN.GPIO_Pin=GPIO_Pin_6;
    GPIO_Init(GPIOA, &SPI_PIN);// MISO

    SPI_PIN.GPIO_Pin=GPIO_Pin_7;
    GPIO_Init(GPIOA, &SPI_PIN);// MOSI

    spi_out.GPIO_Mode=GPIO_Mode_Out_PP;
    spi_out.GPIO_Speed=GPIO_Speed_50MHz;
    spi_out.GPIO_Pin=GPIO_Pin_3;
    GPIO_Init(GPIOA, &spi_out);
    GPIO_ResetBits(GPIOA, spi_out.GPIO_Pin);
    
/*USART GPIO INIT*/
    usart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    usart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &usart_rx);

    usart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    usart_tx.GPIO_Pin=GPIO_Pin_9;
    usart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &usart_tx);
    
/*SPI STRUCT INIT*/
    SPI_EEPROM.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    SPI_EEPROM.SPI_DataSize=SPI_DataSize_8b;
    SPI_EEPROM.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    SPI_EEPROM.SPI_Mode=SPI_Mode_Master;
    SPI_EEPROM.SPI_FirstBit=SPI_FirstBit_MSB;
    SPI_EEPROM.SPI_CPOL=SPI_CPOL_Low;
    SPI_EEPROM.SPI_CPHA=SPI_CPHA_1Edge;
    SPI_Init(SPI1, &SPI_EEPROM);
    
    SPI_Cmd(SPI1, ENABLE);

/*USART STRUCT INIT*/
    serial1.USART_BaudRate=9600;
    serial1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    serial1.USART_Parity=USART_Parity_No;
    serial1.USART_StopBits=USART_StopBits_1;
    serial1.USART_WordLength=USART_WordLength_8b;
    serial1.USART_Mode=( USART_Mode_Tx| USART_Mode_Rx);
    USART_Init(USART1, &serial1);
    USART_Cmd(USART1, ENABLE);

/****************BASE INIT**********************/
    SPI_Write(0x00);
    GPIO_SetBits(GPIOA, spi_out.GPIO_Pin);
    GPIO_ResetBits(GPIOA, spi_out.GPIO_Pin);
    while(1)
    {
        SPI_Write(data);
        data<<=1;
        if(data==0x00)
        {
            data=0x01;
        }
        
        GPIO_SetBits(GPIOA, spi_out.GPIO_Pin);
        GPIO_ResetBits(GPIOA, spi_out.GPIO_Pin);
        for(i=0; i<1000; i++)
            for(j=0; j<5000; j++);

        
        USART_SendData(USART1, 'F');
    }
return 0;
}






#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
