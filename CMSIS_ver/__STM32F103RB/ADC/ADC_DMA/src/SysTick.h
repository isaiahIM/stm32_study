#ifndef SYS_TICK__H
#define SYS_TICK__H

#include "stm32f10x.h"

void Init_SysTick(void);
void Delay(uint32_t time_ms);

#endif
