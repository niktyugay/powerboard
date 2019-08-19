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
	bool en;
	uint8_t singleDose;
	uint8_t speed;
} syringe;

void initHardware(void);

#endif /* SYRINGE_H_ */
