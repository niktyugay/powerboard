/*
 * syringe.c
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */

#include "syringe.h"

Syringe syringe1, syringe2;

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
	syringe1.param.en =					false;
	syringe1.param.volume = 			0;
	syringe1.param.dose = 				0;
	syringe1.param.direction = 			STOP;
	syringe1.param.position = 			START;
	syringe1.encoder.cnt = 				0;
	syringe1.encoder.timCurrent = 		0;
	syringe1.encoder.timOld =			0;
	syringe1.encoder.timer = 			0;
	syringe1.encoder.PERIOD =			PERIOD_ENC;
	syringe1.en = 						syringe1en;
	syringe1.handler = 					syringe1Handler;
	syringe1.timer =					syringe1Timer;
	syringe1.setSpeed =					setSpeedSyringe1;
	syringe1.getSpeed =					getSpeedSyringe1;
	syringe1.setSpeed(0);
	syringe1.speed.PERIOD =				0;
	syringe1.speed.timer =				0;
	syringe1.getVolume = 				getVolumeSyringe1;
	syringe1.setDose = 					setDoseSyringe1;
	syringe1.setDose(0);
	syringe1.buttonUp =					buttonUpSyringe1;
	syringe1.buttonDown =				buttonDownSyringe1;
	syringe1.motor.setRotation = 		setRotationSyringe1;
	syringe1.motor.setRotation(BREAK);
	syringe1.motor.setPWM = 			setPWMSyringe1;
	syringe1.motor.getPWM = 			getPWMSyringe1;
	syringe1.motor.setPWM(0);
	syringe1.cover.state = 				CLOSE;
	syringe1.cover.hallSensor.delay =	DELAY_SENSOR_CLICK;
	syringe1.cover.getState =			getStateCoverSyringe1;
	syringe1.startSensor.status = 		NO_EVENT;
	syringe1.startSensor.timer_1 =		0;
	syringe1.startSensor.timer_2 =		0;
	syringe1.startSensor.delay =		DELAY_SENSOR_CLICK;
	syringe1.endSensor.status = 		NO_EVENT;
	syringe1.endSensor.timer_1 =		0;
	syringe1.endSensor.timer_2 =		0;
	syringe1.endSensor.delay =			DELAY_SENSOR_CLICK;

	syringe2.param.en =					false;
	syringe2.param.volume = 			0;
	syringe2.param.dose = 				0;
	syringe2.param.direction = 			STOP;
	syringe2.param.position = 			START;
	syringe2.encoder.cnt = 				0;
	syringe2.encoder.timCurrent = 		0;
	syringe2.encoder.timOld =			0;
	syringe2.encoder.timer = 			0;
	syringe2.encoder.PERIOD =			PERIOD_ENC;
	syringe2.en = 						syringe2en;
	syringe2.handler = 					syringe2Handler;
	syringe2.timer =					syringe2Timer;
	syringe2.setSpeed =					setSpeedSyringe2;
	syringe2.getSpeed =					getSpeedSyringe2;
	syringe2.setSpeed(0);
	syringe2.speed.PERIOD =				0;
	syringe2.speed.timer =				0;
	syringe2.getVolume = 				getVolumeSyringe2;
	syringe2.setDose = 					setDoseSyringe2;
	syringe2.setDose(0);
	syringe2.buttonUp =					buttonUpSyringe2;
	syringe2.buttonDown =				buttonDownSyringe2;
	syringe2.motor.setRotation = 		setRotationSyringe2;
	syringe2.motor.setRotation(BREAK);
	syringe2.motor.setPWM = 			setPWMSyringe2;
	syringe2.motor.getPWM = 			getPWMSyringe2;
	syringe2.motor.setPWM(0);
	syringe2.cover.state = 				CLOSE;
	syringe2.cover.hallSensor.delay =	DELAY_SENSOR_CLICK;
	syringe2.cover.getState =			getStateCoverSyringe2;
	syringe2.startSensor.delay =		DELAY_SENSOR_CLICK;
	syringe2.endSensor.delay =			DELAY_SENSOR_CLICK;
}

void	setRotationSyringe1(Rotation rotation) {
	switch (rotation) {
		case	UNCLOCKWISE:
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
			syringe1.motor.rotation = CLOCKWISE;
			syringe1.param.direction = PUSH;
			break;
		case	CLOCKWISE:
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
			GPIO_SetBits(GPIOB, GPIO_Pin_9);
			syringe1.motor.rotation = UNCLOCKWISE;
			syringe1.param.direction = PULL;
			break;
		case	BREAK:
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			GPIO_SetBits(GPIOB, GPIO_Pin_9);
			syringe1.motor.rotation = BREAK;
			syringe1.param.direction = STOP;
			break;
	}
}

