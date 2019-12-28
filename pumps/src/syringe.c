/*
 * syringe.c
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */

#include "syringe.h"

// WHITE MHN
//#define VER1
// GREEN MHN
#define VER2

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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	// limit switch initialization
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Hall sensor
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// Buttons
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void	initSyringes() {
	initHardware();
	syringe1.en =						false;
	syringe1.volume = 					0;
	syringe1.dose = 					0;
	syringe1.speed =					0;
	syringe1.encoder.cnt = 				0;
	syringe1.motor.pwm = 				0;
	syringe1.cover.state = 				CLOSE;
	syringe1.cover.hallSensor.delay =	DELAY_SENSOR_CLICK;
	syringe1.startSensor.status = 		NO_EVENT;
	syringe1.startSensor.timer_1 =		0;
	syringe1.startSensor.timer_2 =		0;
	syringe1.startSensor.delay =		DELAY_SENSOR_CLICK;
	syringe1.endSensor.status = 		NO_EVENT;
	syringe1.endSensor.timer_1 =		0;
	syringe1.endSensor.timer_2 =		0;
	syringe1.endSensor.delay =			DELAY_SENSOR_CLICK;
	syringe1.buttonUp.delayClick =		DELAY_CLICK;
	syringe1.buttonUp.delayLongClick =	DELAY_LONGCLICK;
	syringe1.buttonUp.mode =			DEFAULT;
	syringe1.buttonUp.status =			NO_EVENT;
	syringe1.buttonUp.timer_1 =			0;
	syringe1.buttonUp.timer_2 = 		0;
	syringe1.buttonDown.delayClick =	DELAY_CLICK;
	syringe1.buttonDown.delayLongClick =DELAY_LONGCLICK;
	syringe1.buttonDown.mode =			DEFAULT;
	syringe1.buttonDown.status =		NO_EVENT;
	syringe1.buttonDown.timer_1 =		0;
	syringe1.buttonDown.timer_2 = 		0;
	syringe1.handlerMain = 				syringe1Handler;
	syringe1.handlerTimer =				syringe1Timer;
	syringe1.setDose = 					setDoseSyringe1;
	syringe1.motor.setRotation = 		setRotationSyringe1;
	syringe1.motor.setPWM = 			setPWMSyringe1;
	syringe1.motor.getPWM = 			getPWMSyringe1;
	syringe1.start =					syringe1Start;
	syringe1.stop =						syringe1Stop;

	syringe2.en =						false;
	syringe2.volume = 					0;
	syringe2.dose = 					0;
	syringe2.speed =					0;
	syringe2.encoder.cnt = 				0;
	syringe2.motor.pwm = 				0;
	syringe2.cover.state = 				CLOSE;
	syringe2.cover.hallSensor.delay =	DELAY_SENSOR_CLICK;
	syringe2.startSensor.status = 		NO_EVENT;
	syringe2.startSensor.timer_1 =		0;
	syringe2.startSensor.timer_2 =		0;
	syringe2.startSensor.delay =		DELAY_SENSOR_CLICK;
	syringe2.endSensor.status = 		NO_EVENT;
	syringe2.endSensor.timer_1 =		0;
	syringe2.endSensor.timer_2 =		0;
	syringe2.endSensor.delay =			DELAY_SENSOR_CLICK;
	syringe2.buttonUp.delayClick =		DELAY_CLICK;
	syringe2.buttonUp.delayLongClick =	DELAY_LONGCLICK;
	syringe2.buttonUp.mode =			DEFAULT;
	syringe2.buttonUp.status =			NO_EVENT;
	syringe2.buttonUp.timer_1 =			0;
	syringe2.buttonUp.timer_2 = 		0;
	syringe2.buttonDown.delayClick =	DELAY_CLICK;
	syringe2.buttonDown.delayLongClick =DELAY_LONGCLICK;
	syringe2.buttonDown.mode =			DEFAULT;
	syringe2.buttonDown.status =		NO_EVENT;
	syringe2.buttonDown.timer_1 =		0;
	syringe2.buttonDown.timer_2 = 		0;
	syringe2.handlerMain = 				syringe2Handler;
	syringe2.handlerTimer =				syringe2Timer;
	syringe2.setDose = 					setDoseSyringe2;
	syringe2.motor.setRotation = 		setRotationSyringe2;
	syringe2.motor.setPWM = 			setPWMSyringe2;
	syringe2.motor.getPWM = 			getPWMSyringe2;
	syringe2.start =					syringe2Start;
	syringe2.stop =						syringe2Stop;
}

