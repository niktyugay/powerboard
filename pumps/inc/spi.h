/*
 * spi.h
 *
 *  Created on: 16 рту. 2019 у.
 *      Author: tugay
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x.h"

// PROTOCOL
#define CMD_MASK							0x80
// cmd first byte
#define SPEED1 								0x01
#define SPEED2 								0x02
#define START_STOP_M1						0x03
#define START_STOP_M2						0x04
#define MOTOR1_DIRECTION					0x05
#define MOTOR2_DIRECTION					0x06
#define ENC_1								0x07
#define ENC_2								0x08
// VALVES
#define VALVE1								0x09
#define VALVE2								0x0A

//	STATUS
#define DOS_NOSTAT1      					0x0B
#define DOS_START1	     					0x0C
#define DOS_STOP1        					0x0D
#define DOS_PAUSE1       					0x0E
#define DOS_PROCEED1     					0x0F

// COMMANDS
#define DOS_NOCOMM1      					0x10
#define DOS_VOLUME1      					0x11
#define DOS_SPEED1       					0x12
#define DOS_GOTOPOS1     					0x13
#define DOS_CHANGE_COEF_DOSE_0_1ML1			0x14

//	STATUS
#define DOS_NOSTAT2      					0x15
#define DOS_START2	     					0x16
#define DOS_STOP2        					0x17
#define DOS_PAUSE2       					0x18
#define DOS_PROCEED2     					0x19

// COMMANDS
#define DOS_NOCOMM2      					0x1A
#define DOS_VOLUME2      					0x1B
#define DOS_SPEED2       					0x1C
#define DOS_GOTOPOS2     					0x1D
#define DOS_CHANGE_COEF_DOSE_0_1ML2			0x1E

void SPI_Config(void);

#endif /* SPI_H_ */
