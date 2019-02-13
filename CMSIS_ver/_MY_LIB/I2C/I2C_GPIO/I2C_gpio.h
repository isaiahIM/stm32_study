#ifndef I2C_GPIO__H
#define I2C_GPIO__H

#include "stm32f10x.h"

void I2C_Config(void);
void I2C_Start(uint8_t deviceAddr, uint8_t I2C_Direction);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(void);
void I2C_SendACK(FunctionalState ackState);
void I2C_GetAck(void);
void I2C_Stop(void);

#endif
