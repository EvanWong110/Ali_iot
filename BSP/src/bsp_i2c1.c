
#include "bsp_i2c1.h"

#include "bsp_delay.h"

void I2C1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(I2C1_RCCx , ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = I2C1_SCL_Pin | I2C1_SDA_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(I2C1_GPIOx , &GPIO_InitStruct);
	I2C1_Stop();
}

// SDA SCLΪ�ߵ�ƽ��ʱ���ǿ���״̬
// SCLΪ�ߵ�ƽ �� SDA�ɸߵ�ƽת��Ϊ�͵�ƽ��ʱ�� ��Ϊ�����ź�
bool I2C1_Start(void)
{
	I2C1_SDA_H;
	I2C1_SCL_H;
	delay_us(1);
	if(!I2C1_SDA_READ)	//��ȡSDA��״̬�Ƿ�Ϊ�͵�ƽ
		return false;
	I2C1_SDA_L;
	delay_us(1);	//�ȴ���ƽ�ȶ�
	if(I2C1_SDA_READ)	//��ȡSDA��״̬�Ƿ�Ϊ�ߵ�ƽ
		return false;
	I2C1_SCL_L;
	delay_us(1);
	return true;
}


// SCLΪ�ߵ�ƽ,SDAΪ�͵�ƽ
// SDA�ɵ͵�ƽ--->�ߵ�ƽ��ʱ�� ,Ϊֹͣ�ź�
bool I2C1_Stop(void)
{
	I2C1_SCL_H;
//	delay_us(1);
	I2C1_SDA_L;
	delay_us(1);
	if(I2C1_SDA_READ)
		return false;
	I2C1_SDA_H;
	delay_us(1);
	if(!I2C1_SDA_READ)
		return false;
	return true;
}

// SCL SDAΪ�͵�ƽ ; SCLΪ�͵�ƽ--->�ߵ�ƽ�󣬴ӻ���ȡSDA�ϵĵ�ƽ״̬(�͵�ƽ)
void I2C1_Ack(void)
{
	I2C1_SCL_L;
	delay_us(1);
	I2C1_SDA_L;
	delay_us(1);
	I2C1_SCL_H;
	delay_us(1);
	I2C1_SCL_L;
	I2C1_SDA_H;
}
// SCLΪ�͵�ƽ  SDAΪ�ߵ�ƽ   ;  SCL�ɵ͵�ƽ--->�ߵ�ƽ, �ӻ���ȡSDA�ϵĵ�ƽ״̬(�ߵ�ƽ)
void I2C1_NoAck(void)
{
	I2C1_SCL_L;
	delay_us(1);
	I2C1_SDA_H;
	delay_us(1);
	I2C1_SCL_H;
	delay_us(1);
	I2C1_SCL_L;
	delay_us(1);
}

// SCLΪ�͵�ƽ,SDAΪ�ߵ�ƽ; SCL�ɵ͵�ƽ-->�ߵ�ƽ��;�ӻ���������SDA���ϵĵ�ƽ,����ֻҪ�ж�SDA��Ϊ�͵�ƽ�ʹ���ӻ����յ�һ�ֽ�
bool I2C1_WaitAck(void)
{
	I2C1_SCL_L;
	I2C1_SDA_H;
	delay_us(1);
	if(!I2C1_SDA_READ)	//�ж�SDA���ϵĵ�ƽ״̬�Ƿ�Ϊ�͵�ƽ
		return false;
	I2C1_SCL_H;
	delay_us(1);
	if(I2C1_SDA_READ)	//�ж�SDA���ϵĵ�ƽ״̬Ϊ�ߵ�ƽ�Ļ�
	{
		I2C1_SCL_L;
		return false;
	}
	I2C1_SCL_L;
	return true;
}

void I2C1_SendByte(uint8_t SendByte)
{
	for(uint8_t i=0 ; i<8 ; i++)
	{
		I2C1_SCL_L;
		delay_us(1);
		if(SendByte & 0x80)
			I2C1_SDA_H;
		else
			I2C1_SDA_L;
		delay_us(1);
		I2C1_SCL_H;
		delay_us(1);
		SendByte <<= 1;
	}
}

//SCL�ɵ͵�ƽ--->�ߵ�ƽ֮��,�ӻ�������ݷ��͵����ݽ��ı�SDA�ߵĵ�ƽ״̬;����������ȡSDA���ϵĵ�ƽ״̬(8�� === 1�ֽ�)
uint8_t I2C1_ReadByte(void)
{
	uint8_t recieve = 0;
	for(uint8_t i=0 ; i<8 ; i++)
	{
		recieve <<= 1;
		I2C1_SCL_L;
		delay_us(1);
		I2C1_SCL_H;
		delay_us(1);
		if(I2C1_SDA_READ)
			recieve |= 0x01;
	}
	return recieve;
}

bool I2C1_Write_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}
	I2C1_SendByte(REG_Address);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}
	I2C1_SendByte(REG_data);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}
	I2C1_Stop();
	return true;	
}

uint8_t I2C1_Read_REG(uint8_t SlaveAddress, uint8_t REG_Address)
{
	uint8_t recieve=0;
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}
	I2C1_SendByte(REG_Address);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}	
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress + 1);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}	
	recieve = I2C1_ReadByte();
	I2C1_NoAck();
	I2C1_Stop();
	return recieve;
}

bool I2C1_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t * buff , uint8_t len)
{
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}	
	I2C1_SendByte(REG_Address);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}	
	for(uint8_t i=0 ; i<len-1 ; i++)
	{
		I2C1_SendByte(buff[i]);
		if(!I2C1_WaitAck())
		{
			I2C1_Stop();
			return false;
		}	
	}
	I2C1_SendByte(buff[len-1]);
//	I2C1_NoAck();	??
	I2C1_Stop();
	return true;
}

bool I2C1_Read_NByte(uint8_t SlaveAddress, uint8_t REG_Address , uint8_t * buff , uint8_t len)
{
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress);	//����7λ��ַ + 1λдλ
	if(!I2C1_WaitAck())							//�ȴ��ӻ����͵�ACK
	{
		I2C1_Stop();
		return false;
	}	
	I2C1_SendByte(REG_Address);		//���ͼĴ�����ַ
	if(!I2C1_WaitAck())							//�ȴ��ӻ����͵�ACK
	{
		I2C1_Stop();
		return false;
	}		
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress | 0x01);	//����7λ��ַ + 1λ��λ
	if(!I2C1_WaitAck())							//�ȴ��ӻ����͵�ACK
	{
		I2C1_Stop();
		return false;
	}	
	for(uint8_t i=0 ; i<len-1 ; i++)
	{
		*(buff+i) = I2C1_ReadByte();
		I2C1_Ack();
	}
	*(buff+len-1) = I2C1_ReadByte();
	I2C1_NoAck();
	if(!I2C1_Stop())
		return false;
	return true;;	
}

bool I2C1_CheckDevice(uint8_t SlaveAddress)
{
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress);
	if(!I2C1_WaitAck())
	{
		I2C1_Stop();
		return false;
	}	
	I2C1_Stop();
	return true;
}

