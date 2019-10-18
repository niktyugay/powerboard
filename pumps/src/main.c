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
#include "motor.h"
#include "driver.h"
			
uint32_t delay = 0;

extern Syringe syringe1, syringe2;

bool flagValve1, flagValve2;

void initValves(void);

void delay_ms(uint32_t value) {
	delay = value;
	while(delay) {}
}

void SysTick_Handler(void) {
	if (delay > 0) {delay--;}
	//syringe1.timer();
	//syringe2.timer();
}

int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	SPI_Config();
	initSyringes();
	//initMotors();
	flagValve1 = false;
	flagValve2 = false;

	syringe1.encoder.cnt = MAX_ENCODER_CNT; // we don't know, but we assume
	syringe2.encoder.cnt = MAX_ENCODER_CNT; // we don't know, but we assume

	while(1) {
		//syringe1.handler();
		//syringe2.handler();
		if (flagValve1) {
			GPIO_SetBits(GPIOC, GPIO_Pin_0);
		}
		else {
			GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		}
		if (flagValve2) {
			GPIO_SetBits(GPIOC, GPIO_Pin_1);
		}
		else {
			GPIO_ResetBits(GPIOC, GPIO_Pin_1);
		}
	}
}

void initValves() {
	GPIO_InitTypeDef gpio_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpio_struct);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}
