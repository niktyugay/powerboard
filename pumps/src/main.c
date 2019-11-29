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
#include "driver.h"

//#define TEST_MOTOR_1
//#define TEST_MOTOR_2
//#define TEST_MOTOR_1_2
			
uint32_t delay = 0;

uint32_t cnttest = 0;

extern Syringe syringe1, syringe2;
uint8_t speedM1, speedM2, directionM1, directionM2;
bool flagChangeM1 = false;
bool flagChangeM2 = false;
bool flagChangeM1M2 = false;
bool flagM1 = false;
bool flagM2 = false;
bool flagM1M2 = false;
Driver_str driver1;
Driver_str  driver2;
uint8_t statusDriver1, statusDriver2;

bool periphState = false;

bool flagValve1 = false;
bool flagValve2 = false;

void initValves(void);
void initDriverEn(void);
void GPIO_Config(void);

void delay_ms(uint32_t value) {
	delay = value;
	while(delay) {}
}

void SysTick_Handler(void) {
	if (delay > 0) {delay--;}
	if (periphState) {
		syringe1.timer();
		syringe2.timer();
	}
}

int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	delay_ms(10);
	GPIO_Config();
	delay_ms(10);
	initDriverEn();
	delay_ms(10);
	initValves();
	delay_ms(10);
	initSyringes();
	delay_ms(10);
	initUart1(); // нижний драйвер и PC8
	delay_ms(10);
	initUart3(); // верхний драйвер и PC9
	periphState = true;
	syringe1.encoder.cnt = MAX_ENCODER_CNT;
	syringe2.encoder.cnt = MAX_ENCODER_CNT;
	delay_ms(10);
	SPI_Config();

	Driver_StructInit(&driver1, USART3);
	statusDriver1 = initDriver(&driver1);
	setDirection(0, &driver1);
	setSpeed(0,&driver1);

	Driver_StructInit(&driver2, USART1);
	statusDriver2 = initDriver(&driver2);
	setDirection(0,&driver2);
	setSpeed(0, &driver2);

#ifdef TEST_MOTOR_1
	delay_ms(100);
	Driver_StructInit(&driver1, USART1);
	statusDriver1 = initDriver(&driver1);
	setDirection(0, &driver1);
	setSpeed(0,&driver1);
	delay_ms(10);
	setSpeed(200, &driver1);
	delay_ms(500);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	delay_ms(10000);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	setSpeed(0,&driver1);
#endif

#ifdef TEST_MOTOR_2
	delay_ms(100);
	Driver_StructInit(&driver2, USART3);
	statusDriver2 = initDriver(&driver2);
	setDirection(0,&driver2);
	setSpeed(0, &driver2);
	delay_ms(10);
	setSpeed(200, &driver2);
	delay_ms(500);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	delay_ms(10000);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	setSpeed(0,&driver2);
#endif

#ifdef TEST_MOTOR_1_2
	setDirection(1,&driver1);
	delay_ms(1);
	setDirection(0,&driver2);
	delay_ms(1);
	setSpeed(50,&driver1);
	delay_ms(1);
	setSpeed(60,&driver2);
	delay_ms(200);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	delay_ms(200);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	delay_ms(10000);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	delay_ms(200);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	setSpeed(0,&driver1);
	delay_ms(1);
	setSpeed(0,&driver2);
#endif

	while(1) {

		/*==================== SYRINGES =================*/
		syringe1.handler();
		syringe2.handler();
		/*===============================================*/

		/*===================== VALVES ==================*/
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
		/*===============================================*/

		/*===================== MOTORS ==================*/
		if (flagChangeM1) {
			flagChangeM1 = false;
			if (flagM1) {
				setDirection(directionM1,&driver1); // usart 3
				setSpeed(speedM1,&driver1);
				GPIO_ResetBits(GPIOC, GPIO_Pin_9);
			}
			else {
				setSpeed(0,&driver1);
				GPIO_SetBits(GPIOC, GPIO_Pin_9);
			}
		}
		if (flagChangeM2) {
			flagChangeM2 = false;
			if (flagM2) {
				setDirection(directionM2,&driver2); // usart 1
				setSpeed(speedM2,&driver2);
				GPIO_ResetBits(GPIOC, GPIO_Pin_8);
			}
			else {
				setSpeed(0,&driver2);
				GPIO_SetBits(GPIOC, GPIO_Pin_8);
			}
		}
		/*===============================================*/

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

void initDriverEn() {
	GPIO_InitTypeDef gpio_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpio_struct);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
}

void GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	/*================================= GPIOA ==================================*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(	GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
	/*==========================================================================*/

	/*================================= GPIOB ==================================*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,	GPIO_Pin_0 );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);
	/*==========================================================================*/

	/*================================= GPIOC ==================================*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,	GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 );
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1); // for close valve
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);
	/*==========================================================================*/

	/*================================= GPIOD ==================================*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, DISABLE);
	/*==========================================================================*/

}