void	setRotationSyringe1(Rotation rotation) {
	switch (rotation) {
		case	CLOCKWISE:
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
#ifdef VER2
			TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
#endif
#ifdef VER1
				TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
#endif

			syringe1.motor.rotation = CLOCKWISE;
			break;
		case	UNCLOCKWISE:
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
			GPIO_SetBits(GPIOB, GPIO_Pin_9);
#ifdef VER2
				TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
#endif
#ifdef VER1
				TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
#endif
			syringe1.motor.rotation = UNCLOCKWISE;
			break;
	}
}

void	setRotationSyringe2(Rotation rotation) {
	switch (rotation) {
		case	CLOCKWISE:
			GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			GPIO_SetBits(GPIOC, GPIO_Pin_15);
#ifdef VER2
			TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
#endif
#ifdef VER1
			TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
#endif
			syringe2.motor.rotation = CLOCKWISE;
			break;
		case	UNCLOCKWISE:
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			GPIO_ResetBits(GPIOC, GPIO_Pin_15);
#ifdef VER2
			TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
#endif
#ifdef VER1
			TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
#endif
			syringe2.motor.rotation = UNCLOCKWISE;
			break;
	}
}

void	setPWMSyringe1(uint8_t dutyCycle) {
	syringe1.motor.pwm = dutyCycle * 80;
}

uint8_t	getPWMSyringe1() {
	return syringe1.motor.pwm / 80;
}

void	setPWMSyringe2(uint8_t dutyCycle) {
	syringe2.motor.pwm = dutyCycle * 80;
}

uint8_t	getPWMSyringe2() {
	return syringe2.motor.pwm / 80;
}

void	setDoseSyringe1(uint16_t value) {
	syringe1.dose = DOSE_O_1ML*value;
	syringe1.motor.setPWM(50);
	syringe1.motor.setRotation(UNCLOCKWISE);
}

void	setDoseSyringe2(uint16_t value) {
	syringe2.dose = DOSE_O_1ML*value;
	syringe2.motor.setPWM(50);
	syringe2.motor.setRotation(UNCLOCKWISE);
}

CoverState	getStateCoverSyringe1() {
	return syringe1.cover.state;
}

CoverState	getStateCoverSyringe2() {
	return syringe2.cover.state;
}

void	syringe1Start() {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = syringe1.motor.pwm;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	syringe1.en = true;
}

void	syringe1Stop() {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	syringe1.en = false;
}

