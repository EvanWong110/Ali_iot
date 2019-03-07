#ifndef __BSP_EEPROM_H_
#define __BSP_EEPROM_H_

#ifdef  __cplusplus
	extern "C" {
#endif

	#include "stm32f4xx.h"

	#include "FreeRTOS.h"
	#include "task.h"

	#include <stdbool.h>

	#define EEPROM_ADDRESS 			0xA0
	#define EEPROM_PAGE_SIZE		8
	#define EEPROM_PAGE_NUMBER	32
	#define EEPROM_SIZE					256
	

	void EEPROM_Init(void);
	bool EEPROM_Write_Page(uint8_t page, uint8_t offset, uint8_t * data, uint8_t len);
	bool EEPROM_Read_Page(uint8_t page, uint8_t offset, uint8_t * data, uint8_t len);
	bool EEPROM_Write(uint16_t address, uint8_t * data, uint16_t len);	
	bool EEPROM_Read(uint16_t address, uint8_t * data, uint16_t len);

	void EEPROM_Test(void);
	void EEPROM_Test1(void);

#ifdef  __cplusplus
	}
#endif

#endif
