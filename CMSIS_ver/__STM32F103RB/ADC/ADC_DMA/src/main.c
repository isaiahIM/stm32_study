#include "stm32f10x.h"
#include "usart.h"
#include "debug_support.h"
#include "SysTick.h"

ADC_InitTypeDef adc;
DMA_InitTypeDef adc_dma;
GPIO_InitTypeDef adc_gpio;

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;

volatile uint32_t adc_readData;
uint8_t adc_ch=1;

void ADC_Config(uint32_t *adc_readData);

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    adc_gpio.GPIO_Mode=GPIO_Mode_AIN;
    adc_gpio.GPIO_Pin=GPIO_Pin_1;
    GPIO_Init(GPIOA, &adc_gpio);
    
/*USART GPIO CONFIG*/
    uart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    uart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &uart_rx);

    uart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    uart_tx.GPIO_Pin=GPIO_Pin_9;
    uart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart_tx);

    
/*ADC-DMA CONFIG*/
    DMA_DeInit(DMA1_Channel1);
    adc_dma.DMA_M2M=DMA_M2M_Disable;
    adc_dma.DMA_DIR=DMA_DIR_PeripheralSRC;
    adc_dma.DMA_MemoryBaseAddr=(uint32_t)&adc_readData;
    adc_dma.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;// 16bit
    adc_dma.DMA_MemoryInc=DMA_MemoryInc_Disable;// memory increment disable
    adc_dma.DMA_Mode=DMA_Mode_Circular;// DMA LOOP
    adc_dma.DMA_PeripheralBaseAddr=(uint32_t)&(ADC1->DR);// adc data register Refer
    adc_dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
    adc_dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    adc_dma.DMA_Priority=DMA_Priority_High;// high Priority
    adc_dma.DMA_BufferSize=1;
    DMA_Init(DMA1_Channel1, &adc_dma);// DMA1 CH1 is include ADC1
    DMA_Cmd(DMA1_Channel1, ENABLE);
    

/*ADC CONFIG*/
    adc.ADC_Mode=ADC_Mode_Independent;
    adc.ADC_DataAlign=ADC_DataAlign_Right;
    adc.ADC_ScanConvMode=ENABLE;// SCAN ENABLE
    adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
    adc.ADC_NbrOfChannel=adc_ch;
    adc.ADC_ContinuousConvMode=ENABLE;
    ADC_Init(ADC1, &adc);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    /*ADC CALIBRATION*/
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
    USART_BaseConfig(USART1, 9600, USART_WordLength_8b, (USART_Mode_Tx|USART_Mode_Rx), USART_Parity_No, USART_StopBits_1);
    Init_SysTick();
		
   while(1)
   {
        printf("ADC: %d\n", adc_readData);
        Delay(100);
   }
}




