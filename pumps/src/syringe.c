/*
 * syringe.c
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */

#include "syringe.h"

Syringe syringe1, syringe2;
Sensors startSensorSyringe1, endSensorSyringe1, hallSensorSyringe1;
Sensors startSensorSyringe2, endSensorSyringe2, hallSensorSyringe2;
Buttons	buttonUpSyringe1, buttonDownSyringe1;
Buttons	buttonUpSyringe2, buttonDownSyringe2;

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
	// PWM initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 8000;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	// AIN1,2 pins initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	// BIN1,2 pins initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	// STBY pin initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	TIM_Cmd(TIM4, ENABLE);
	// encoder initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	// limit switch initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Hall sensor
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// Buttons
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void	initSyringes() {
	initHardware();
	syringe1.en = 						syringe1en;
	syringe1.handler = 					syringe1Handler;
	syringe1.timer =					syringe1Timer;
	syringe1.getVolume = 				getVolumeSyringe1;
	syringe1.speed.setSpeed =			setSpeedSyringe1;
	syringe1.speed.getSpeed =			getSpeedSyringe1;
	syringe1.setDose = 					setDoseSyringe1;
	syringe1.setDirection =				setDirectionSyringe1;
	syringe1.getDirection =				getDirectionSyringe1;
	syringe1.motor.setPWM = 			setPWMSyringe1;
	syringe1.motor.getPWM = 			getPWMSyringe1;
	syringe1.encoder.getEncoder = 		getEncoderSyringe1;
	syringe2.en = 						syringe2en;
	syringe2.handler = 					syringe2Handler;
	syringe2.timer =					syringe2Timer;
	syringe2.getVolume = 				getVolumeSyringe2;
	syringe2.speed.setSpeed = 			setSpeedSyringe2;
	syringe2.speed.getSpeed = 			getSpeedSyringe2;
	syringe2.setDose = 					setDoseSyringe2;
	syringe2.motor.setPWM = 			setPWMSyringe2;
	syringe2.motor.getPWM = 			getPWMSyringe2;
	syringe2.encoder.getEncoder = 		getEncoderSyringe2;
	syringe1.setPosition = 				setPositionSyringe1;
	syringe1.getPosition = 				getPositionSyringe1;
	syringe1.cover.getState =			getStateCoverSyringe1;
	syringe2.setPosition = 				setPositionSyringe2;
	syringe2.getPosition = 				getPositionSyringe2;
	syringe2.cover.getState = 			getStateCoverSyringe2;

	syringe1.param.en =					false;
	syringe1.motor.setPWM(0);
	syringe1.encoder.cnt = 				0;
	syringe1.encoder.timCurrent = 		0;
	syringe1.encoder.timOld =			0;
	syringe1.encoder.timer = 			0;
	syringe1.encoder.PERIOD =			PERIOD_ENC;
	syringe1.speed.setSpeed(0);
	syringe1.speed.PERIOD =				0;
	syringe1.speed.timer =				0;
	startSensorSyringe1.delay =			100;
	startSensorSyringe1.status =		NO_EVENT;
	startSensorSyringe1.timer_1 =		0;
	startSensorSyringe1.timer_2 =		0;
	endSensorSyringe1.delay =			100;
	endSensorSyringe1.status =			NO_EVENT;
	endSensorSyringe1.timer_1 =			0;
	endSensorSyringe1.timer_2 =			0;

	syringe2.param.en =					false;
	syringe2.motor.setPWM(0);
	syringe2.encoder.cnt = 				0;
	syringe2.encoder.timCurrent = 		0;
	syringe2.encoder.timOld =			0;
	syringe2.encoder.timer = 			0;
	syringe2.encoder.PERIOD =			PERIOD_ENC;
	syringe2.speed.setSpeed(0);
	syringe2.speed.PERIOD =				0;
	syringe2.speed.timer =				0;
	startSensorSyringe2.delay =			100;
	startSensorSyringe2.status =		NO_EVENT;
	startSensorSyringe2.timer_1 =		0;
	startSensorSyringe2.timer_2 =		0;
	endSensorSyringe2.delay =			100;
	endSensorSyringe2.status =			NO_EVENT;
	endSensorSyringe2.timer_1 =			0;
	endSensorSyringe2.timer_2 =			0;
}

