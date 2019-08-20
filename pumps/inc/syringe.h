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
	bool 		en;
	uint8_t 	singleDose;
	uint8_t 	speed;
	uint8_t		pwm;
	void		(*setPWM)(uint8_t);
} syringe;

void initHardware(void);
void initSyringes(void);
void setPWMSyringe1(uint8_t dutyCycle);
void setPWMSyringe2(uint8_t dutyCycle);
void setSpeedSyringe1(uint8_t volume);
void setSpeedSyringe2(uint8_t volume);
void setVolumeSyringe1(uint8_t volume);
void setVolumeSyringe2(uint8_t volume);

#endif /* SYRINGE_H_ */
