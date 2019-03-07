#include "bsp_rtc.h"

#include "bsp_init.h"
#include <time.h>

uint8_t My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	uint16_t retry=0x1FFF;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);	//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);		//解锁后备寄存器访问保护
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5A50)	//判定是否是第一次配置
	{
		RCC_LSEConfig(RCC_LSE_ON);	//开启LSE时钟
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)		//等待LSE晶振时钟准备就绪
		{
			retry++;
			delay_ms(10);
		}
		if(retry ==0)
		{
			return 1;
		}
		else
		{
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//RTCCLK的时钟源为LSE
			RCC_RTCCLKCmd(ENABLE);			//使能RTC时钟

			RTC_InitStruct.RTC_AsynchPrediv = 0x7F;
			RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
			RTC_InitStruct.RTC_SynchPrediv = 0xFF;
			RTC_Init(&RTC_InitStruct);

			RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
			RTC_TimeStruct.RTC_Hours = 13;
			RTC_TimeStruct.RTC_Minutes= 28;
			RTC_TimeStruct.RTC_Seconds = 30;
			RTC_SetTime(RTC_Format_BIN  , &RTC_TimeStruct);	//配置时间
			
			RTC_DateStruct.RTC_Date = 5;
			RTC_DateStruct.RTC_Month = 3;
			RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Tuesday;
			RTC_DateStruct.RTC_Year = 19;
			RTC_SetDate(RTC_Format_BIN , &RTC_DateStruct);	//配置日期
			RTC_WriteBackupRegister(RTC_BKP_DR0,0x5A50);		//设定备份区间，不需要第二次设置
		
		}
	}
	else
	{
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//RTCCLK的时钟源为LSE
			RCC_RTCCLKCmd(ENABLE);			//使能RTC时钟

			RTC_InitStruct.RTC_AsynchPrediv = 0x7F;
			RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
			RTC_InitStruct.RTC_SynchPrediv = 0xFF;
			RTC_Init(&RTC_InitStruct);		
	}
	return 0;
}

uint32_t get_fattime(void)
{
	struct tm stm;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef	RTC_TimeStruct;
	
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	stm.tm_year = RTC_DateStruct.RTC_Year + 100;
	stm.tm_mon	= RTC_DateStruct.RTC_Month - 1;		
	stm.tm_mday	= RTC_DateStruct.RTC_Date;			
	stm.tm_hour	= RTC_TimeStruct.RTC_Hours - 8;		
	stm.tm_min	= RTC_TimeStruct.RTC_Minutes;   
	stm.tm_sec	= RTC_TimeStruct.RTC_Seconds;

	return mktime(&stm);
}