void	setPWMSyringe1(uint8_t dutyCycle) {
	if (dutyCycle < 0) {
		dutyCycle = 0;
	}
	else if (dutyCycle > 100) {
		dutyCycle  = 100;
	}
	syringe1.motor.pwm = dutyCycle * 80;
}

uint16_t	getPWMSyringe1() {
	return syringe1.motor.pwm;
}

void	setPWMSyringe2(uint8_t dutyCycle) {
	if (dutyCycle < 0) {
		dutyCycle = 0;
	}
	else if (dutyCycle > 100) {
		dutyCycle  = 100;
	}
	syringe2.motor.pwm = dutyCycle * 80;
}

uint16_t	getPWMSyringe2() {
	return syringe2.motor.pwm;
}

void	setEncoderSyringe1(uint16_t value) {
	TIM2->CNT = value;
}

uint16_t	getEncoderSyringe1() {
	return syringe1.encoder.cnt;
}

void	setEncoderSyringe2(uint16_t value) {
	TIM3->CNT = value;
}

uint16_t	getEncoderSyringe2() {
	return syringe2.encoder.cnt;
}

void	setSpeedSyringe1(uint16_t speed) {
	if (speed > 0) {
		syringe1.speed.speed = speed;
		syringe1.speed.PERIOD = 3600000/(speed/100);
	}
	else {
		syringe1.speed.speed = 0;
		syringe1.speed.PERIOD = 0;
	}
}

uint16_t	getSpeedSyringe1() {
	return syringe1.speed.speed;
}

void	setSpeedSyringe2(uint16_t speed) {
	if (speed > 0) {
		syringe2.speed.speed = speed;
		syringe2.speed.PERIOD = 3600000/(speed/100);
	}
	else {
		syringe2.speed.speed = 0;
		syringe2.speed.PERIOD = 0;
	}
}

uint16_t	getSpeedSyringe2() {
	return syringe2.speed.speed;
}

void	setDoseSyringe1(uint16_t value) {
	syringe1.param.dose = syringe1.encoder.getEncoder() - (DOSE_O_1ML*value)/100;
}

void	setDoseSyringe2(uint16_t value) {
	syringe2.param.dose = syringe2.encoder.getEncoder() - (DOSE_O_1ML*value)/100;
}

uint16_t	getVolumeSyringe1() {
	return syringe1.param.volume;
}

uint16_t	getVolumeSyringe2() {
	return syringe2.param.volume;
}

void	setDirectionSyringe1(Direction direction) {
	switch (direction) {
	case	PUSH:
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		syringe1.param.direction = PUSH;
		break;
	case	PULL:
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		syringe1.param.direction = PULL;
		break;
	case	STOP:
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		syringe1.param.direction = STOP;
		break;
	}
}

Direction	getDirectionSyringe1() {
	return syringe1.param.direction;
}

void	setDirectionSyringe2(Direction direction) {
	switch (direction) {
	case	PUSH:
		GPIO_ResetBits(GPIOC, GPIO_Pin_14);
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
		syringe2.param.direction = PUSH;
		break;
	case	PULL:
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
		syringe2.param.direction = PULL;
		break;
	case	STOP:
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
		syringe2.param.direction = STOP;
		break;
	}
}

Direction	getDirectionSyringe2() {
	return syringe2.param.direction;
}

void	setPositionSyringe1(Position position) {
	syringe1.param.position = position;
}

Position	getPositionSyringe1() {
	return syringe1.param.position;
}

void	setPositionSyringe2(Position position) {
	syringe2.param.position = position;
}

Position	getPositionSyringe2() {
	return syringe2.param.position;
}

Cover	getStateCoverSyringe1() {
	return syringe1.cover.state;
}

Cover	getStateCoverSyringe2() {
	return syringe2.cover.state;
}

void	syringe1en(bool state) {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	if (state) {
		TIM_OCInitStructure.TIM_Pulse = syringe1.motor.getPWM();
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		syringe1.param.en = true;
	}
	else {
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		syringe1.param.en = false;
	}
}

