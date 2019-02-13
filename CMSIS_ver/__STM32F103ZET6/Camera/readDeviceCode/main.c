#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "I2C_gpio.h"


#define HIGH_DENSITY
#define CAMERA_ADDR 0X42


GPIO_TypeDef *XCL_PORT=GPIOA;
uint16_t XCL_PIN= GPIO_Pin_1;

TIM_TimeBaseInitTypeDef timBase;
TIM_OCInitTypeDef timeOC;

void OV7670_WriteReg(uint8_t reg, uint8_t data);
void OV7670_ReadReg(uint8_t reg, uint8_t *readData);


int main()
{
    GPIO_InitTypeDef tim_gpio;
    
    uint8_t read=0;
    
/*RCC CLOCK SET*/    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

/*I2C SET*/
    I2C_Config();


/*TIMER SET*/

    /*TIMER GPIO SET*/
    tim_gpio.GPIO_Mode=GPIO_Mode_AF_PP;
    tim_gpio.GPIO_Pin=XCL_PIN;
    tim_gpio.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(XCL_PORT, &tim_gpio);

    /*TIMER BASE(OVF) SET*/
    timBase.TIM_ClockDivision=0;
    timBase.TIM_CounterMode=TIM_CounterMode_Up;
    timBase.TIM_Period=3;// 10us ovf
    timBase.TIM_Prescaler=0;
    timBase.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2, &timBase);
    TIM_PrescalerConfig(TIM2, 359, TIM_PSCReloadMode_Update);// 5us count
    
    /*TIMER Output Compare SET(include H/W TIMER)*/
    timeOC.TIM_OCMode=TIM_OCMode_PWM1;
    timeOC.TIM_OutputState=TIM_OutputState_Enable;
    timeOC.TIM_OutputNState = TIM_OutputState_Disable;
    timeOC.TIM_OCPolarity=TIM_OCPolarity_High;
    timeOC.TIM_OCNPolarity=TIM_OCNPolarity_High;
    timeOC.TIM_Pulse=2;
    TIM_OC2Init(TIM2, &timeOC);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_Cmd(TIM2, ENABLE);

    
    SysTick_Init();
    USART_BaseConfig(USART_NUM_1, USART1, 9600, USART_WordLength_8b, (USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);

    
    while(1)
    {
        OV7670_ReadReg(0x0a, &read);
        printf("readData: %x\n", read);

        OV7670_ReadReg(0x0b, &read);
        printf("readData2: %x\n\n\n", read);
        Delay_ms(500);
    }
}


void OV7670_WriteReg(uint8_t reg, uint8_t data)
{
    I2C_Start(CAMERA_ADDR, I2C_Direction_Transmitter);
    I2C_WriteByte(reg);
    I2C_WriteByte(data);
    I2C_Stop();
}

void OV7670_ReadReg(uint8_t reg, uint8_t *readData)
{    
    I2C_Start(CAMERA_ADDR, I2C_Direction_Transmitter);
    
    I2C_WriteByte(reg);

    I2C_Stop();

    Delay_ms(5);
    
    I2C_Start(CAMERA_ADDR, I2C_Direction_Receiver);

    *readData=I2C_ReadByte();
    
    I2C_SendACK(DISABLE);
    
    I2C_Stop();
}



