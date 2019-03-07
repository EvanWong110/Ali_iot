
#include "bsp_eeprom.h"
#include "bsp_gpio.h"
#include "bsp_i2c2.h"

void EEPROM_Init(void)
{
	uint8_t write[8] = {0,0,0,0,0,0,0,0};
	GPIO_Initialize(GPIOE , GPIO_Pin_10 |GPIO_Pin_12 |GPIO_Pin_15 , GPIO_Mode_OUT , GPIO_OType_PP , GPIO_PuPd_NOPULL);	//初始化A0 A1 A2端口 默认输出低电平
	for(uint8_t i=0 ; i<32 ; i++)
	{
		EEPROM_Write_Page(i , 0 , write , 8);
	}
}

//AT24C02  0~255个地址 每个地址存储1个字节   [256个字节分成32页 每页8字节]
bool EEPROM_Write_Page(uint8_t page, uint8_t offset, uint8_t * data, uint8_t len)	//写一页数据
{
	uint8_t address = page *EEPROM_PAGE_SIZE + offset;
	if(!I2C2_Start())
		return false;
	I2C2_SendByte(EEPROM_ADDRESS);
	if(!I2C2_WaitAck())
	{
		I2C2_Stop();
		return false;
	}
	I2C2_SendByte(address);
	if(!I2C2_WaitAck())
	{
		I2C2_Stop();
		return false;
	}
	for(uint8_t i=0 ; i<len-1 ; i++)	//len =5
	{
		I2C2_SendByte(data[i]);
		if(!I2C2_WaitAck())
		{
			I2C2_Stop();
			return false;
		}		
	}
	I2C2_SendByte(data[len-1]);
	I2C2_NoAck();
	I2C2_Stop();
	vTaskDelay(5/portTICK_PERIOD_MS);
	return true;
}

bool EEPROM_Write(uint16_t address, uint8_t * data, uint16_t len)
{
	bool result = false;
	uint8_t page = address / EEPROM_PAGE_SIZE;		//计算当前在哪页
	uint8_t offset = address % EEPROM_PAGE_SIZE;	//计算当前的偏移地址
	uint8_t length = EEPROM_PAGE_SIZE - offset;		//当前页数剩下多少字节可以写
	
	if((address + len) >= EEPROM_SIZE)
	{
		return false;
	}
	
	if(length >= len)	//不需要跨页写
	{
		result = EEPROM_Write_Page(page , offset , data , len);
		if(result == false)
			return false;
	}
	else		//需要跨页写
	{
		result = EEPROM_Write_Page(page , offset , data , length);
		if(result == false)
			return false;
		data +=length;
		page = page +1;
		uint8_t page_count = (len - length) / EEPROM_PAGE_SIZE + page;
		for(; page < page_count ; page++ , data +=EEPROM_PAGE_SIZE)
		{
			result = EEPROM_Write_Page(page , 0 , data , EEPROM_PAGE_SIZE);
			if(result == false)
				return false;
		}
		uint8_t end_len = (len - length) - ((len - length) / EEPROM_PAGE_SIZE * EEPROM_PAGE_SIZE);
		if(end_len >0)
		{
			result = EEPROM_Write_Page(page , 0 , data , end_len);
			if(result == false)
				return false;
		}
	}
	return true;
}


bool EEPROM_Read_Page(uint8_t page, uint8_t offset, uint8_t * data, uint8_t len)
{
	uint8_t address = page * EEPROM_PAGE_SIZE + offset;
	if(!I2C2_Start())
		return false;
	I2C2_SendByte(EEPROM_ADDRESS);
	if(!I2C2_WaitAck())
	{
		I2C2_Stop();
		return false;
	}		
	I2C2_SendByte(address);
	if(!I2C2_WaitAck())
	{
		I2C2_Stop();
		return false;
	}	
	if(!I2C2_Start())
		return false;
	I2C2_SendByte(EEPROM_ADDRESS | 0x01);
	if(!I2C2_WaitAck())
	{
		I2C2_Stop();
		return false;
	}	
	for(uint8_t i=0 ; i<len-1 ; i++)
	{
		data[i] = I2C2_ReadByte();
		I2C2_Ack();
	}
	data[len-1] = I2C2_ReadByte();
	I2C2_NoAck();
	I2C2_Stop();
	return true;
}

bool EEPROM_Read(uint16_t address, uint8_t * data, uint16_t len)
{
	bool result = false;
	uint8_t page = address / EEPROM_PAGE_SIZE;
	uint8_t offset = address % EEPROM_PAGE_SIZE;
	uint8_t length = EEPROM_PAGE_SIZE - offset;
	
	if((address + len) >= EEPROM_SIZE)
	{
		return false;
	}
	
	if(length >= len)	//不需要跨页读
	{
		result = EEPROM_Read_Page(page , offset , data , len);
		if(result ==false)
			return false;
	}
	else	//跨页读
	{
		result = EEPROM_Read_Page(page , offset , data , length);
		if(result ==false)
			return false;
		data +=length;
		page = page +1;
		uint8_t page_count = (len - length) / EEPROM_PAGE_SIZE + page;
		for(; page <page_count ; page++ , data +=EEPROM_PAGE_SIZE)	//读取整页数据
		{
			result = EEPROM_Read_Page(page , 0 , data , EEPROM_PAGE_SIZE);
			if(result ==false)
				return false;
		}
		uint8_t end_len = (len-length) - (len - length)/EEPROM_PAGE_SIZE * EEPROM_PAGE_SIZE;
		if(end_len > 0)
		{
			result = EEPROM_Read_Page(page , 0 , data , end_len);
			if(result ==false)
				return false;
		}
	}
	return true;
}

uint8_t write_data[40] = {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
uint8_t read[32][8];
void EEPROM_Test(void)
{
	EEPROM_Write(3 , write_data , 37);
	for(uint8_t i=0 ; i<32 ; i++)
	{
		EEPROM_Read_Page(i , 0 , read[i] , 8);
	}
}

uint8_t read_data[255] = {0};

void EEPROM_Test1(void)
{
	uint8_t write[32][8];
	for(uint8_t i=0 ; i<32 ; i++)
	{
		for(uint8_t j=0 ; j<8 ; j++)
		{
			write[i][j] = i*8+j;
		}
	}
	for(uint8_t i=0 ; i<32 ; i++)
	{
		EEPROM_Write_Page(i , 0 , write[i] , 8);
	}	
	EEPROM_Read(8 , read_data , 59);
}