void	syringe1Timer() {
	// Syringe 1
	/*==========================================================================================*/
	if (syringe1.en == true) {
		if (syringe1.dose != 0) {
			if (TIM2->CNT >= syringe1.dose) {
				syringe1.stop();
				syringe1.dose = 0;
#ifdef VER1
				if (syringe1.motor.rotation == UNCLOCKWISE) { // PULL
					syringe1.encoder.cnt = syringe1.encoder.cnt + TIM2->CNT;
				}
				else if (syringe1.motor.rotation == CLOCKWISE) { // PUSH
					syringe1.encoder.cnt = syringe1.encoder.cnt - TIM2->CNT;
				}
#endif
#ifdef VER2
				if (syringe1.motor.rotation == UNCLOCKWISE) { // PUSH
					syringe1.encoder.cnt = syringe1.encoder.cnt - TIM2->CNT;
				}
				else if (syringe1.motor.rotation == CLOCKWISE) { // PULL
					syringe1.encoder.cnt = syringe1.encoder.cnt + TIM2->CNT;
				}
#endif
				TIM2->CNT = 0;
			}
		}
		else {
#ifdef VER1
			if (syringe1.motor.rotation == UNCLOCKWISE) { // PULL
				syringe1.encoder.cnt = syringe1.encoder.cnt + TIM2->CNT;
			}
			else if (syringe1.motor.rotation == CLOCKWISE) { // PUSH
				syringe1.encoder.cnt = syringe1.encoder.cnt - TIM2->CNT;
			}
#endif
#ifdef VER2
			if (syringe1.motor.rotation == UNCLOCKWISE) { // PUSH
				syringe1.encoder.cnt = syringe1.encoder.cnt - TIM2->CNT;
			}
			else if (syringe1.motor.rotation == CLOCKWISE) { // PULL
				syringe1.encoder.cnt = syringe1.encoder.cnt + TIM2->CNT;
			}
#endif
			TIM2->CNT = 0;
		}
	}
	/*==========================================================================================*/
	if (syringe1.speed != 0 && syringe1.cover.state == CLOSE && syringe1.endSensor.status != PRESSED) {
		if (syringe1.timer == 0) {
			syringe1.timer = ONE_HOUR_MS/COUNT_OF_PERIOD;
			syringe1.dose = (DOSE_O_1ML*syringe1.speed)/COUNT_OF_PERIOD;
			syringe1.motor.setPWM(100);
#ifdef VER1
			syringe1.motor.setRotation(CLOCKWISE); // PUSH
#endif
#ifdef VER2
			syringe1.motor.setRotation(UNCLOCKWISE); // PUSH
#endif
			syringe1.start();
		}
		syringe1.timer--;
	}
	/*==========================================================================================*/
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
			syringe1.cover.state = OPEN;
			syringe1.speed = 0;
		}
	}
	else if (syringe1.cover.hallSensor.status  == RELEASED) {
		syringe1.cover.hallSensor.timer_2++;
		if (syringe1.cover.hallSensor.timer_2 >= syringe1.cover.hallSensor.delay) {
			syringe1.cover.hallSensor.timer_2 = 0;
			syringe1.cover.state = CLOSE;
		}
	}
	/*==========================================================================================*/
	// Limit switch start
#ifdef VER1
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET) {
		syringe1.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET && syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.status = RELEASED;
	}
	if (syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.timer_1++;
		if (syringe1.startSensor.timer_1 == syringe1.startSensor.delay) {
			syringe1.volume = MAX_VOLUME;
			syringe1.encoder.cnt = MAX_ENCODER_CNT;
			if (syringe1.motor.rotation == UNCLOCKWISE) { // PULL
				syringe1.stop();
			}
		}
	}
	else if (syringe1.startSensor.status == RELEASED) {
		syringe1.startSensor.timer_2++;
		if (syringe1.startSensor.timer_2 == syringe1.startSensor.delay) {
			syringe1.startSensor.status = NO_EVENT;
			syringe1.startSensor.timer_1 = 0;
			syringe1.startSensor.timer_2 = 0;
		}
	}
#endif
#ifdef VER2
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_SET) {
		syringe1.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_RESET && syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.status = RELEASED;
	}
	if (syringe1.startSensor.status == PRESSED) {
		syringe1.startSensor.timer_1++;
		if (syringe1.startSensor.timer_1 == syringe1.startSensor.delay) {
			syringe1.volume = MAX_VOLUME;
			syringe1.encoder.cnt = MAX_ENCODER_CNT;
			if (syringe1.motor.rotation == CLOCKWISE) { // PULL
				syringe1.stop();
			}
		}
	}
	else if (syringe1.startSensor.status == RELEASED) {
		syringe1.startSensor.timer_2++;
		if (syringe1.startSensor.timer_2 == syringe1.startSensor.delay) {
			syringe1.startSensor.status = NO_EVENT;
			syringe1.startSensor.timer_1 = 0;
			syringe1.startSensor.timer_2 = 0;
		}
	}
#endif
	/*==========================================================================================*/
	// Limit switch end
#ifdef VER1
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_SET) {
		syringe1.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_RESET && syringe1.endSensor.status == PRESSED) {
		syringe1.endSensor.status = RELEASED;
	}
	if (syringe1.endSensor.status  == PRESSED) {
		syringe1.endSensor.timer_1++;
		if (syringe1.endSensor.timer_1 == syringe1.endSensor.delay) {
			syringe1.volume = 0;
			syringe1.encoder.cnt = 0;
			if (syringe1.motor.rotation == CLOCKWISE) { // PUSH
				syringe1.stop();
				syringe1.speed = 0;
			}
		}
	}
	else if (syringe1.endSensor.status == RELEASED) {
		syringe1.endSensor.timer_2++;
		if (syringe1.endSensor.timer_2 == syringe1.endSensor.delay) {
			syringe1.endSensor.status = NO_EVENT;
			syringe1.endSensor.timer_1 = 0;
			syringe1.endSensor.timer_2 = 0;
		}
	}
