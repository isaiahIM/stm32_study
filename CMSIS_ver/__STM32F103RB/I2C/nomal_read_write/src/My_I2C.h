#ifndef MY_I2C__H
#define MY_I2C__H

#include "stm32f10x.h"
#include "debug_support.h"

#define DEVICE_ADDRESS 0Xa0
#define TIME_OUT_MAX 2000
								#define DEBUG_MODE

void I2C_Config(I2C_InitTypeDef * device, uint16_t I2C_Ack_state, uint16_t I2C_AcknowledgedAddress, uint32_t I2C_Speed, uint16_t I2C_DutyCycle, uint16_t I2C_Mode);
void I2C_Write(uint16_t wordAddr, char data);
char I2C_Read(uint16_t WordAddr);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(void);
void I2C_Start(uint8_t deviceAddr, uint8_t I2C_direction);
void I2C_Stop(void);

#endif

