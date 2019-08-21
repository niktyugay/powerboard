/*
 * syringe.c
 *
 *  Created on: 19 рту. 2019 у.
 *      Author: tugay
 */

#include "syringe.h"

syringe syringe1, syringe2;

void initHardware() {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 8000;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void initSyringes() {
	syringe1.param.en = false;
	syringe1.setPWM = setPWMSyringe1;
	syringe1.setSpeed = setSpeedSyringe1;
	syringe1.setDose = setDoseSyringe1;
	syringe1.getPWM = getPWMSyringe1;
	syringe1.getSpeed = getSpeedSyringe1;
	syringe1.en = syringe1en;
	syringe1.setPWM(0);
	syringe1.setDose(0);
	syringe1.setSpeed(0);

	syringe2.param.en = false;
	syringe2.setPWM = setPWMSyringe2;
	syringe2.setSpeed = setSpeedSyringe2;
	syringe2.setDose = setDoseSyringe2;
	syringe2.getPWM = getPWMSyringe2;
	syringe2.getSpeed = getSpeedSyringe2;
	syringe2.en = syringe2en;
	syringe2.setPWM(0);
	syringe2.setDose(0);
	syringe2.setSpeed(0);
}

void setPWMSyringe1(uint8_t dutyCycle) {
	syringe1.param.pwm = dutyCycle;
}

void setPWMSyringe2(uint8_t dutyCycle) {
	syringe2.param.pwm = dutyCycle;
}

void setSpeedSyringe1(uint8_t speed) {
	syringe1.param.speed = speed;
	syringe1.param.period = 3600000/(speed/100);
}

void setSpeedSyringe2(uint8_t speed) {
	syringe2.param.speed = speed;
	syringe2.param.period = 3600000/(speed/100);
}

void setVolumeSyringe1(uint8_t dose) {
	syringe1.param.dose = dose;
}

void setVolumeSyringe2(uint8_t dose) {
	syringe2.param.dose = dose;
}

void	syringe1en(bool state) {
	if (state) {
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_Pulse = syringe1.getPWM;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_Cmd(TIM4, ENABLE);
	}
	else {

	}
}

void	syringe2en(bool state) {
	if (state) {
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_Pulse = syringe2.getPWM;
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_Cmd(TIM4, ENABLE);
	}
	else {

	}
}