#endif
#ifdef VER2
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET) {
		syringe1.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET && syringe1.endSensor.status == PRESSED) {
		syringe1.endSensor.status = RELEASED;
	}
	if (syringe1.endSensor.status  == PRESSED) {
		syringe1.endSensor.timer_1++;
		if (syringe1.endSensor.timer_1 == syringe1.endSensor.delay) {
			syringe1.volume = 0;
			syringe1.encoder.cnt = 0;
			if (syringe1.motor.rotation == UNCLOCKWISE) { // PUSH
				syringe1.stop();
				syringe1.speed = 0;
			}
		}
	}
	else if (syringe1.endSensor.status == RELEASED) {
		syringe1.endSensor.timer_2++;
		if (syringe1.endSensor.timer_2 == syringe1.endSensor.delay) {
			syringe1.endSensor.status = NO_EVENT;
			syringe1.endSensor.timer_1 = 0;
			syringe1.endSensor.timer_2 = 0;
		}
	}
#endif
	/*==========================================================================================*/
	// Button UP
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_SET) {
		syringe1.buttonUp.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET && syringe1.buttonUp.status == PRESSED) {
		syringe1.buttonUp.status = RELEASED;
	}
	if (syringe1.buttonUp.status == PRESSED) {
		syringe1.buttonUp.timer_1++;
		if (syringe1.buttonUp.timer_1 == syringe1.buttonUp.delayLongClick) {
			syringe1.buttonUp.mode = LONG_CLICK;
		}
	}
	else if (syringe1.buttonUp.status == RELEASED) {
		if (syringe1.buttonUp.timer_1 >= syringe1.buttonUp.delayLongClick) {
			syringe1.buttonUp.timer_1 = 0;
			syringe1.buttonUp.mode = DEFAULT;
			syringe1.buttonUp.status = NO_EVENT;
			syringe1.encoder.cnt = syringe1.encoder.cnt + TIM2->CNT;
			TIM2->CNT = 0;
			syringe1.stop();
		}
		else {
			syringe1.buttonUp.timer_2++;
			if (syringe1.buttonUp.timer_2 >= syringe1.buttonUp.delayClick && syringe1.buttonUp.timer_2 < syringe1.buttonUp.delayLongClick) {
				syringe1.buttonUp.mode = CLICK;
				syringe1.buttonUp.status = NO_EVENT;
				syringe1.buttonUp.timer_1 = 0;
				syringe1.buttonUp.timer_2 = 0;
			}
		}
	}
	/*==========================================================================================*/
	// Button Down
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET) {
		syringe1.buttonDown.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET && syringe1.buttonDown.status == PRESSED) {
		syringe1.buttonDown.status = RELEASED;
	}
	if (syringe1.buttonDown.status == PRESSED) {
		syringe1.buttonDown.timer_1++;
		if (syringe1.buttonDown.timer_1 == syringe1.buttonDown.delayLongClick) {
			syringe1.buttonDown.mode = LONG_CLICK;
		}
	}
	else if (syringe1.buttonDown.status == RELEASED) {
		if (syringe1.buttonDown.timer_1 >= syringe1.buttonDown.delayLongClick) {
			syringe1.buttonDown.timer_1 = 0;
			syringe1.buttonDown.mode = DEFAULT;
			syringe1.buttonDown.status = NO_EVENT;
			syringe1.encoder.cnt = syringe1.encoder.cnt - TIM2->CNT;
			TIM2->CNT = 0;
			syringe1.stop();
		}
		else {
			syringe1.buttonDown.timer_2++;
			if (syringe1.buttonDown.timer_2 >= syringe1.buttonDown.delayClick && syringe1.buttonDown.timer_2 < syringe1.buttonDown.delayLongClick) {
				syringe1.buttonDown.mode = CLICK;
				syringe1.buttonDown.status = NO_EVENT;
				syringe1.buttonDown.timer_1 = 0;
				syringe1.buttonDown.timer_2 = 0;
			}
		}
	}
	/*==========================================================================================*/
}

