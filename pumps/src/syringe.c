/*
 * syringe.c
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */

#include "syringe.h"

syringe syringe1, syringe2;

void	initHardware() {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// PWM init
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	// AIN1,2; BIN1,2; STBY pins init
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	// encoder init
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
	TIM_Cmd(TIM4, ENABLE);
}

void	initSyringes() {
	initHardware();

	syringe1.handler = syringe1Handler;
	syringe1.param.en = false;
	syringe1.setPWM = setPWMSyringe1;
	syringe1.getEncCnt = getEncCntSyringe1;
	syringe1.setSpeed = setSpeedSyringe1;
	syringe1.setDose = setDoseSyringe1;
	syringe1.getPWM = getPWMSyringe1;
	syringe1.getSpeed = getSpeedSyringe1;
	syringe1.en = syringe1en;

	syringe2.handler = syringe2Handler;
	syringe2.param.en = false;
	syringe2.setPWM = setPWMSyringe2;
	syringe2.setEncCnt = setEncCntSyringe2;
	syringe2.getEncCnt = getEncCntSyringe2;
	syringe2.setSpeed = setSpeedSyringe2;
	syringe2.setDose = setDoseSyringe2;
	syringe2.getPWM = getPWMSyringe2;
	syringe2.getSpeed = getSpeedSyringe2;
	syringe2.en = syringe2en;

	syringe1.setPWM(0);
	syringe1.setDose(0);
	syringe1.setSpeed(0);
	syringe1.param.timerForSpeed = 0;

	syringe2.setPWM(0);
	syringe2.setDose(0);
	syringe2.setSpeed(0);
	syringe2.param.timerForSpeed = 0;
}

void	setPWMSyringe1(uint8_t dutyCycle) {
	if (dutyCycle < 0) {
		dutyCycle = 0;
	}
	else if (dutyCycle > 100) {
		dutyCycle  = 100;
	}
	syringe1.param.pwm = dutyCycle;
}

uint8_t	getPWMSyringe1() {
	return syringe1.param.pwm;
}

void	setPWMSyringe2(uint8_t dutyCycle) {
	if (dutyCycle < 0) {
		dutyCycle = 0;
	}
	else if (dutyCycle > 100) {
		dutyCycle  = 100;
	}
	syringe2.param.pwm = dutyCycle;
}

uint8_t	getPWMSyringe2() {
	return syringe2.param.pwm;
}

void	setEncCntSyringe1(uint16_t value) {

}

uint16_t	getEncCntSyringe1() {
	return 0;
}

void	setEncCntSyringe2(uint16_t value) {

}

uint16_t	getEncCntSyringe2() {
	return 0;
}

void	setSpeedSyringe1(uint16_t speed) {
	if (speed < 0) {
		speed = 0;
	}
	syringe1.param.speed = speed;
	syringe1.param.period = 3600000/(speed/100);
}

uint16_t	getSpeedSyringe1() {
	return syringe1.param.speed;
}

void	setSpeedSyringe2(uint16_t speed) {
	if (speed < 0) {
		speed = 0;
	}
	syringe2.param.speed = speed;
	syringe2.param.period = 3600000/(speed/100);
}

uint16_t	getSpeedSyringe2() {
	return syringe2.param.speed;
}

void	setDoseSyringe1(uint16_t dose) {
	if (dose > 20000) {
		dose = 20000;
	}
	syringe1.param.dose = dose;
}

void	setDoseSyringe2(uint16_t dose) {
	if (dose > 20000) {
		dose = 20000;
	}
	syringe2.param.dose = dose;
}

void	syringe1en(bool state) {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	if (state) {
		TIM_OCInitStructure.TIM_Pulse = syringe1.getPWM();
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	}
	else {
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	}
}

void	syringe2en(bool state) {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	if (state) {
		TIM_OCInitStructure.TIM_Pulse = syringe2.getPWM();
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	}
	else {
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	}
}

void	backToStartPositionSyringe1() {

}

void	backToStartPositionSyringe2() {

}

void	syringe1Handler() {
	if (syringe1.param.dose != 0) {
		if (syringe1.getEncCnt() == syringe1.param.dose) {
			syringe1.param.dose = 0;
			syringe1.en(false);
		}
	}
}

void	syringe2Handler() {

}
