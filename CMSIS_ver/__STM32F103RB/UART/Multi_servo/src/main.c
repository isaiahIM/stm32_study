#include "stm32f10x.h"
#include<stdio.h>

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



/*SERVO Control define*/
#define SERVO_MIN_PULSE 70
#define SERVO_MAX_PULSE 220
#define SERVO_COUNT 2

#define OVER_THROTTLE 1
#define UNDER_THROTTLE 2
#define THROTTLE_OK 0

/*USART define*/
#define USART_READ_FINISH 1
#define USART_READ_YET 0


GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;
GPIO_InitTypeDef servo;

USART_InitTypeDef serial1;

TIM_TimeBaseInitTypeDef servo_base;
TIM_OCInitTypeDef servo_oc;

volatile int SYS_readData=0;// system value

const uint16_t servoPin[SERVO_COUNT]={GPIO_Pin_4, GPIO_Pin_5};
volatile uint16_t servoPos[SERVO_COUNT]={SERVO_MIN_PULSE, SERVO_MIN_PULSE};
volatile int servoSelect=0;


volatile int usart_readState;

/*Function Prototype*/    
int USART_ReadData(int *data);
int GetReadData(void);
void ResetReadData(void);

    
int ChkThrottleRange(int Throttle);
void Throttle(uint16_t *prev_pos, int cur_pos, int speed);
int main(void)
{
    int i, servo_writeSelect=0;
    
/*RCC Clock Init*/
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1EN, ENABLE);
    
/*NVIC Init*/
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_EnableIRQ(TIM1_CC_IRQn);

/*GPIO INIT*/
    uart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    uart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &uart_rx);

    uart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    uart_tx.GPIO_Pin=GPIO_Pin_9;
    uart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart_tx);

    for(i=0; i<SERVO_COUNT; i++)
    {
    servo.GPIO_Mode=GPIO_Mode_Out_PP;
    servo.GPIO_Pin=servoPin[i];
    servo.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &servo);
    }

    
/*USART Init*/
    serial1.USART_BaudRate=9600;
    serial1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    serial1.USART_Parity=USART_Parity_No;
    serial1.USART_StopBits=USART_StopBits_1;
    serial1.USART_WordLength=USART_WordLength_8b;
    serial1.USART_Mode=(USART_Mode_Rx | USART_Mode_Tx);
    USART_Init(USART1, &serial1);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);


/*TIMER1 Init*/
    servo_base.TIM_ClockDivision=0;
    servo_base.TIM_CounterMode=TIM_CounterMode_Up;
    servo_base.TIM_Prescaler=(SystemCoreClock/100000)-1;// 10us count
    servo_base.TIM_Period=(2000/SERVO_COUNT)-1;// 20ms Cycle, multi servo control
    TIM_TimeBaseInit(TIM1, &servo_base);

    servo_oc.TIM_OCMode=TIM_OCMode_PWM1;
    servo_oc.TIM_OutputState=TIM_OutputState_Enable;
    servo_oc.TIM_Pulse=SERVO_MIN_PULSE;
    servo_oc.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &servo_oc);

    TIM_Cmd(TIM1, ENABLE);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
/****************base init(up)*****************/
    
    GPIO_SetBits(GPIOA, servoPin[0]);
    
    while(1)
    {

        if(usart_readState==USART_READ_FINISH)
        {

            Throttle(&servoPos[servo_writeSelect], SYS_readData, 2);
            ResetReadData();
            servo_writeSelect++;
            servo_writeSelect%=SERVO_COUNT;
            usart_readState=USART_READ_YET;
        }
    }
    
}


/******************User Define Function:******************/

    /*USART Function*/
int USART_ReadData(int *data)
{
    char buffer=USART_ReceiveData(USART1);

    if(buffer=='f')
    {
      return USART_READ_FINISH;
    }
    
    (*data)*=10;
    (*data)+=(buffer-48);
    return USART_READ_YET;
}

int GetReadData(void)
{
    return SYS_readData;
}

void ResetReadData(void)
{
    SYS_readData=0;
}

    /*SERVO Control Function*/
int ChkThrottleRange(int Throttle)
{
    if(Throttle > SERVO_MAX_PULSE)
    {
        return OVER_THROTTLE;
    }
    else if(Throttle < SERVO_MIN_PULSE)
    {
        return UNDER_THROTTLE;
    }

    return THROTTLE_OK;
}

void Throttle(uint16_t *prev_pos, int cur_pos, int speed)
{
    int dir=0, throttle_state;

/*Check Throttle dir*/
    if( (*prev_pos) < cur_pos)
    {
        dir=1*speed;
    }
    
    else if( (*prev_pos) > cur_pos)
    {
        dir=-1*speed;
    }

/*Throttle move*/
    while( (*prev_pos) >= SERVO_MIN_PULSE && (*prev_pos) <= SERVO_MAX_PULSE && (*prev_pos)!=cur_pos)
    {
        (*prev_pos)+=dir;
        printf("Throttle: %d\n", (*prev_pos) );
    }

/*check over Thorottle*/
    throttle_state = ChkThrottleRange(*prev_pos);

    if(throttle_state == OVER_THROTTLE)// throttle over the range
    {
        (*prev_pos) = SERVO_MAX_PULSE;
    }
    else if(throttle_state == UNDER_THROTTLE)// throttle under the range
    {
        (*prev_pos) = SERVO_MIN_PULSE;
    }
}




/**********************IRQ HANDLER LINE*****************************/

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        servoSelect++;
        servoSelect%=SERVO_COUNT;
        
        GPIO_SetBits(GPIOA, servoPin[servoSelect] );
        TIM_SetCompare1(TIM1, servoPos[(servoSelect+1)%SERVO_COUNT] );
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

void TIM1_CC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
    {
        GPIO_ResetBits(GPIOA,  servoPin[servoSelect] );
    }
     
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}


void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) )
    {
        usart_readState=USART_ReadData(&SYS_readData);

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}