void	syringe2Start() {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = syringe2.motor.pwm;;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	syringe2.en = true;
}

void	syringe2Stop() {
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	syringe2.en = false;
}

void 	syringe2Timer() {
	/*==========================================================================================*/
	if (syringe2.en == true) {
		if (syringe2.dose != 0) {
			if (TIM3->CNT >= syringe2.dose) {
				syringe2.stop();
				syringe2.dose = 0;
#ifdef VER1
				if (syringe2.motor.rotation == UNCLOCKWISE) { // PULL
					syringe2.encoder.cnt = syringe2.encoder.cnt + TIM3->CNT;
				}
				else if (syringe1.motor.rotation == CLOCKWISE) { // PUSH
					syringe2.encoder.cnt = syringe2.encoder.cnt - TIM3->CNT;
				}
#endif
#ifdef VER2
				if (syringe2.motor.rotation == UNCLOCKWISE) { // PUSH
					syringe2.encoder.cnt = syringe2.encoder.cnt - TIM3->CNT;
				}
				else if (syringe1.motor.rotation == CLOCKWISE) { // PULL
					syringe2.encoder.cnt = syringe2.encoder.cnt + TIM3->CNT;
				}
#endif
				TIM3->CNT = 0;
			}
		}
		else {
#ifdef VER1
			if (syringe2.motor.rotation == UNCLOCKWISE) { // PULL
				syringe2.encoder.cnt = syringe2.encoder.cnt + TIM3->CNT;
			}
			else if (syringe2.motor.rotation == CLOCKWISE) { // PUSH
				syringe2.encoder.cnt = syringe2.encoder.cnt - TIM3->CNT;
			}
#endif
#ifdef VER2
			if (syringe2.motor.rotation == UNCLOCKWISE) { // PUSH
				syringe2.encoder.cnt = syringe2.encoder.cnt - TIM3->CNT;
			}
			else if (syringe2.motor.rotation == CLOCKWISE) { // PULL
				syringe2.encoder.cnt = syringe2.encoder.cnt + TIM3->CNT;
			}
#endif
			TIM3->CNT = 0;
		}
	}
	/*==========================================================================================*/
	if (syringe2.speed != 0 && syringe2.cover.state == CLOSE && syringe2.endSensor.status != PRESSED) {
		if (syringe2.timer == 0) {
			syringe2.timer = ONE_HOUR_MS/COUNT_OF_PERIOD;
			syringe2.dose = (DOSE_O_1ML*syringe2.speed)/COUNT_OF_PERIOD;
			syringe2.motor.setPWM(100);
#ifdef VER1
			syringe2.motor.setRotation(CLOCKWISE);
#endif
#ifdef VER2
			syringe2.motor.setRotation(UNCLOCKWISE);
#endif
			syringe2.start();
		}
		syringe2.timer--;
	}
	/*==========================================================================================*/
	// Hall sensor
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_SET) {
		syringe2.cover.hallSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_RESET && syringe2.cover.hallSensor.status == PRESSED) {
		syringe2.cover.hallSensor.status = RELEASED;
	}
	if (syringe2.cover.hallSensor.status  == PRESSED) {
		syringe2.cover.hallSensor.timer_1++;
		if (syringe2.cover.hallSensor.timer_1 >= syringe2.cover.hallSensor.delay) {
			syringe2.cover.hallSensor.timer_1 = 0;
			syringe2.cover.state = OPEN;
			syringe2.speed = 0;
		}
	}
	else if (syringe2.cover.hallSensor.status  == RELEASED) {
		syringe2.cover.hallSensor.timer_2++;
		if (syringe2.cover.hallSensor.timer_2 >= syringe2.cover.hallSensor.delay) {
			syringe2.cover.hallSensor.timer_2 = 0;
			syringe2.cover.state = CLOSE;
		}
	}
	/*==========================================================================================*/
	// Limit switch start
