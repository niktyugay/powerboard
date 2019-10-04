/*
 * syringe.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: tugay
 */
#ifndef SYRINGE_H_
#define SYRINGE_H_
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stdbool.h"
#define DOSE_O_1ML			1000
#define PERIOD_ENC			50
#define MAX_VOLUME			20000
typedef enum 	{
	STOP,
	PUSH,
	PULL,
} 	Direction;
typedef enum	{
	BREAK,
	CLOCKWISE,
	UNCLOCKWISE
}	Rotation;
typedef enum	{
	START,
	END,
	MIDDLE
} 	Position;
typedef enum	{
	CLOSE,
	OPEN
}	CoverState;
typedef enum	{
	DEFAULT,
	CLICK,
	LONG_CLICK,
	DOUBLE_CLICK
}	ButtonsMode;
typedef enum	{
	NO_EVENT,
	PRESSED,
	RELEASED
}	ButtonStatus;
typedef struct	{
	uint32_t				timer_1;
	uint32_t				timer_2;
	ButtonStatus			status;
	ButtonsMode				mode;
	uint16_t				delayClick;
	uint16_t				delayLongClick;
}	Buttons;
typedef struct	{
	uint32_t				timer_1;
	uint32_t				timer_2;
	ButtonStatus			status;
	uint16_t				delay;
}	Sensors;
typedef struct	{
	uint8_t					pwm;
	Rotation				rotation;
	void					(*setPWM)(uint8_t);
	uint16_t				(*getPWM)(void);
	void					(*setRotation)(Rotation);
} Motor;
typedef struct	{
	uint8_t					timer;
	uint8_t					PERIOD;
	uint32_t				cnt;
	uint16_t				timOld;
	uint16_t				timCurrent;
	uint16_t				(*getEncoder)(void);
} Encoder;
typedef struct	{
	Sensors					hallSensor;
	CoverState				state;
	CoverState				(*getState)(void);
} Cover;
typedef struct	{
	uint32_t				timer;
	uint32_t 				PERIOD;
	uint16_t				speed;
	uint16_t				dose;
	void					(*setSpeed)(uint16_t);
	uint16_t				(*getSpeed)(void);
} Speed;
typedef struct	{
	bool 					en;
	uint16_t				volume;
	uint16_t				dose;
	Direction				direction;
	Position				position;
} Syringe_varibles;
typedef struct	{
	void					(*en)(bool);
	void					(*handler)(void);
	void					(*timer)(void);
	Syringe_varibles 		param;
	Motor					motor;
	Encoder					encoder;
	Cover					cover;
	Speed					speed;
	void					(*setDose)(uint16_t);
	Sensors					startSensor, endSensors;
	void					(*setVolume)(uint16_t);
	uint16_t				(*getVolume)(void);
	void					(*setPosition)(Position);
	Position				(*getPosition)(void);
	void					(*setDirection)(void);
	Direction				(*getDirection)(void);
} Syringe;
void 						initHardware(void);
void 						initSyringes(void);

void						syringe1en(bool state);
void						syringe2en(bool state);

void						syringe1Handler(void);
void						syringe2Handler(void);

void						syringe1Timer(void);
void						syringe2Timer(void);

void 						setPWMSyringe1(uint8_t dutyCycle);
uint16_t					getPWMSyringe1(void);
void 						setPWMSyringe2(uint8_t dutyCycle);
uint16_t					getPWMSyringe2(void);

uint16_t					getEncoderSyringe1(void);
uint16_t					getEncoderSyringe2(void);

void 						setSpeedSyringe1(uint16_t volume);
uint16_t					getSpeedSyringe1(void);
void 						setSpeedSyringe2(uint16_t volume);
uint16_t					getSpeedSyringe2(void);

void 						setDoseSyringe1(uint16_t value);
void 						setDoseSyringe2(uint16_t value);

uint16_t					getVolumeSyringe1(void);
uint16_t					getVolumeSyringe2(void);

void						setDirectionSyringe1(Direction direction);
Direction					getDirectionSyringe1(void);
void						setDirectionSyringe2(Direction direction);
Direction					getDirectionSyringe2(void);

void						setPositionSyringe1(Position position);
Position					getPositionSyringe1(void);
void						setPositionSyringe2(Position position);
Position					getPositionSyringe2(void);

CoverState					getStateCoverSyringe1(void);
CoverState					getStateCoverSyringe2(void);
#endif /* SYRINGE_H_ */
