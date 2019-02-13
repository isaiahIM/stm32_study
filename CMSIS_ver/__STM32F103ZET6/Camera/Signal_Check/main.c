#include "usart.h"
#include "SysTick.h"
#include "debug_support.h"
#include "stm32f10x.h"
#include "I2C_gpio.h"


#define HIGH_DENSITY

#define CAMERA_ADDR 0X42
#define OV7670_COUNT_ENABLE 1
#define OV7670_COUNT_DISABLE 0

GPIO_TypeDef *XCL_PORT=GPIOA;
uint16_t XCL_PIN= GPIO_Pin_1;

TIM_TimeBaseInitTypeDef timBase;
TIM_OCInitTypeDef timeOC;

uint32_t g_pclk=0;
uint32_t g_hsync=0;
uint8_t g_cntState=0;

void OV7670_Init(void);
void OV7670_WriteReg(uint8_t reg, uint8_t data);
void OV7670_ReadReg(uint8_t reg, uint8_t *readData);

const uint8_t change_reg[167][2]=
{
    
     /*  {0x12, 0x46},*/

       {0x3a, 0x04},
	{0x40, 0xd0},
	{0x12, 0x14},
	{0x32, 0x80},
	{0x17, 0x16},
	{0x18, 0x04},
	{0x19, 0x02},
	{0x1a, 0x7b},//0x7a,
	{0x03, 0x06},//0x0a,
	{0x0c, 0x00},
	{0x3e, 0x00},//
	{0x70, 0x00},
	{0x71, 0x00},
	{0x72, 0x11},
	{0x73, 0x00},//
	{0xa2, 0x02},
	{0x11, 0x00},
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},
	{0x7d, 0x3c},
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},
	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},
	{0x87, 0xc4},
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x20},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},
	{0x24, 0x75},
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},
	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},
	{0xaa, 0x94},
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},
	{0x1e, 0x37},//0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
	{0x35, 0x0b},
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},//
	{0x3c, 0x78},
	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x0c},///////////////////////
	{0x6b, 0x10},//PLL
	{0x74, 0x19},
	{0x8d, 0x4f},
	{0x8e, 0x00},
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
	{0xb3, 0x82},
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},
	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
	{0x5b, 0x44},
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},
	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},
	{0x6e, 0x11},
	{0x6f, 0x9f},//0x9e for advance AWB
	{0x6a, 0x40},
	{0x01, 0x40},
	{0x02, 0x40},
	{0x13, 0xe7},
	{0x15, 0x60},// 0x00 << com10
	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},
	{0x58, 0x9e},	
	{0x41, 0x08},
	{0x3f, 0x00},
	{0x75, 0x05},
	{0x76, 0xe1},
	{0x4c, 0x00},
	{0x77, 0x01},
	{0x3d, 0xc2},	//0xc0,
	{0x4b, 0x09},
	{0xc9, 0x60},
	{0x41, 0x38},
	{0x56, 0x40},//0x40,  change according to Jim's request	
	{0x34, 0x11},
	{0x3b, 0x02},//0x00,//0x02,
	{0xa4, 0x89},//0x88,
	{0x96, 0x00},
	{0x97, 0x30},
	{0x98, 0x20},
	{0x99, 0x30},
	{0x9a, 0x84},
	{0x9b, 0x29},
	{0x9c, 0x03},
	{0x9d, 0x4c},
	{0x9e, 0x3f},
	{0x78, 0x04},	
	{0x79, 0x01},
	{0xc8, 0xf0},
	{0x79, 0x0f},
	{0xc8, 0x00},
	{0x79, 0x10},
	{0xc8, 0x7e},
	{0x79, 0x0a},
	{0xc8, 0x80},
	{0x79, 0x0b},
	{0xc8, 0x01},
	{0x79, 0x0c},
	{0xc8, 0x0f},
	{0x79, 0x0d},
	{0xc8, 0x20},
	{0x79, 0x09},
	{0xc8, 0x80},
	{0x79, 0x02},
	{0xc8, 0xc0},
	{0x79, 0x03},
	{0xc8, 0x40},
	{0x79, 0x05},
	{0xc8, 0x30},
	{0x79, 0x26},
	{0x09, 0x03},
	{0x55, 0x00},
	{0x56, 0x40},	
	{0x3b, 0x42},//0x82,//0xc0,//0xc2,
    
};


int main()
{
    GPIO_InitTypeDef tim_gpio;
    
    GPIO_InitTypeDef clk_gpio;
    EXTI_InitTypeDef clk_exti;
    
    uint8_t read=0;
    
/*RCC CLOCK SET*/    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
/*I2C SET*/
    I2C_Config();

/*Interrupt SET*/
    /*PCLK - EXTI0*/
    clk_gpio.GPIO_Mode=GPIO_Mode_IPU;
    clk_gpio.GPIO_Pin=GPIO_Pin_0;
    clk_gpio.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &clk_gpio);

    clk_exti.EXTI_Line=EXTI_Line0;
    clk_exti.EXTI_Mode=EXTI_Mode_Interrupt;
    clk_exti.EXTI_Trigger=EXTI_Trigger_Falling;
    clk_exti.EXTI_LineCmd=ENABLE;
    EXTI_Init(&clk_exti);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    /*HREF(H sync) - EXTI3*/
    clk_gpio.GPIO_Pin=GPIO_Pin_3;
    GPIO_Init(GPIOA, &clk_gpio);

    clk_exti.EXTI_Line=EXTI_Line3;
    clk_exti.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
    EXTI_Init(&clk_exti);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

    /*VREF(V sync)*/
//    clk_gpio.GPIO_Pin=GPIO_Pin_4;
//    GPIO_Init(GPIOA, &clk_gpio);
//
//    clk_exti.EXTI_Line=EXTI_Line4;
//    EXTI_Init(&clk_exti);
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);

    
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
    USART_BaseConfig(USART_NUM_1, USART1, 38400, USART_WordLength_8b, (USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);

    printf("OV 7670 Initalizing...\n");
    OV7670_Init();
    printf("Initalizing finish\n");
    
    OV7670_ReadReg(0x0a, &read);
    printf("readData: %x\n", read);
    OV7670_ReadReg(0x0b, &read);
    printf("readData2: %x\n\n\n", read);
    Delay_ms(500);
    
    /*NVIC SET*/
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    while(1)
    {
        
    }
}


/****************IRQ HANDLER******************/

void EXTI0_IRQHandler(void) 
{
    if(g_cntState==OV7670_COUNT_ENABLE) 
    {
        g_pclk+=1;
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI3_IRQHandler(void)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==Bit_RESET)// Rising Interrupt
    {
        g_cntState=OV7670_COUNT_ENABLE;
    }

    else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==Bit_SET)// Falling Interrupt
    {
        g_cntState=OV7670_COUNT_DISABLE;
        printf("pclock: %d\n", g_pclk);
        g_pclk=0;
    }

    EXTI_ClearITPendingBit(EXTI_Line3);
}


/*************USER FUNCTION*****************/

void OV7670_Init(void)
{
    int i;
    
    for(i=0; i<167; i++)
    {
        OV7670_WriteReg(change_reg[i][0], change_reg[i][1]);
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




