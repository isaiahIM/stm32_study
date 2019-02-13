#include "RTC.h"
#include "stm32f10x.h"

void Init_RTC(void)
{   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    PWR_DeInit();

    PWR_BackupAccessCmd(ENABLE);

    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}
  
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);

    RTC_StableDelay();

    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask();
    
    RTC_SetPrescaler(0X7FFF);// 32.768 KHz
    RTC_WaitForLastTask();
}

void RTC_StableDelay(void)
{
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
}
