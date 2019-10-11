/*
 * spi.c
 *
 *  Created on: 16 ���. 2019 �.
 *      Author: tugay
 */

#include "spi.h"
#include "syringe.h"

extern Syringe syringe1;

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
	//SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	//NVIC_EnableIRQ(SPI1_IRQn);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_IRQHandler() {
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
		aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPI1);
		switch(aRxBuffer[0]) {
			case M1_SPEED1:
				break;
			case M1_SPEED1 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M2_SPEED2:
				break;
			case M2_SPEED2 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M1_DIRECTION:
				break;
			case M1_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M2_DIRECTION:
				break;
			case M2_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case M1_ENC:
				break;
			case M2_ENC:
				break;
			case VALVE1:
				break;
			case VALVE1 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case VALVE2:
				break;
			case VALVE2 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case DOS1_EN:
				break;
			case DOS1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (syringe1.cover.getState() == CLOSE) {
						syringe1.en(true);
					}
				}
				break;
			case DOS2_EN:
				break;
			case DOS2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case DOS1_SINGLE_DOSE:
				break;
			case DOS1_SINGLE_DOSE | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe1.setDose(aRxBuffer[1]);
				}
				break;
			case DOS2_SINGLE_DOSE:
				break;
			case DOS2_SINGLE_DOSE | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
			case DOS1_SPEED:
				break;
			case DOS1_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe1.setSpeed(aRxBuffer[1]);
				}
				break;
			case DOS2_SPEED:
				break;
			case DOS2_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
				}
				break;
		}
	}
}
