#ifndef __BSP_INIT_H_
#define __BSP_INIT_H_

#include "stm32f4xx.h"

#include "stm32f4xx.h"

#include "bsp_sram.h"
#include "bsp_uart.h"
#include <stdio.h>


void delay_us(uint16_t xus);
void delay_ms(uint16_t xms);

void Hard_Init(void);

#endif
