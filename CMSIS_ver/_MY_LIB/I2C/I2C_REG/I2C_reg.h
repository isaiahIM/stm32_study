#ifndef MY_I2C__H
#define MY_I2C__H

#include "stm32f10x.h"
#include "debug_support.h"

#define I2C_TIME_OUT_MAX 2000
#define I2C_START_ERROR 1
#define I2C_ADDR_SEND_ERROR 2
#define I2C_BYTE_WRTIE_ERROR 3
#define I2C_BYTE_READ_ERRR 4



#define DEVICE_ADDRESS 0Xa0


								#define DEBUG_MODE

void I2C_Config(I2C_InitTypeDef * device, uint16_t I2C_Ack_state, uint16_t I2C_AcknowledgedAddress, uint32_t I2C_Speed, uint16_t I2C_DutyCycle, uint16_t I2C_Mode);
int I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(void);
int I2C_Start(uint8_t deviceAddr, uint8_t I2C_direction);
void I2C_Stop(void);

#endif