void	syringe2en(bool state) {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	if (state) {
		TIM_OCInitStructure.TIM_Pulse = syringe2.motor.getPWM();
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		syringe2.param.en = true;
	}
	else {
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		syringe2.param.en = false;
	}
}

void	syringe1Handler() {
	if (syringe1.param.dose != 0) {
		if (syringe1.encoder.getEncoder() == syringe1.param.dose) {
			syringe1.param.dose = 0;
			syringe1.en(false);
		}
	}
	else if (syringe1.speed.dose != 0) {
		if (syringe1.encoder.getEncoder() == syringe1.speed.dose) {
			syringe1.speed.dose = 0;
			syringe1.en(false);
		}
	}
}

void	syringe2Handler() {
	if (syringe2.param.dose != 0) {
		if (syringe2.encoder.getEncoder() == syringe2.param.dose) {
			syringe2.param.dose = 0;
			syringe2.en(false);
		}
	}
	else if (syringe2.speed.dose != 0) {
		if (syringe2.encoder.getEncoder() == syringe2.speed.dose) {
			syringe2.speed.dose = 0;
			syringe2.en(false);
		}
	}
}

void	syringe1Timer() {
	// Syringe 1
	if (syringe1.param.en) {
		syringe1.encoder.timer++;
		if (syringe1.encoder.timer == syringe1.encoder.timer) {
			syringe1.encoder.timer = 0;
			syringe1.encoder.timOld = syringe1.encoder.timCurrent;
			syringe1.encoder.timCurrent = TIM2->CNT;
			syringe1.encoder.cnt += syringe1.encoder.timCurrent - syringe1.encoder.timOld;
			syringe1.param.volume = syringe1.encoder.getEncoder() / DOSE_O_1ML;
		}
		syringe1.speed.timer++;
		if (syringe1.speed.timer == syringe1.speed.PERIOD) {
			syringe1.speed.timer = 0;
		}
	}
	// Hall sensor
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == Bit_SET) {
		hallSensorSyringe1.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == Bit_RESET && hallSensorSyringe1.status == PRESSED) {
		hallSensorSyringe1.status = RELEASED;
	}
	if (hallSensorSyringe1.status == PRESSED) {
		hallSensorSyringe1.timer_1++;
		if (hallSensorSyringe1.timer_1 >= hallSensorSyringe1.delay) {
			hallSensorSyringe1.timer_1 = 0;
			syringe1.en(false);
			syringe1.cover(OPEN);
		}
	}
	else if (hallSensorSyringe1.status == RELEASED) {
		hallSensorSyringe1.timer_2++;
		if (hallSensorSyringe1.timer_2 >= hallSensorSyringe1.delay) {
			hallSensorSyringe1.timer_2 = 0;
			syringe1.cover(CLOSE);
		}
	}

	// Limit switch start
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_SET) {
		startSensorSyringe1.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET && startSensorSyringe1.status == PRESSED) {
		startSensorSyringe1.status = RELEASED;
	}
	if (startSensorSyringe1.status == PRESSED) {
		startSensorSyringe1.timer_1++;
		if (startSensorSyringe1.timer_1 == startSensorSyringe1.delay) {
			syringe1.setVolume(MAX_VOLUME);
			syringe1.setPosition(START);
			if (syringe1.getDirection() == PULL) {
				syringe1.en(false);
			}
		}
	}
	else if (startSensorSyringe1.status == RELEASED) {
		startSensorSyringe1.timer_2++;
		if (startSensorSyringe1.timer_2 == startSensorSyringe1.delay) {
			syringe1.setPosition(MIDDLE);
			startSensorSyringe1.status = NO_EVENT;
			startSensorSyringe1.timer_1 = 0;
			startSensorSyringe1.timer_2 = 0;
		}
	}

	// Limit switch end
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET) {
		endSensorSyringe1.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET && endSensorSyringe1.status == PRESSED) {
		endSensorSyringe1.status = RELEASED;
	}
	if (endSensorSyringe1.status == PRESSED) {
		endSensorSyringe1.timer_1++;
		if (endSensorSyringe1.timer_1 == endSensorSyringe1.delay) {
			syringe1.setVolume(0);
			syringe1.setPosition(END);
			if (syringe1.getDirection() == PUSH) {
				syringe1.en(false);
			}
		}
	}
	else if (endSensorSyringe1.status == RELEASED) {
		endSensorSyringe1.timer_2++;
		if (endSensorSyringe1.timer_2 == endSensorSyringe1.delay) {
			syringe1.setPosition(MIDDLE);
			endSensorSyringe1.status = NO_EVENT;
			endSensorSyringe1.timer_1 = 0;
			endSensorSyringe1.timer_2 = 0;
		}
	}
}

