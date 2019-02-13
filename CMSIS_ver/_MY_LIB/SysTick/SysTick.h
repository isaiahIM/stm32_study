#ifndef SYS_TICK__H
#define SYS_TICK__H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_ms(uint32_t time_ms);

#endif
