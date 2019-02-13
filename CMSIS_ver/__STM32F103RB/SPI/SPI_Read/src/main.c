#include "stm32f10x.h"
#include "usart.h"
#include "debug_support.h"
#include "SysTick.h"

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;
GPIO_InitTypeDef spi_gpio;
GPIO_InitTypeDef ss;

SPI_InitTypeDef spi;

int SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t data);
int SPI_ReadByte(SPI_TypeDef* SPIx, uint8_t *read_data);

#define TIME_OUT_MAX 2000 
#define SPI_CONNECT_ERROR 2
#define SPI_SUCCESS 0

#define SPI_WRITE_ERROR 1
#define SPI_READ_ERROR 1


#define SPI_ENABLE 0X06
#define SPI_WRITE_MODE 0X02
#define SPI_READ_MODE 0X03

int main()
{
    uint8_t wordAddr=0x00, data=0x30, readData;
    int spi_error;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    ss.GPIO_Mode=GPIO_Mode_Out_PP;
    ss.GPIO_Speed=GPIO_Speed_50MHz;
    ss.GPIO_Pin=GPIO_Pin_1;
    GPIO_Init(GPIOA, &ss);
    GPIO_SetBits(GPIOA, ss.GPIO_Pin);

/*SPI GPIO CONFIG*/
    spi_gpio.GPIO_Mode=GPIO_Mode_AF_PP;
    spi_gpio.GPIO_Speed=GPIO_Speed_50MHz;
    spi_gpio.GPIO_Pin=(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);// SCK, MOSI, MISO
    GPIO_Init(GPIOA, &spi_gpio);
    
/*SPI CONFIG*/
    spi.SPI_Mode=SPI_Mode_Master;
    spi.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    spi.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize=SPI_DataSize_8b;
    spi.SPI_CPHA=SPI_CPHA_1Edge;
    spi.SPI_CPOL=SPI_CPOL_Low;
    spi.SPI_FirstBit=SPI_FirstBit_MSB;
    SPI_Init(SPI1, &spi);
    SPI_Cmd(SPI1, ENABLE);


    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Tx|USART_Mode_Rx), USART_Parity_No, USART_StopBits_1);
    SysTick_Init();
    GPIO_SetBits(GPIOA, ss.GPIO_Pin);
   while(1)
   {
        GPIO_ResetBits(GPIOA, ss.GPIO_Pin);// cs low
        spi_error= SPI_WriteByte(SPI1, SPI_ENABLE);
        printf("enable: errorCode: %d\n", spi_error);
        GPIO_SetBits(GPIOA, ss.GPIO_Pin);// cs high

        GPIO_ResetBits(GPIOA, ss.GPIO_Pin);
        spi_error= SPI_WriteByte(SPI1, SPI_WRITE_MODE);
        printf("write mode: errorCode: %d\n", spi_error);
        
        spi_error= SPI_WriteByte(SPI1, wordAddr);
        printf("Word: errorCode: %d\n", spi_error);
        
        spi_error= SPI_WriteByte(SPI1, data);
        printf("write mode: errorCode: %d\n", spi_error);
        GPIO_ResetBits(GPIOA, ss.GPIO_Pin);
        
        printf("Write SUCCESS!!\n\n");
        Delay_ms(1000);

        GPIO_ResetBits(GPIOA, ss.GPIO_Pin);
        spi_error= SPI_WriteByte(SPI1, SPI_READ_MODE);
        printf("read mode: errorCode: %d\n", spi_error);
        
        spi_error= SPI_WriteByte(SPI1, wordAddr);
        printf("Word: errorCode: %d\n", spi_error);
        
        spi_error= SPI_WriteByte(SPI1, 0x00);// dummy data send
        printf("dummy send: errorCode: %d\n", spi_error);

        spi_error= SPI_ReadByte(SPI1, &readData);// dummy data send
        printf("read: errorCode: %d\n", spi_error);
        GPIO_SetBits(GPIOA, ss.GPIO_Pin);
        
        printf("ReadData: %d\n\n", readData);
   }
   
}

int SPI_WriteByte(SPI_TypeDef* SPIx, uint16_t data)
{
    volatile int timeOut=TIME_OUT_MAX;

    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)==1)// spi bus is busy
    {
        timeOut--;
        if(timeOut==0)
        {
           return SPI_CONNECT_ERROR;// return 0
        }
    }

    SPI_I2S_SendData(SPIx, data);

    timeOut=TIME_OUT_MAX;
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==0)// spi TX not finish
    {
        timeOut--;
        if(timeOut==0)
        {
           return SPI_WRITE_ERROR;// return 0
        }
    }

    return SPI_SUCCESS;// return 1
}

int SPI_ReadByte(SPI_TypeDef* SPIx, uint8_t *read_data)
{
    volatile int timeOut=TIME_OUT_MAX;

    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)==0)// spi bus is busy
    {
        timeOut--;
        if(timeOut==0)
        {
           return SPI_CONNECT_ERROR;// return 0
        }
    }
    
    *read_data=SPI_I2S_ReceiveData(SPIx);

    timeOut=TIME_OUT_MAX;
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==1)// spi RX not finish
    {
        timeOut--;
        if(timeOut==0)
        {
           return SPI_READ_ERROR;// return 0
        }
    }

    return SPI_SUCCESS;
}

