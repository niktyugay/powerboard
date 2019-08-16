/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "spi.h"
			
uint32_t delay = 0;

void delay_ms(uint32_t value) {
	delay = value;
	while(delay) {}
}

void SysTick_Handler(void) {
	if (delay > 0) {delay--;}
}

int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	while(1) {

	}
}
