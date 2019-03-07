
#ifndef __BSP_I2C1_H_
#define __BSP_I2C1_H_


#include "stm32f4xx.h"
#include <stdbool.h>


#define I2C1_RCCx				RCC_AHB1Periph_GPIOB
#define I2C1_GPIOx			GPIOB

#define I2C1_SCL_Pin		GPIO_Pin_8
#define I2C1_SDA_Pin		GPIO_Pin_9

#define I2C1_SCL_H			(I2C1_GPIOx->BSRRL = I2C1_SCL_Pin)
#define I2C1_SCL_L			(I2C1_GPIOx->BSRRH = I2C1_SCL_Pin)
												                                
#define I2C1_SDA_H			(I2C1_GPIOx->BSRRL = I2C1_SDA_Pin)
#define I2C1_SDA_L			(I2C1_GPIOx->BSRRH = I2C1_SDA_Pin)

#define I2C1_SDA_READ		I2C1_GPIOx->IDR & I2C1_SDA_Pin

void I2C1_GPIO_Config(void);
bool I2C1_Start(void);
bool I2C1_Stop(void);
void I2C1_Ack(void);
void I2C1_NoAck(void);
bool I2C1_WaitAck(void);
void I2C1_SendByte(uint8_t SendByte);
uint8_t I2C1_ReadByte(void);

bool I2C1_Write_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
uint8_t I2C1_Read_REG(uint8_t SlaveAddress, uint8_t REG_Address);

bool I2C1_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t * buff , uint8_t len);
bool I2C1_Read_NByte(uint8_t SlaveAddress, uint8_t REG_Address , uint8_t * buff , uint8_t len);

bool I2C1_CheckDevice(uint8_t SlaveAddress);

#endif
