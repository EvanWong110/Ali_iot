#ifndef __BSP_RTC_H_
#define __BSP_RTC_H_

#include "stm32f4xx.h"


uint8_t My_RTC_Init(void);
uint32_t get_fattime(void);

#endif
