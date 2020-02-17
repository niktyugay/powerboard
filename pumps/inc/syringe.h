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
#define DOSE_O_1ML			3697
#define PERIOD_ENC			50
#define MAX_VOLUME			214
#define	MAX_ENCODER_CNT		794500
#define	ONE_HOUR_MS			3600000
#define COUNT_OF_PERIOD		5
#define	DELAY_CLICK			175
#define	DELAY_LONGCLICK		2000
#define	DELAY_SENSOR_CLICK	100

typedef enum 	{
	STOP,
	PUSH,
	PULL,
} 	Direction;
typedef enum	{
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
	uint16_t				pwm;
	Rotation				rotation;
	void					(*setPWM)(uint8_t);
	uint8_t					(*getPWM)(void);
	void					(*setRotation)(Rotation);
} Motor;
typedef struct	{
	uint32_t				cnt;
} Encoder;
typedef struct	{
	Sensors					hallSensor;
	CoverState				state;
} Cover;
typedef struct	{
	bool 					en;
	uint8_t					volume;
	uint16_t				dose;
	Direction				direction;
	Position				position;
} Syringe_varibles;
typedef struct	{
	bool 					en;
	uint8_t					volume;
	uint16_t				dose;
	uint8_t					speed;

	uint32_t				timer;

	Motor					motor;
	Encoder					encoder;
	Cover					cover;
	Sensors					startSensor, endSensor;
	Buttons					buttonUp, buttonDown;

	void					(*handlerMain)(void);
	void					(*handlerTimer)(void);

	void					(*start)(void);
	void					(*stop)(void);
	void					(*setDose)(uint16_t);
	uint8_t					(*getVolume)(void);
} Syringe;

void 						initHardware(void);
void 						initSyringes(void);

void						syringe1Handler(void);
void						syringe2Handler(void);
void						syringe1Timer(void);
void						syringe2Timer(void);

void						syringe1Start(void);
void						syringe1Stop(void);
void 						setDoseSyringe1(uint16_t value);

void						syringe2Start(void);
void						syringe2Stop(void);
void 						setDoseSyringe2(uint16_t value);

void						buttonUpSyringe1(ButtonsMode);
void						buttonDownSyringe1(ButtonsMode);
void						buttonUpSyringe2(ButtonsMode);
void						buttonDownSyringe2(ButtonsMode);

void 						setPWMSyringe1(uint8_t dutyCycle);
uint8_t						getPWMSyringe1(void);
void 						setPWMSyringe2(uint8_t dutyCycle);
uint8_t						getPWMSyringe2(void);
void						setRotationSyringe1(Rotation);
void						setRotationSyringe2(Rotation);

#endif /* SYRINGE_H_ */
