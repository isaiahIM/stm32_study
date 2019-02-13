#include "My_I2C.h"

GPIO_InitTypeDef I2C_GPIO;
GPIO_InitTypeDef usart_rx;
GPIO_InitTypeDef usart_tx;

I2C_InitTypeDef eeprom;
USART_InitTypeDef serial1;

int main(void)
{
    char data='A', readData=0;
    volatile int i, j;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// GPIO A Clock Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// GPIO B Clock Enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);// I2C1 Clock enable

    I2C_GPIO.GPIO_Mode=GPIO_Mode_AF_OD;
    I2C_GPIO.GPIO_Pin=GPIO_Pin_6;
    I2C_GPIO.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &I2C_GPIO);// SCL

    I2C_GPIO.GPIO_Pin=GPIO_Pin_7;
    GPIO_Init(GPIOB, &I2C_GPIO);// SDA

    usart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    usart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &usart_rx);

    usart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    usart_tx.GPIO_Pin=GPIO_Pin_9;
    usart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &usart_tx);

    serial1.USART_BaudRate=9600;
    serial1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    serial1.USART_Parity=USART_Parity_No;
    serial1.USART_StopBits=USART_StopBits_1;
    serial1.USART_WordLength=USART_WordLength_8b;
    serial1.USART_Mode=( USART_Mode_Tx| USART_Mode_Rx);
    USART_Init(USART1, &serial1);
    USART_Cmd(USART1, ENABLE);

    I2C_Config(&eeprom, I2C_Ack_Disable, I2C_AcknowledgedAddress_7bit, 100000, I2C_DutyCycle_2, I2C_Mode_I2C);
/*************************************BASE INIT (UP) ******************************/

while(1)
{
I2C_Start(DEVICE_ADDRESS, I2C_Direction_Transmitter);
I2C_Write(0x01, data);

I2C_Stop();
for(i=0; i<100; i++)
    for(j=0; j<1000; j++);

readData=I2C_Read(0x01);
//USART_SendData(USART1, readData);
printf("%c ", readData);
data++;
for(i=0; i<1000; i++)
    for(j=0; j<5000; j++);
}

}



