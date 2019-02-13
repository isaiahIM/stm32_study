#include "I2C_gpio.h"
#include "SysTick.h"

GPIO_TypeDef *SCL_PORT=GPIOB;
GPIO_TypeDef *SDA_PORT=GPIOB;

uint16_t SCL_PIN=GPIO_Pin_6;
uint16_t SDA_PIN=GPIO_Pin_7;

void I2C_Config(void)
{
	GPIO_InitTypeDef i2c1_gpio;
	
	SysTick_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	i2c1_gpio.GPIO_Mode=GPIO_Mode_Out_OD;
    i2c1_gpio.GPIO_Speed=GPIO_Speed_2MHz;
    
    i2c1_gpio.GPIO_Pin=SCL_PIN;
    GPIO_Init(SCL_PORT, &i2c1_gpio);// SCL
    GPIO_ResetBits(SCL_PORT, SCL_PIN);
    
    i2c1_gpio.GPIO_Pin=SDA_PIN;
    GPIO_Init(SDA_PORT, &i2c1_gpio);// SDA
    
}
void I2C_Start(uint8_t deviceAddr, uint8_t I2C_Direction)
{
/*START*/
    GPIO_SetBits(SDA_PORT, SDA_PIN);
    Delay_ms(1);
    GPIO_SetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);
    GPIO_ResetBits(SDA_PORT, SDA_PIN);
    Delay_ms(1);
    GPIO_ResetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);

    if(I2C_Direction==I2C_Direction_Receiver)
    {
        I2C_WriteByte(deviceAddr|0x01);
    }
    else
    {   
        I2C_WriteByte(deviceAddr);
    }
    I2C_GetAck();
}

void I2C_WriteByte(uint8_t data)
{
    uint8_t mask=0x80, i;
  
    for(i=0; i<8; i++)
    {
        if( (data&mask) !=0)
        {
            GPIO_SetBits(SDA_PORT, SDA_PIN);
            Delay_ms(1);
            GPIO_SetBits(SCL_PORT, SCL_PIN);
        }
        else
        {
            GPIO_ResetBits(SDA_PORT, SDA_PIN);
            Delay_ms(1);
            GPIO_SetBits(SCL_PORT, SCL_PIN);
        }

        Delay_ms(1);
        mask>>=1;
        GPIO_ResetBits(SCL_PORT, SCL_PIN);
        Delay_ms(1);
    }
    I2C_GetAck();
}

uint8_t I2C_ReadByte(void)
{
    int i;
    uint8_t readData=0x00;
    GPIO_InitTypeDef sda;
    
    I2C_GetAck();

    sda.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    sda.GPIO_Speed=GPIO_Speed_2MHz;
    sda.GPIO_Pin=SDA_PIN;
    GPIO_Init(SDA_PORT, &sda);

    for(i=0; i<8; i++)
    {
        GPIO_SetBits(SCL_PORT, SCL_PIN);
        Delay_ms(1);
        GPIO_ResetBits(SCL_PORT, SCL_PIN);
        Delay_ms(1);
        readData<<=1;
        readData|=GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN);
    }
    sda.GPIO_Mode=GPIO_Mode_Out_OD;
    GPIO_Init(SDA_PORT, &sda);
    
    return readData;
}


void I2C_Stop(void)
{

    GPIO_ResetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);
    GPIO_ResetBits(SDA_PORT, SDA_PIN);
    Delay_ms(1);
    GPIO_SetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);
    GPIO_SetBits(SDA_PORT, SDA_PIN);
    Delay_ms(1);
    
}


void I2C_SendACK(FunctionalState ackState)
{
    if(ackState==ENABLE)
    {
        GPIO_SetBits(SDA_PORT, SDA_PIN);
    }
    else
    {
        GPIO_ResetBits(SDA_PORT, SDA_PIN);
    }
    
    GPIO_SetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);
    GPIO_ResetBits(SCL_PORT, SCL_PIN);
}

void I2C_GetAck(void)
{
    GPIO_SetBits(SCL_PORT, SCL_PIN);
    Delay_ms(1);
    GPIO_ResetBits(SCL_PORT, SCL_PIN);
}




