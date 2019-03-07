
#ifndef __BSP_SRAM_H_
#define __BSP_SRAM_H_

#include "stm32f4xx.h"


#define EXT_SRAM_ADDR  	((uint32_t)0x68000000)
#define EXT_SRAM_SIZE	(1024 * 1024)

void FSMC_SRAM_Init(void);
int SRAM_main(void);

#endif
