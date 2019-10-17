/*
 * spi.c
 *
 *  Created on: 16 ���. 2019 �.
 *      Author: tugay
 */

#include "spi.h"
#include "syringe.h"

extern Syringe syringe1, syringe2;

uint8_t aRxBuffer[10];
uint8_t ubRxIndex = 0;

extern bool flagValve1, flagValve2;

void SPI_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
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
	NVIC_EnableIRQ(SPI1_IRQn);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_IRQHandler() {
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
		aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPI1);
		switch(aRxBuffer[0]) {
			case M1_SPEED1:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x55);
				ubRxIndex = 0;
				break;
			case M1_SPEED1 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M2_SPEED2:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x55);
				ubRxIndex = 0;
				break;
			case M2_SPEED2 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M1_DIRECTION:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x55);
				ubRxIndex = 0;
				break;
			case M1_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M2_DIRECTION:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x55);
				ubRxIndex = 0;
				break;
			case M2_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M1_ENC:
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {}
				SPI_I2S_SendData(SPI1, 0xFF);
				ubRxIndex = 0;
				break;
			case M2_ENC:
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {}
				SPI_I2S_SendData(SPI1, 0xFF);
				ubRxIndex = 0;
				break;
			case VALVE1:
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {}
				if (flagValve1) {
					SPI_I2S_SendData(SPI1, 0xFF);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				ubRxIndex = 0;
				break;
			case VALVE1 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (aRxBuffer[1] == 0xFF) {
						flagValve1 = true;
					}
					else {
						flagValve1 = false;
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case VALVE2:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				if (flagValve2) {
					SPI_I2S_SendData(SPI1, 0xFF);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				ubRxIndex = 0;
				break;
			case VALVE2 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (aRxBuffer[1] == 0xFF) {
						flagValve2 = true;
					}
					else {
						flagValve2 = false;
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_EN:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				if (syringe1.param.en) {
					SPI_I2S_SendData(SPI1, 0xFF);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
 				break;
			case DOS1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (syringe1.cover.getState() == CLOSE) {
						syringe1.en(true);
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_EN:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				if (syringe2.param.en) {
					SPI_I2S_SendData(SPI1, 0xFF);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (syringe2.cover.getState() == CLOSE) {
						syringe2.en(true);
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_SINGLE_DOSE | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe1.setDose(aRxBuffer[1]);
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_SINGLE_DOSE | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe2.setDose(aRxBuffer[1]);
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_SPEED:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, syringe1.speed.speed);
				ubRxIndex = 0;
				break;
			case DOS1_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe1.setSpeed(aRxBuffer[1]);
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_SPEED:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, syringe2.speed.speed);
				ubRxIndex = 0;
				break;
			case DOS2_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_VOLUME:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, syringe1.getVolume);
				ubRxIndex = 0;
				break;
			case DOS2_VOLUME:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, syringe2.getVolume);
				ubRxIndex = 0;
				break;
			case DOS_COVERS:
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				if (syringe1.cover.getState() == CLOSE && syringe2.cover.getState() == CLOSE) {
					SPI_I2S_SendData(SPI1, 0x03);
				}
				else if (syringe1.cover.getState() == OPEN && syringe2.cover.getState() == CLOSE) {
					SPI_I2S_SendData(SPI1, 0x01);
				}
				else if (syringe1.cover.getState() == CLOSE && syringe2.cover.getState() == OPEN) {
					SPI_I2S_SendData(SPI1, 0x02);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				ubRxIndex = 0;
				break;
			default :
				ubRxIndex = 0;
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x00);
				break;
		}
	}
}