#ifdef VER1
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET) {
		syringe2.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET && syringe2.startSensor.status == PRESSED) {
		syringe2.startSensor.status = RELEASED;
	}
	if (syringe2.startSensor.status == PRESSED) {
		syringe2.startSensor.timer_1++;
		if (syringe2.startSensor.timer_1 == syringe2.startSensor.delay) {
			syringe2.volume = MAX_VOLUME;
			syringe2.encoder.cnt = MAX_ENCODER_CNT;
			if (syringe2.motor.rotation == UNCLOCKWISE) { // PULL
				syringe2.stop();
			}
		}
	}
	else if (syringe2.startSensor.status == RELEASED) {
		syringe2.startSensor.timer_2++;
		if (syringe2.startSensor.timer_2 == syringe2.startSensor.delay) {
			syringe2.startSensor.status = NO_EVENT;
			syringe2.startSensor.timer_1 = 0;
			syringe2.startSensor.timer_2 = 0;
		}
	}
#endif
#ifdef VER2
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_SET) {
		syringe2.startSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET && syringe2.startSensor.status == PRESSED) {
		syringe2.startSensor.status = RELEASED;
	}
	if (syringe2.startSensor.status == PRESSED) {
		syringe2.startSensor.timer_1++;
		if (syringe2.startSensor.timer_1 == syringe2.startSensor.delay) {
			syringe2.volume = MAX_VOLUME;
			syringe2.encoder.cnt = MAX_ENCODER_CNT;
			if (syringe2.motor.rotation == CLOCKWISE) { // PULL
				syringe2.stop();
			}
		}
	}
	else if (syringe2.startSensor.status == RELEASED) {
		syringe2.startSensor.timer_2++;
		if (syringe2.startSensor.timer_2 == syringe2.startSensor.delay) {
			syringe2.startSensor.status = NO_EVENT;
			syringe2.startSensor.timer_1 = 0;
			syringe2.startSensor.timer_2 = 0;
		}
	}
#endif

	/*==========================================================================================*/
	// Limit switch end
#ifdef VER1
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_SET) {
		syringe2.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET && syringe2.endSensor.status == PRESSED) {
		syringe2.endSensor.status = RELEASED;
	}
	if (syringe2.endSensor.status  == PRESSED) {
		syringe2.endSensor.timer_1++;
		if (syringe2.endSensor.timer_1 == syringe2.endSensor.delay) {
			syringe2.volume = 0;
			syringe2.encoder.cnt = 0;
			if (syringe2.motor.rotation == CLOCKWISE) { // PUSH
				syringe2.stop();
				syringe2.speed = 0;
			}
		}
	}
	else if (syringe2.endSensor.status == RELEASED) {
		syringe2.endSensor.timer_2++;
		if (syringe2.endSensor.timer_2 == syringe2.endSensor.delay) {
			syringe2.endSensor.status = NO_EVENT;
			syringe2.endSensor.timer_1 = 0;
			syringe2.endSensor.timer_2 = 0;
		}
	}
#endif
#ifdef VER2
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET) {
		syringe2.endSensor.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET && syringe2.endSensor.status == PRESSED) {
		syringe2.endSensor.status = RELEASED;
	}
	if (syringe2.endSensor.status  == PRESSED) {
		syringe2.endSensor.timer_1++;
		if (syringe2.endSensor.timer_1 == syringe2.endSensor.delay) {
			syringe2.volume = 0;
			syringe2.encoder.cnt = 0;
			if (syringe2.motor.rotation == UNCLOCKWISE) { // PUSH
				syringe2.stop();
				syringe2.speed = 0;
			}
		}
	}
	else if (syringe2.endSensor.status == RELEASED) {
		syringe2.endSensor.timer_2++;
		if (syringe2.endSensor.timer_2 == syringe2.endSensor.delay) {
			syringe2.endSensor.status = NO_EVENT;
			syringe2.endSensor.timer_1 = 0;
			syringe2.endSensor.timer_2 = 0;
		}
	}
