
#include "stm32f4xx_it.h"

void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
	printf("HardFault Handler \n");
	for(;;)
	{
		
	}
}


void MemManage_Handler(void)
{

  while (1)
  {
  }
}


void BusFault_Handler(void)
{

  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  while (1)
  {
  }
}


void DebugMon_Handler(void)
{
}
