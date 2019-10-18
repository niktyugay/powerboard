/*
 * spi.h
 *
 *  Created on: 16 ���. 2019 �.
 *      Author: tugay
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x.h"

// HEAD
#define CMD_SET								0x80

// GENERAL cmd
#define SELFTEST							0x14

// CMD for peristaltic pumps
#define M1_SPEED1							0x01
#define M2_SPEED2							0x02
#define M1_EN								0x03
#define M2_EN								0x04
#define M1_DIRECTION						0x05
#define M2_DIRECTION						0x06
#define M1_ENC								0x07
#define M2_ENC								0x08

// CMD for valves
#define VALVE1								0x09
#define VALVE2								0x0A

//	CMD for syringes
#define DOS1_EN								0x0B
#define DOS2_EN								0x0C
#define DOS1_SINGLE_DOSE					0x0D	// set none
#define DOS2_SINGLE_DOSE					0x0E	// set none
#define DOS1_SPEED							0x0F
#define DOS2_SPEED							0x10
#define DOS1_VOLUME     					0x11    // set none
#define DOS2_VOLUME     					0x12    // set none
#define DOS_COVERS							0x13

void SPI_Config(void);

#endif /* SPI_H_ */
