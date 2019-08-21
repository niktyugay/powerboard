/*
 * syringe.h
 *
 *  Created on: 19 рту. 2019 у.
 *      Author: tugay
 */

#ifndef SYRINGE_H_
#define SYRINGE_H_

#include "stm32f10x.h"
#include "stdbool.h"

typedef struct {
	bool 					en;
	uint8_t					pwm;
	uint8_t 				dose;
	uint8_t 				speed;
	uint32_t 				period;
} syringe_varibles;

typedef struct {
	syringe_varibles 		param;
	void					(*setPWM)(uint8_t);
	uint8_t					(*getPWM)(void);
	void					(*setSpeed)(uint8_t);
	uint8_t					(*getSpeed)(void);
	void					(*setDose)(uint8_t);
	void					(*en)(bool);
} syringe;

void 						initHardware(void);
void 						initSyringes(void);

void 						setPWMSyringe1(uint8_t dutyCycle);
uint8_t						getPWMSyringe1(void);
void 						setPWMSyringe2(uint8_t dutyCycle);
uint8_t						getPWMSyringe2(void);
void 						setSpeedSyringe1(uint8_t volume);
uint8_t						getSpeedSyringe1(void);
void 						setSpeedSyringe2(uint8_t volume);
uint8_t						getSpeedSyringe2(void);
void 						setDoseSyringe1(uint8_t volume);
void 						setDoseSyringe2(uint8_t volume);
void						syringe1en(bool state);
void						syringe2en(bool state);

#endif /* SYRINGE_H_ */
