

#include "task_init.h"



void task_init( void *pvParameters )
{
	ETH_BSP_Config();
	
  for(;;)
  {
		xTaskCreate(task_DHCP, "DHCP", configMINIMAL_STACK_SIZE * 8, NULL,4, NULL);

		vTaskDelete(NULL);
  }
}

