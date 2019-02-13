#include"My_I2C.h"


void I2C_Config(I2C_InitTypeDef * device, uint16_t I2C_Ack_state, uint16_t I2C_AcknowledgedAddress, uint32_t I2C_Speed, uint16_t I2C_DutyCycle, uint16_t I2C_Mode)
{
    device->I2C_Ack=I2C_Ack_state;
    device->I2C_AcknowledgedAddress=I2C_AcknowledgedAddress;
    device->I2C_DutyCycle=I2C_DutyCycle;
    device->I2C_ClockSpeed=I2C_Speed;
    device->I2C_Mode=I2C_Mode;
    device->I2C_OwnAddress1=0x00;
    I2C_Init(I2C1, device);
    I2C_Cmd(I2C1, ENABLE);
}

int I2C_Start(uint8_t deviceAddr, uint8_t I2C_direction)
{
    int timeOut;
    I2C_GenerateSTART(I2C1, ENABLE);// I2C START CLOCK
    timeOut=TIME_OUT_MAX;
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)==0)// Run time
    {
        timeOut--;
        if(timeOut<=0)
        {
            return I2C_START_ERROR;
//            while(1);
        }
    }
    
    /*Device address*/
    I2C_Send7bitAddress(I2C1, deviceAddr, I2C_direction);
    
    timeOut=TIME_OUT_MAX;
    if(I2C_direction==I2C_Direction_Transmitter)
    {
        while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0)
        {
            timeOut--;
            if(timeOut<=0)
            {
#ifdef DEBUG_MODE
                return I2C_ADDR_SEND_ERROR;
//                while(1);
#endif
            }
        }
    }
    
    else if(I2C_direction==I2C_Direction_Receiver)
    {
        while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0)
        {
            timeOut--;
            if(timeOut<=0)
            {
#ifdef DEBUG_MODE
                return I2C_ADDR_SEND_ERROR;
//                while(1);
#endif
            }
        }
    }
    
}

void I2C_Stop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

int I2C_WriteByte(uint8_t data)
{
     int timeOut;
    I2C_SendData(I2C1, data);


    timeOut=TIME_OUT_MAX;
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0)
   {
        timeOut--;
    	if(timeOut<=0)
        {
#ifdef DEBUG_MODE
            return I2C_BYTE_WRTIE_ERROR;
//            while(1);
#endif
        }
    }  
}

uint8_t I2C_ReadByte(void)
{
    uint8_t data;
    int timeOut;
    
    /*clear status register*/
    I2C1->SR1;
    I2C1->SR2;

    /*delay from read finish*/
    timeOut=TIME_OUT_MAX;
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==0 )
   {
        timeOut--;
        if(timeOut<=0)
        {
#ifdef DEBUG_MODE
            return I2C_BYTE_READ_ERROR;
//            while(1);
#endif
        }
    }

    /*DATA READ*/
    data=I2C_ReceiveData(I2C1);

    return data;
}
