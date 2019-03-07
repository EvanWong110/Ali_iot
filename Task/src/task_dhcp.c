#include "task_dhcp.h"

#include "Ali_iot.h"

#include "bsp_rtc.h"


void task_DHCP( void *pvParameters )
{
		while(1)
		{
			vTaskDelay(500/portTICK_PERIOD_MS);
		}
}