#endif
	/*==========================================================================================*/
	// Button UP
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_SET) {
		syringe2.buttonUp.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_RESET && syringe2.buttonUp.status == PRESSED) {
		syringe2.buttonUp.status = RELEASED;
	}
	if (syringe2.buttonUp.status == PRESSED) {
		syringe2.buttonUp.timer_1++;
		if (syringe2.buttonUp.timer_1 == syringe2.buttonUp.delayLongClick) {
			syringe2.buttonUp.mode = LONG_CLICK;
		}
	}
	else if (syringe2.buttonUp.status == RELEASED) {
		if (syringe2.buttonUp.timer_1 >= syringe2.buttonUp.delayLongClick) {
			syringe2.buttonUp.timer_1 = 0;
			syringe2.buttonUp.mode = DEFAULT;
			syringe2.buttonUp.status = NO_EVENT;
			syringe2.encoder.cnt = syringe2.encoder.cnt + TIM3->CNT;
			TIM3->CNT = 0;
			syringe2.stop();
		}
		else {
			syringe2.buttonUp.timer_2++;
			if (syringe2.buttonUp.timer_2 >= syringe2.buttonUp.delayClick && syringe2.buttonUp.timer_2 < syringe2.buttonUp.delayLongClick) {
				syringe2.buttonUp.mode = CLICK;
				syringe2.buttonUp.status = NO_EVENT;
				syringe2.buttonUp.timer_1 = 0;
				syringe2.buttonUp.timer_2 = 0;
			}
		}
	}
	/*==========================================================================================*/
	// Button Down
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET) {
		syringe2.buttonDown.status = PRESSED;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_RESET && syringe2.buttonDown.status == PRESSED) {
		syringe2.buttonDown.status = RELEASED;
	}
	if (syringe2.buttonDown.status == PRESSED) {
		syringe2.buttonDown.timer_1++;
		if (syringe2.buttonDown.timer_1 == syringe2.buttonDown.delayLongClick) {
			syringe2.buttonDown.mode = LONG_CLICK;
		}
	}
	else if (syringe2.buttonDown.status == RELEASED) {
		if (syringe2.buttonDown.timer_1 >= syringe2.buttonDown.delayLongClick) {
			syringe2.buttonDown.timer_1 = 0;
			syringe2.buttonDown.mode = DEFAULT;
			syringe2.buttonDown.status = NO_EVENT;
			syringe2.encoder.cnt = syringe2.encoder.cnt - TIM3->CNT;
			TIM3->CNT = 0;
			syringe2.stop();
		}
		else {
			syringe2.buttonDown.timer_2++;
			if (syringe2.buttonDown.timer_2 >= syringe2.buttonDown.delayClick && syringe2.buttonDown.timer_2 < syringe2.buttonDown.delayLongClick) {
				syringe2.buttonDown.mode = CLICK;
				syringe2.buttonDown.status = NO_EVENT;
				syringe2.buttonDown.timer_1 = 0;
				syringe2.buttonDown.timer_2 = 0;
			}
		}
	}
	/*==========================================================================================*/
}

