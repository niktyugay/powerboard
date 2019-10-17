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
			
uint32_t delay = 0;

extern Syringe syringe1, syringe2;

bool flagValve1, flagValve2;

void initValves(void);
void initUart1(void);
void initUart2(void);
void initMotors(void);
void sendByteUsart1(uint8_t);
void sendByteUsart2(uint8_t);

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

void initUart1() {
	GPIO_InitTypeDef gpio_struct;
	USART_InitTypeDef	usart_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_9;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin = GPIO_Pin_10;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	usart_struct.USART_BaudRate = 115200;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &usart_struct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
}

void initUart2() {
	GPIO_InitTypeDef gpio_struct;
	USART_InitTypeDef	usart_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_2;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin = GPIO_Pin_3;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	usart_struct.USART_BaudRate = 115200;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &usart_struct);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);
}

void initMotors() {
	initUart1();
	initUart2();
}

void USART1_IRQHandler() {
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
		USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);
	}
}

void USART2_IRQHandler() {
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
		USART_ClearITPendingBit(USART2, USART_FLAG_RXNE);
	}
}

void sendByteUsart1(uint8_t byte) {
	while(USART_GetFlagStatus(USART1, USART_IT_TC));
	USART_SendData(USART1, byte);
	delay_ms(1);
}

void sendByteUsart2(uint8_t byte) {
	while(USART_GetFlagStatus(USART2, USART_IT_TC));
	USART_SendData(USART2, byte);
	delay_ms(1);
}
