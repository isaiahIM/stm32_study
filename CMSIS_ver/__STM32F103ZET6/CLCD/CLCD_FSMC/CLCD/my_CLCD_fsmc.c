#include "my_CLCD_fsmc.h"

GPIO_TypeDef* CLCD_DATA_Port[CLCD_DATA_NUM]=
{
    GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE,
};
uint16_t CLCD_DATA_Pin[CLCD_DATA_NUM]=
{
    GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_0, GPIO_Pin_1,
    GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, 
};

/************USER DEFINE**************/
GPIO_TypeDef* CLCD_RW_Port=GPIOD;
uint16_t CLCD_RW_Pin=GPIO_Pin_7;

GPIO_TypeDef* CLCD_EN_Port=GPIOD;
uint16_t CLCD_EN_Pin=GPIO_Pin_5;

GPIO_TypeDef* CLCD_RS_Port=GPIOF;
uint16_t CLCD_RS_Pin=GPIO_Pin_0;
/************USER DEFINE**************/


void CLCD_FSMC_Init(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	  /* FSMC_Bank1_NORSRAM4 configuration */
	p.FSMC_AddressSetupTime = 0;
	p.FSMC_AddressHoldTime = 0;
	p.FSMC_DataSetupTime = 10;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	/* Color LCD configuration ------------------------------------
	     LCD configured as follow:
	        - Data/Address MUX = Disable
	        - Memory Type = SRAM
	        - Data Width = 8bit
	        - Write Operation = Enable
	        - Extended Mode = Enable
	        - Asynchronous Wait = Disable 
	*/
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

	/* BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void CLCD_Reg_Write(uint8_t data)
{
    CLCD_REG_WRITE=data;
	Delay_ms(10);
}

void CLCD_Conofig(void)
{
    uint8_t i;
    GPIO_InitTypeDef clcd;
    SysTick_Init();
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	
    /*data pin*/
    clcd.GPIO_Mode=GPIO_Mode_AF_PP;
    clcd.GPIO_Speed=GPIO_Speed_50MHz;
    for(i=0; i<CLCD_DATA_NUM; i++)
    {
        clcd.GPIO_Pin=CLCD_DATA_Pin[i];
        GPIO_Init(CLCD_DATA_Port[i], &clcd);
    }

    clcd.GPIO_Pin=CLCD_RW_Pin;
    GPIO_Init(CLCD_RW_Port, &clcd);// RW
//    GPIO_ResetBits(CLCD_RW_Port, CLCD_RW_Pin);

    clcd.GPIO_Pin=CLCD_EN_Pin;
    GPIO_Init(CLCD_EN_Port, &clcd);// EN
//    GPIO_ResetBits(CLCD_EN_Port, CLCD_EN_Pin);

    clcd.GPIO_Pin=CLCD_RS_Pin;
    GPIO_Init(CLCD_RS_Port, &clcd);// RS
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS


 	CLCD_FSMC_Init();
    
	Delay_ms(1);

	CLCD_Reg_Write(0x38);
	CLCD_Reg_Write(0x0c);
	CLCD_Reg_Write(0x06);
	
	CLCD_Reg_Write(0x80);
    
}

void CLCD_Print(uint8_t  column, uint8_t row, uint8_t data)
{
    uint8_t displayPos;

//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
    if(column==1)
    {
        displayPos=0x80;
    }
    else
    {
        displayPos=0xc0;
    }
    displayPos|=row-1;
    CLCD_Reg_Write(0x80);
		Delay_ms(10);
	//CLCD_Reg_Write(displayPos);

	CLCD_DISPLAY_WRITE=data;
	Delay_ms(10);
//    Delay_ms(1);
//    GPIO_SetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS
//    Delay_ms(1);
//    CLCD_Reg_Write(data);
//    Delay_ms(1);
//    GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
}

void CLCD_PrintString(uint8_t startCol, uint8_t startRow, uint8_t string[], uint8_t size)
{
    uint8_t i;
    for(i=0; i<size; i++)
    {
        CLCD_Print(startCol, startRow+i, string[i]);
    }
}

int CLCD_MKFont(uint8_t CLCD_storeAddress, uint8_t font[])
{
    uint8_t i, cgRamAddr;
    
    cgRamAddr=0x40+(8*CLCD_storeAddress);
    
    if(CLCD_storeAddress>8)
    {
        return CLCD_ERR_CGRAM_ADDR_OVF;// CGRAM Address OverFlow
    }

    for(i=0; i<8; i++)
    {
        CLCD_Reg_Write(cgRamAddr);
        
//        GPIO_SetBits(CLCD_RS_Port, CLCD_RS_Pin);// RS
//        Delay_ms(1);
        CLCD_Reg_Write(font[i]);
//        Delay_ms(1);
//        GPIO_ResetBits(CLCD_RS_Port, CLCD_RS_Pin);
        cgRamAddr++;
    }
    
    return 1;
}
