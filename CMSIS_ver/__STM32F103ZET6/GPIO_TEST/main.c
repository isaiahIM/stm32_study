#include "usart.h"
#include "SysTick.h"
#include "stm32f10x.h"

GPIO_InitTypeDef uart_rx;
GPIO_InitTypeDef uart_tx;

GPIO_InitTypeDef led;

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    uart_rx.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    uart_rx.GPIO_Pin=GPIO_Pin_10;
    GPIO_Init(GPIOA, &uart_rx);

    uart_tx.GPIO_Mode=GPIO_Mode_AF_PP;
    uart_tx.GPIO_Pin=GPIO_Pin_9;
    uart_tx.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart_tx);

    led.GPIO_Mode=GPIO_Mode_Out_PP;
    led.GPIO_Pin=GPIO_Pin_0;
    led.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &led);
    
    USART_BaseConfig(USART1, 9600, USART_WordLength_8b, (USART_Mode_Rx|USART_Mode_Tx), USART_Parity_No, USART_StopBits_1);
    Init_SysTick();

    while(1)
    {
        GPIO_ResetBits(GPIOB, led.GPIO_Pin);
        Delay(500);
        GPIO_SetBits(GPIOB, led.GPIO_Pin);
        Delay(500);
    }
}


