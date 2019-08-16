/*
 * spi.c
 *
 *  Created on: 16 рту. 2019 у.
 *      Author: tugay
 */

#include "spi.h"

uint8_t aRxBuffer[10];
uint8_t ubRxIndex = 0;

void SPI_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(SPI1_IRQn);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_IRQHandler() {
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
		aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPI1);
		switch(aRxBuffer[0]) {
			case SPEED1:
				break;
			case SPEED1 | CMD_MASK:
				break;
			case SPEED2:
				break;
			case SPEED2 | CMD_MASK:
				break;
			case START_STOP_M1 | CMD_MASK:
				break;
			case START_STOP_M2 | CMD_MASK:
				break;
			case ENC_1:
				break;
			case ENC_2:
				break;
			case VALVE1:
				break;
			case VALVE1 | CMD_MASK:
				break;
			case MOTOR1_DIRECTION:
				break;
			case MOTOR1_DIRECTION | CMD_MASK:
				break;
			case MOTOR2_DIRECTION:
				break;
			case MOTOR2_DIRECTION | CMD_MASK:
				break;
			case VALVE2:
				break;
			case VALVE2 | CMD_MASK:
				break;
			case DOS_NOSTAT1:
				break;
			case DOS_START1:
				break;
			case DOS_STOP1:
				break;
			case DOS_PAUSE1:
				break;
			case DOS_PROCEED1:
				break;
			case DOS_NOCOMM1:
				break;
			case DOS_VOLUME1:
				break;
			case DOS_SPEED1:
				break;
			case DOS_GOTOPOS1:
				break;
			case DOS_CHANGE_COEF_DOSE_0_1ML1:
				break;
			case DOS_NOSTAT2:
				break;
			case DOS_START2:
				break;
			case DOS_STOP2:
				break;
			case DOS_PAUSE2:
				break;
			case DOS_PROCEED2:
				break;
			case DOS_NOCOMM2:
				break;
			case DOS_VOLUME2:
				break;
			case DOS_SPEED2:
				break;
			case DOS_GOTOPOS2:
				break;
			case DOS_CHANGE_COEF_DOSE_0_1ML2:
				break;
		}
	}
}