void	 syringe1Handler() {

	syringe1.volume = syringe1.encoder.cnt / DOSE_O_1ML;

	// LONG CLICK HANDLER
	if (syringe1.buttonUp.mode == LONG_CLICK) {
		syringe1.buttonUp.mode = DEFAULT;
		if (syringe1.startSensor.status != PRESSED && syringe1.cover.state == OPEN) {
#ifdef VER1
			syringe1.motor.setRotation(UNCLOCKWISE); // PULL
#endif
#ifdef VER2
			syringe1.motor.setRotation(CLOCKWISE); // PULL
#endif
			syringe1.motor.setPWM(100);
			syringe1.start();
		}
	}
	if (syringe1.cover.state == CLOSE && syringe1.buttonUp.mode == LONG_CLICK) {
		syringe1.buttonUp.mode = DEFAULT;
		syringe1.stop();
	}
	if (syringe1.buttonDown.mode == LONG_CLICK) {
		syringe1.buttonDown.mode = DEFAULT;
		if (syringe1.endSensor.status != PRESSED && syringe1.cover.state == OPEN) {
#ifdef VER1
			syringe1.motor.setRotation(CLOCKWISE); // PUSH
#endif
#ifdef VER2
			syringe1.motor.setRotation(UNCLOCKWISE); // PUSH
#endif
			syringe1.motor.setPWM(100);
			syringe1.start();
		}
	}
	if (syringe1.cover.state == CLOSE && syringe1.buttonDown.mode == LONG_CLICK) {
		syringe1.buttonDown.mode = DEFAULT;
		syringe1.stop();
	}

	// CLICK HANDLER
	if (syringe1.buttonUp.mode == CLICK) {
		syringe1.buttonUp.mode = DEFAULT;
		if (syringe1.startSensor.status != PRESSED && syringe1.cover.state == OPEN) {
			syringe1.dose = DOSE_O_1ML;
			TIM2->CNT = 0;
#ifdef VER1
			syringe1.motor.setRotation(UNCLOCKWISE); // PULL
#endif
#ifdef VER2
			syringe1.motor.setRotation(CLOCKWISE); // PULL
#endif
			syringe1.motor.setPWM(100);
			syringe1.start();
		}
	}
	if (syringe1.buttonDown.mode == CLICK) {
		syringe1.buttonDown.mode = DEFAULT;
		if (syringe1.endSensor.status != PRESSED && syringe1.cover.state == OPEN) {
			syringe1.dose = DOSE_O_1ML;
			TIM2->CNT = 0;
#ifdef VER1
			syringe1.motor.setRotation(CLOCKWISE); // PUSH
#endif
#ifdef VER2
			syringe1.motor.setRotation(UNCLOCKWISE); // PUSH
#endif
			syringe1.motor.setPWM(100);
			syringe1.start();
		}
	}
}

void	 syringe2Handler() {

	syringe2.volume = syringe2.encoder.cnt / DOSE_O_1ML;

	// LONG CLICK HANDLER
	if (syringe2.buttonUp.mode == LONG_CLICK) {
		syringe2.buttonUp.mode = DEFAULT;
		if (syringe2.startSensor.status != PRESSED && syringe2.cover.state == OPEN) {
#ifdef VER1
			syringe2.motor.setRotation(UNCLOCKWISE);
#endif
#ifdef VER2
			syringe2.motor.setRotation(CLOCKWISE);
#endif
			syringe2.motor.setPWM(100);
			syringe2.start();
		}
	}
	if (syringe2.cover.state == CLOSE && syringe2.buttonUp.mode == LONG_CLICK) {
		syringe2.buttonUp.mode = DEFAULT;
		syringe2.stop();
	}
	if (syringe2.buttonDown.mode == LONG_CLICK) {
		syringe2.buttonDown.mode = DEFAULT;
		if (syringe2.endSensor.status != PRESSED && syringe2.cover.state == OPEN) {
#ifdef VER1
			syringe2.motor.setRotation(CLOCKWISE);
#endif
#ifdef VER2
			syringe2.motor.setRotation(UNCLOCKWISE);
#endif
			syringe2.motor.setPWM(100);
			syringe2.start();
		}
	}
	if (syringe2.cover.state == CLOSE && syringe2.buttonDown.mode == LONG_CLICK) {
		syringe2.buttonDown.mode = DEFAULT;
		syringe2.stop();
	}

	// CLICK HANDLER
	if (syringe2.buttonUp.mode == CLICK) {
		syringe2.buttonUp.mode = DEFAULT;
		if (syringe2.startSensor.status != PRESSED && syringe2.cover.state == OPEN) {
			syringe2.dose = DOSE_O_1ML;
			TIM3->CNT = 0;
#ifdef VER1
			syringe2.motor.setRotation(UNCLOCKWISE); // PULL
#endif
#ifdef VER2
			syringe2.motor.setRotation(CLOCKWISE); // PULL
#endif
			syringe2.motor.setPWM(100);
			syringe2.start();
		}
	}
	if (syringe2.buttonDown.mode == CLICK) {
		syringe2.buttonDown.mode = DEFAULT;
		if (syringe2.endSensor.status != PRESSED && syringe2.cover.state == OPEN) {
			syringe2.dose = DOSE_O_1ML;
			TIM3->CNT = 0;
#ifdef VER1
			syringe2.motor.setRotation(CLOCKWISE); // PUSH
#endif
#ifdef VER2
			syringe2.motor.setRotation(UNCLOCKWISE); // PUSH
#endif
			syringe2.motor.setPWM(100);
			syringe2.start();

		}
	}
}
