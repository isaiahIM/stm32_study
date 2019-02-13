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
void I2C_Write(uint16_t wordAddr, char data)
{
    I2C_WriteByte((uint8_t)((wordAddr&0XFF00)>>8) );// word address(high)
    I2C_WriteByte((uint8_t)(wordAddr&0X00FF) );// word address(low)
    
    I2C_WriteByte(data);// data write
}

char I2C_Read(uint16_t WordAddr)
{
    char readData=0x01;
    
    /*START*/
    I2C_Start(DEVICE_ADDRESS, I2C_Direction_Transmitter);
    
    /*WORD ADDRESS(2BYTE)*/
    I2C_WriteByte( (uint8_t)( (WordAddr&0xFF00)>>8) );
    I2C_WriteByte((uint8_t)(WordAddr&0X00FF) );
    
    /*RE-START*/
    I2C_Start(DEVICE_ADDRESS, I2C_Direction_Receiver);


    /*DATA READ*/
    readData=I2C_ReadByte();
    
    /*NO ACK*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);// NO SEND ACK(Write)

    /*STOP BIT*/
    I2C_GenerateSTOP(I2C1, ENABLE);// STOP BIT
    
    return readData;
}

void I2C_Start(uint8_t deviceAddr, uint8_t I2C_direction)
{
    int timeOut;
    I2C_GenerateSTART(I2C1, ENABLE);// I2C START CLOCK
    timeOut=TIME_OUT_MAX;
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)==0)// Run time
    {
        timeOut--;
        if(timeOut<=0)
        {
            printf("start error");
            while(1);
        }
    }
    
    /*word address*/
    I2C_Send7bitAddress( I2C1, deviceAddr, I2C_direction);
    
    timeOut=TIME_OUT_MAX;
    if(I2C_direction==I2C_Direction_Transmitter)
    {
        while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0)
        {
            timeOut--;
            if(timeOut<=0)
            {
#ifdef DEBUG_MODE
                printf("slave addr write fail-transmit");
                while(1);
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
                printf("slave addr read fail-receive");
                while(1);
#endif
            }
        }
    }
    
}

void I2C_Stop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_WriteByte(uint8_t data)
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
            printf("master write fail");
            while(1);
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
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

    /*DATA READ*/
    data=I2C_ReceiveData(I2C1);

    timeOut=TIME_OUT_MAX;
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==0 )
   {
        timeOut--;
        if(timeOut<=0)
        {
#ifdef DEBUG_MODE
            printf("MASTER read fail");
            while(1);
#endif
        }
    }

    return data;
}
