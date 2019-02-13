#include "stm32f10x.h"
#include "RTC.h"

#define RTC_BACKUP_DATA 0XAAAA
#define RTC_ALAM_VAL 100

#define FND_DIGIT_10 1
#define FND_DIGIT_1 0


void FND_High(uint16_t pin);
void FND_Low(uint16_t pin);

void FND_Display(int num, uint8_t digit);

GPIO_InitTypeDef fnd;
GPIO_InitTypeDef test_sig;
TIM_TimeBaseInitTypeDef timer;

volatile uint32_t SYS_RTC_count=0;
volatile uint8_t digitState=FND_DIGIT_1;

const uint16_t fnd_digit[2]={GPIO_Pin_10, GPIO_Pin_9};
const uint16_t fnd_pinMap[8]={GPIO_Pin_6, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_3,
                                              GPIO_Pin_2, GPIO_Pin_7, GPIO_Pin_4, GPIO_Pin_5};// a~dp
const uint8_t fnd_font[10]=
{
    0xfc,// 0
    0x60,// 1
    0xda,// 2
    0xf2,// 3
    0x66,// 4
    0xb6,// 5
    0xbe,// 6
    0xe4,// 7
    0xfe,// 8
    0xf6// 9
    };

int main()
{
    int i;

/*APB CLOCK INIT*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
/*FND GPIO INIT*/
    fnd.GPIO_Mode=GPIO_Mode_Out_PP;
    fnd.GPIO_Speed=GPIO_Speed_50MHz;
    for(i=0; i<8; i++)
    {
        fnd.GPIO_Pin=fnd_pinMap[i];
        GPIO_Init(GPIOA, &fnd);
    }
    for(i=0; i<2; i++)
    {
        fnd.GPIO_Pin=fnd_digit[i];
        GPIO_Init(GPIOA, &fnd);
    }
    test_sig.GPIO_Mode=GPIO_Mode_Out_PP;
    test_sig.GPIO_Speed=GPIO_Speed_50MHz;
    test_sig.GPIO_Pin=GPIO_Pin_11;
    GPIO_Init(GPIOA, &test_sig);

/*RTC INIT*/
    if(BKP_ReadBackupRegister(BKP_DR1)!=RTC_BACKUP_DATA)
    {
        Init_RTC();
        RTC_StableDelay();

        /*alam Intettupt set*/
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        RTC_StableDelay();
        
        /*alam value set*/
        RTC_SetAlarm(RTC_ALAM_VAL);
        RTC_StableDelay();
        
        BKP_WriteBackupRegister(BKP_DR1, RTC_BACKUP_DATA);
    }
    else
    {
        RTC_StableDelay();
        
        /*sec(time) Interrupt set*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();

        /*alam Interrupt set*/
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        RTC_WaitForLastTask();

        /*alam value set*/
        RTC_SetAlarm(RTC_ALAM_VAL);
    }
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
    
/*TIMER INIT*/
    timer.TIM_ClockDivision=0;
    timer.TIM_CounterMode=TIM_CounterMode_Up;
    timer.TIM_Prescaler=(SystemCoreClock/10000)-1;// 100us count
    timer.TIM_Period=100;// 10us interrupt
    TIM_TimeBaseInit(TIM1, &timer);
    TIM_Cmd(TIM1, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    /*NVIC INIT*/
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
/*******************BASE INIT***********************/

    while(1)
    {
        FND_Display(SYS_RTC_count, digitState);
    }
}


/**********INTERRUPT IRQ HANDLER***********/
void RTC_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_SEC) == SET )// 1sec Interrupt
    {
        SYS_RTC_count+=1;
        RTC_SetCounter(SYS_RTC_count);
        RTC_WaitForLastTask();
        RTC_ClearITPendingBit(RTC_IT_SEC);

        if(SYS_RTC_count%2)
        {
            GPIO_SetBits(GPIOA, test_sig.GPIO_Pin);
        }
        else
        {
            GPIO_ResetBits(GPIOA, test_sig.GPIO_Pin);
        }
    }
    
    else if(RTC_GetITStatus(RTC_IT_ALR) == SET)// 100sec Interrupt
    {
        SYS_RTC_count=0;
        RTC_SetCounter(SYS_RTC_count);
        RTC_WaitForLastTask();
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        if(digitState==FND_DIGIT_1)
        {
            digitState=FND_DIGIT_10;
        }
        else
        {
            digitState=FND_DIGIT_1;
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

/*********USER DEFINE FUNCTION************/

void FND_High(uint16_t pin)
{
    GPIO_ResetBits(GPIOA, pin);
}

void FND_Low(uint16_t pin)
{
    GPIO_SetBits(GPIOA, pin);
}

void FND_Display(int num, uint8_t digit)
{
    uint8_t displayNum, i, fnd_mask=0x80;

    if(digit==FND_DIGIT_10)
    {
        displayNum=( (num%100)-(num%10) )/10 ;
    }
    else if(digit==FND_DIGIT_1)
    {
        displayNum=(num%10);
    }

    for(i=0; i<8; i++)
    {
        if( (fnd_font[displayNum]&fnd_mask)!=0)
        {
            FND_High(fnd_pinMap[i]);
        }
        else
        {
            FND_Low(fnd_pinMap[i]);
        }
        fnd_mask>>=1;
    }

    FND_Low(fnd_digit[digit]);// fnd high
    FND_High(fnd_digit[ (digit+1)%2] );
}



