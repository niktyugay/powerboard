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
#include "syringe.h"
			
uint32_t delay = 0;

extern Syringe syringe1, syringe2;

void delay_ms(uint32_t value) {
	delay = value;
	while(delay) {}
}

void SysTick_Handler(void) {
	if (delay > 0) {delay--;}
	syringe1.timer();
	//syringe2.timer();
}

int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	SPI_Config();
	initSyringes();

	syringe1.motor.setPWM(50);
	syringe1.motor.setRotation(UNCLOCKWISE);

	syringe1.en(true);

	while(1) {
		syringe1.handler();
		//syringe2.handler();
	}
}
