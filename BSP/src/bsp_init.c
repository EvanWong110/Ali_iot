
#include "bsp_init.h"
#include "bsp_hash.h"
#include "bsp_sdio.h"

#include "bsp_rtc.h"

void Hard_Init(void)
{
	uint8_t state;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	UART1_Init(115200);
	My_RTC_Init();
	FSMC_SRAM_Init();
	BSP_hash_sha1_Init();
	while(SRAM_main())
	{
		printf("SRAM Test is false\n");
	}
	printf("SRAM test is ok\n");
	
	state = SD_Init();
	if(state != SD_OK)
	{
		printf("SDIO Init is false %d\n",state);
	}
	printf("SDIO Init is Ok\n");
}


void delay_us(uint16_t xus)
{
	for(uint16_t i=0 ; i<xus ; i++)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

void delay_ms(uint16_t xms)
{
	for(uint16_t i=0 ; i<xms ; i++)
	{
		delay_us(1000);
	}
}