void	setRotationSyringe2(Rotation rotation) {
	switch (rotation) {
		case	UNCLOCKWISE:
			GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			GPIO_SetBits(GPIOC, GPIO_Pin_15);
			syringe2.motor.rotation = UNCLOCKWISE;
			syringe2.param.direction = PUSH;
			break;
		case	CLOCKWISE:
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			GPIO_ResetBits(GPIOC, GPIO_Pin_15);
			syringe2.motor.rotation = CLOCKWISE;
			syringe2.param.direction = PULL;
			break;
		case	BREAK:
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			GPIO_SetBits(GPIOC, GPIO_Pin_15);
			syringe2.motor.rotation = BREAK;
			syringe2.param.direction = STOP;
			break;
	}
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

uint8_t	getPWMSyringe1() {
	return syringe1.motor.pwm / 80;
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

uint8_t	getPWMSyringe2() {
	return syringe2.motor.pwm / 80;
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
	syringe1.param.dose = syringe1.encoder.cnt - (DOSE_O_1ML*value)/100;
}

void	setDoseSyringe2(uint16_t value) {
	syringe2.param.dose = syringe2.encoder.cnt - (DOSE_O_1ML*value)/100;
}

uint16_t	getVolumeSyringe1() {
	return syringe1.param.volume;
}

uint16_t	getVolumeSyringe2() {
	return syringe2.param.volume;
}

void	buttonUpSyringe1(ButtonsMode mode) {
	switch(mode) {
	case CLICK:
		break;
	case LONG_CLICK:
		break;
	case DEFAULT:
		break;
	}
}

void	buttonDownSyringe1(ButtonsMode mode) {
	switch(mode) {
	case CLICK:
		break;
	case LONG_CLICK:
		break;
	case DEFAULT:
		break;
	}
}

void	buttonUpSyringe2(ButtonsMode mode) {
	switch(mode) {
	case CLICK:
		break;
	case LONG_CLICK:
		break;
	case DEFAULT:
		break;
	}
}

void	buttonDownSyringe2(ButtonsMode mode) {
	switch(mode) {
	case CLICK:
		break;
	case LONG_CLICK:
		break;
	case DEFAULT:
		break;
	}
}

CoverState	getStateCoverSyringe1() {
	return syringe1.cover.state;
}

CoverState	getStateCoverSyringe2() {
	return syringe2.cover.state;
}

void	syringe1en(bool state) {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	if (state) {
		TIM_OCInitStructure.TIM_Pulse = syringe1.motor.pwm;
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
		TIM_OCInitStructure.TIM_Pulse = syringe2.motor.pwm;
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
		if (syringe1.encoder.cnt == syringe1.param.dose) {
			syringe1.param.dose = 0;
			syringe1.en(false);
		}
	}
	else if (syringe1.speed.dose != 0) {
		if (syringe1.encoder.cnt == syringe1.speed.dose) {
			syringe1.speed.dose = 0;
			syringe1.en(false);
		}
	}
}

void	syringe2Handler() {
	if (syringe2.param.dose != 0) {
		if (syringe2.encoder.cnt == syringe2.param.dose) {
			syringe2.param.dose = 0;
			syringe2.en(false);
		}
	}
	else if (syringe2.speed.dose != 0) {
		if (syringe2.encoder.cnt == syringe2.speed.dose) {
			syringe2.speed.dose = 0;
			syringe2.en(false);
		}
	}
}

void	syringe1Timer() {
	// Syringe 1
	if (syringe1.param.en) {
		syringe1.encoder.timer++;
		if (syringe1.encoder.timer == syringe1.encoder.PERIOD) {
			syringe1.encoder.timer = 0;
			syringe1.encoder.timOld = syringe1.encoder.timCurrent;
			syringe1.encoder.timCurrent = TIM2->CNT;
			//syringe1.encoder.delta = syringe1.encoder.timCurrent - syringe1.encoder.timOld;
			syringe1.encoder.cnt = syringe1.encoder.cnt + syringe1.encoder.timCurrent - syringe1.encoder.timOld;
			//syringe1.encoder.cnt += (syringe1.encoder.timCurrent - syringe1.encoder.timOld);
			syringe1.param.volume = syringe1.encoder.cnt / DOSE_O_1ML;
		}
		syringe1.speed.timer++;
		if (syringe1.speed.timer == syringe1.speed.PERIOD) {
			syringe1.speed.timer = 0;
		}
	}
	// Hall sensor
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == Bit_SET) {
		syringe1.cover.hallSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == Bit_RESET && syringe1.cover.hallSensor.status == PRESSED) {
		syringe1.cover.hallSensor.status = RELEASED;
	}
	if (syringe1.cover.hallSensor.status  == PRESSED) {
		syringe1.cover.hallSensor.timer_1++;
		if (syringe1.cover.hallSensor.timer_1 >= syringe1.cover.hallSensor.delay) {
			syringe1.cover.hallSensor.timer_1 = 0;
			syringe1.en(false);
			syringe1.cover.state = OPEN;
		}
	}
	else if (syringe1.cover.hallSensor.status  == RELEASED) {
		syringe1.cover.hallSensor.timer_2++;
		if (syringe1.cover.hallSensor.timer_2 >= syringe1.cover.hallSensor.delay) {
			syringe1.cover.hallSensor.timer_2 = 0;
			syringe1.cover.state = CLOSE;
		}
	}
	// Limit switch start
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_SET) {
		syringe1.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET && syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.status = RELEASED;
	}
	if (syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.timer_1++;
		if (syringe1.startSensor.timer_1 == syringe1.startSensor.delay) {
			syringe1.param.volume = MAX_VOLUME;
			syringe1.param.position = START;
			if (syringe1.param.direction == PULL) {
				syringe1.en(false);
			}
		}
	}
	else if (syringe1.startSensor.status == RELEASED) {
		syringe1.startSensor.timer_2++;
		if (syringe1.startSensor.timer_2 == syringe1.startSensor.delay) {
			syringe1.param.position = MIDDLE;
			syringe1.startSensor.status = NO_EVENT;
			syringe1.startSensor.timer_1 = 0;
			syringe1.startSensor.timer_2 = 0;
		}
	}
	// Limit switch end
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET) {
		syringe1.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET && syringe1.endSensor.status == PRESSED) {
		syringe1.endSensor.status = RELEASED;
	}
	if (syringe1.endSensor.status  == PRESSED) {
		syringe1.endSensor.timer_1++;
		if (syringe1.endSensor.timer_1 == syringe1.endSensor.delay) {
			syringe1.param.volume = 0;
			syringe1.param.position = END;
			if (syringe1.param.direction == PUSH) {
				syringe1.en(false);
			}
		}
	}
	else if (syringe1.endSensor.status == RELEASED) {
		syringe1.endSensor.timer_2++;
		if (syringe1.endSensor.timer_2 == syringe1.endSensor.delay) {
			syringe1.param.position = MIDDLE;
			syringe1.endSensor.status = NO_EVENT;
			syringe1.endSensor.timer_1 = 0;
			syringe1.endSensor.timer_2 = 0;
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
			syringe2.param.volume = syringe2.encoder.cnt / DOSE_O_1ML;
		}
		syringe2.speed.timer++;
		if (syringe2.speed.timer == syringe2.speed.PERIOD) {
			syringe2.speed.timer = 0;
		}
	}
	// Hall sensor
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_RESET) {
		syringe2.cover.hallSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_SET) {
		syringe2.cover.hallSensor.status = RELEASED;
	}
	if (syringe2.cover.hallSensor.status == PRESSED) {
		syringe2.cover.hallSensor.timer_1++;
		if (syringe2.cover.hallSensor.timer_1 >= syringe2.cover.hallSensor.delay) {
			syringe2.cover.hallSensor.timer_1 = 0;
			syringe2.en(false);
			syringe2.cover.state = OPEN;
		}
	}
	else if (syringe2.cover.hallSensor.status == RELEASED) {
		syringe2.cover.hallSensor.timer_2++;
		if (syringe2.cover.hallSensor.timer_2 >= syringe2.cover.hallSensor.delay) {
			syringe2.cover.hallSensor.timer_2 = 0;
			syringe2.cover.state = CLOSE;
		}
	}
	// Limit switch
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET) {
		syringe2.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET) {
		syringe2.startSensor.status = RELEASED;
	}
	if (syringe2.startSensor.status == PRESSED) {
		syringe2.startSensor.timer_1++;
		if (syringe2.startSensor.timer_1 == syringe2.startSensor.delay) {
			syringe2.param.volume = MAX_VOLUME;
			syringe2.param.position = START;
			if (syringe2.param.direction == PULL) {
				syringe2.en(false);
			}
		}
	}
	else if (syringe2.startSensor.status == RELEASED) {
		syringe2.startSensor.timer_2++;
		if (syringe2.startSensor.timer_2 == syringe2.startSensor.delay) {
			syringe2.param.position = MIDDLE;
			syringe2.startSensor.status = NO_EVENT;
			syringe2.startSensor.timer_1 = 0;
			syringe2.startSensor.timer_2 = 0;
		}
	}
	// Limit switch
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_SET) {
		syringe2.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET) {
		syringe2.endSensor.status = RELEASED;
	}
	if (syringe2.endSensor.status  == PRESSED) {
		syringe2.endSensor.timer_1++;
		if (syringe2.endSensor.timer_1 == syringe2.endSensor.delay) {
			syringe2.param.volume = 0;
			syringe2.param.position = END;
			if (syringe2.param.direction == PUSH) {
				syringe2.en(false);
			}
		}
	}
	else if (syringe2.endSensor.status  == RELEASED) {
		syringe2.endSensor.timer_2++;
		if (syringe2.endSensor.timer_2 == syringe2.endSensor.delay) {
			syringe2.param.position = MIDDLE;
			syringe2.endSensor.status  = NO_EVENT;
			syringe2.endSensor.timer_1 = 0;
			syringe2.endSensor.timer_2 = 0;
		}
	}
}

