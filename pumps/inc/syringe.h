/*
 * syringe.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */

#ifndef SYRINGE_H_
#define SYRINGE_H_

#include "stm32f10x.h"
#include "stdbool.h"

typedef struct {
	bool 					en;
	uint8_t					pwm;
	uint16_t 				dose;
	uint16_t 				speed;
	uint32_t 				period;
	uint16_t				volume;
	uint32_t				timerForSpeed;
} syringe_varibles;

typedef struct {
	syringe_varibles 		param;

	void					(*setSpeed)(uint16_t);
	uint16_t				(*getSpeed)(void);
	void					(*setDose)(uint16_t);
	void					(*en)(bool);
	void					(*backToStartPosition)(void);
	void					(*setVolume)(uint16_t);
	uint16_t				(*getVolume)(void);
	void					(*handler)(void);

	void					(*setPWM)(uint8_t);
	uint8_t					(*getPWM)(void);
	void					(*setEncCnt)(uint16_t);
	uint16_t				(*getEncCnt)(void);
} syringe;

void 						initHardware(void);
void 						initSyringes(void);

void 						setPWMSyringe1(uint8_t dutyCycle);
uint8_t						getPWMSyringe1(void);
void 						setPWMSyringe2(uint8_t dutyCycle);
uint8_t						getPWMSyringe2(void);
void						setEncCntSyringe1(uint16_t value);
uint16_t					getEncCntSyringe1(void);
void						setEncCntSyringe2(uint16_t value);
uint16_t					getEncCntSyringe2(void);

void 						setSpeedSyringe1(uint16_t volume);
uint16_t					getSpeedSyringe1(void);
void 						setSpeedSyringe2(uint16_t volume);
uint16_t					getSpeedSyringe2(void);
void 						setDoseSyringe1(uint16_t volume);
void 						setDoseSyringe2(uint16_t volume);
void						setVolumeSyringe1(uint16_t value);
uint16_t					getVolumeSyringe1(void);
void						setVolumeSyringe2(uint16_t value);
uint16_t					getVolumeSyringe2(void);

void						syringe1Handler();
void						syringe2Handler();

void						syringe1en(bool state);
void						syringe2en(bool state);
void						backToStartPositionSyringe1(void);
void						backToStartPositionSyringe2(void);

#endif /* SYRINGE_H_ */
