
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

// SDA SCL为高电平的时候是空闲状态
// SCL为高电平 ， SDA由高电平转换为低电平的时候 ，为启动信号
bool I2C1_Start(void)
{
	I2C1_SDA_H;
	I2C1_SCL_H;
	delay_us(1);
	if(!I2C1_SDA_READ)	//读取SDA的状态是否为低电平
		return false;
	I2C1_SDA_L;
	delay_us(1);	//等待电平稳定
	if(I2C1_SDA_READ)	//读取SDA的状态是否为高电平
		return false;
	I2C1_SCL_L;
	delay_us(1);
	return true;
}


// SCL为高电平,SDA为低电平
// SDA由低电平--->高电平的时候 ,为停止信号
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

// SCL SDA为低电平 ; SCL为低电平--->高电平后，从机读取SDA上的电平状态(低电平)
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
// SCL为低电平  SDA为高电平   ;  SCL由低电平--->高电平, 从机读取SDA上的电平状态(高电平)
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

// SCL为低电平,SDA为高电平; SCL由低电平-->高电平后;从机将会拉低SDA线上的电平,主机只要判断SDA线为低电平就代表从机接收到一字节
bool I2C1_WaitAck(void)
{
	I2C1_SCL_L;
	I2C1_SDA_H;
	delay_us(1);
	if(!I2C1_SDA_READ)	//判断SDA线上的电平状态是否为低电平
		return false;
	I2C1_SCL_H;
	delay_us(1);
	if(I2C1_SDA_READ)	//判断SDA线上的电平状态为高电平的话
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

//SCL由低电平--->高电平之后,从机将会根据发送的数据将改变SDA线的电平状态;主机辅助读取SDA线上的电平状态(8次 === 1字节)
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
	I2C1_SendByte(SlaveAddress);	//发送7位地址 + 1位写位
	if(!I2C1_WaitAck())							//等待从机发送的ACK
	{
		I2C1_Stop();
		return false;
	}	
	I2C1_SendByte(REG_Address);		//发送寄存器地址
	if(!I2C1_WaitAck())							//等待从机发送的ACK
	{
		I2C1_Stop();
		return false;
	}		
	if(!I2C1_Start())
		return false;
	I2C1_SendByte(SlaveAddress | 0x01);	//发送7位地址 + 1位读位
	if(!I2C1_WaitAck())							//等待从机发送的ACK
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