void	syringe2Timer() {
	if (syringe2.param.en) {
		syringe2.encoder.timer++;
		if (syringe2.encoder.timer == syringe2.encoder.PERIOD) {
			syringe2.encoder.timer = 0;
			syringe2.encoder.timOld = syringe2.encoder.timCurrent;
			syringe2.encoder.timCurrent = TIM3->CNT;
			syringe2.encoder.cnt += syringe2.encoder.timCurrent - syringe2.encoder.timOld;
			syringe2.param.volume = syringe2.encoder.getEncoder() / DOSE_O_1ML;
		}
		syringe2.speed.timer++;
		if (syringe2.speed.timer == syringe2.speed.PERIOD) {
			syringe2.speed.timer = 0;
		}
	}
	// Hall sensor
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_RESET) {
		hallSensorSyringe2.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_SET) {
		hallSensorSyringe2.status = RELEASED;
	}
	if (hallSensorSyringe2.status == PRESSED) {
		hallSensorSyringe2.timer_1++;
		if (hallSensorSyringe2.timer_1 >= hallSensorSyringe2.delay) {
			hallSensorSyringe2.timer_1 = 0;
			syringe2.en(false);
			syringe2.cover(OPEN);
		}
	}
	else if (hallSensorSyringe2.status == RELEASED) {
		hallSensorSyringe2.timer_2++;
		if (hallSensorSyringe2.timer_2 >= hallSensorSyringe2.delay) {
			hallSensorSyringe2.timer_2 = 0;
			syringe2.cover(CLOSE);
		}
	}
	// Limit switch
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET) {
		startSensorSyringe2.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET) {
		startSensorSyringe2.status = RELEASED;
	}
	if (startSensorSyringe2.status == PRESSED) {
		startSensorSyringe2.timer_1++;
		if (startSensorSyringe2.timer_1 == startSensorSyringe2.delay) {
			syringe2.setVolume(MAX_VOLUME);
			syringe2.setPosition(START);
			if (syringe2.getDirection() == PULL) {
				syringe2.en(false);
			}
		}
	}
	else if (startSensorSyringe2.status == RELEASED) {
		startSensorSyringe2.timer_2++;
		if (startSensorSyringe2.timer_2 == startSensorSyringe2.delay) {
			syringe2.setPosition(MIDDLE);
			startSensorSyringe2.status = NO_EVENT;
			startSensorSyringe2.timer_1 = 0;
			startSensorSyringe2.timer_2 = 0;
		}
	}
	// Limit switch
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_SET) {
		endSensorSyringe2.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET) {
		endSensorSyringe2.status = RELEASED;
	}
	if (endSensorSyringe2.status == PRESSED) {
		endSensorSyringe2.timer_1++;
		if (endSensorSyringe2.timer_1 == endSensorSyringe2.delay) {
			syringe2.setVolume(0);
			syringe2.setPosition(END);
			if (syringe2.getDirection() == PUSH) {
				syringe2.en(false);
			}
		}
	}
	else if (endSensorSyringe2.status == RELEASED) {
		endSensorSyringe2.timer_2++;
		if (endSensorSyringe2.timer_2 == endSensorSyringe2.delay) {
			syringe2.setPosition(MIDDLE);
			endSensorSyringe2.status = NO_EVENT;
			endSensorSyringe2.timer_1 = 0;
			endSensorSyringe2.timer_2 = 0;
		}
	}
}

