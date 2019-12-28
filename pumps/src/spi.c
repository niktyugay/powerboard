/*
 * spi.c
 *
 *  Created on: 16 ���. 2019 �.
 *      Author: tugay
 */

#include "spi.h"
#include "syringe.h"
#include "driver.h"

extern Syringe syringe1, syringe2;

uint8_t aRxBuffer[10];
uint8_t ubRxIndex = 0;

extern bool flagValve1, flagValve2;
extern uint8_t speedM1, speedM2, directionM1, directionM2;
extern bool flagM1, flagM2;
extern bool flagChangeM1,flagChangeM2;

uint8_t syringe1speed = 0;
uint8_t syringe2speed = 0;

void SPI_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // CPOL_Low => CPOL = 0; CPOL_High => CPOL = 1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // CPHA_1Edge => CPHA = 0; CPHA_2Edge => CPHA = 1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI1, &SPI_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // MISO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // CS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5; // SCLK | MOSI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(SPI1_IRQn);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t send_get_data(uint8_t value) {
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_IT_TXE) == RESET);
	SPI_I2S_SendData(SPI1, value);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

void SPI1_IRQHandler() {
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
		aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPI1);
		switch(aRxBuffer[0]) {

			/*============================== SELF TEST =============================*/
			case SELFTEST | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case SELFTEST:
				SPI_I2S_SendData(SPI1, 0x01);
				ubRxIndex = 0;
				break;
			/*======================================================================*/

			/*=============================== MOTOR 1 ==============================*/
			case M1_SPEED1 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					speedM1 = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M1_SPEED1:
				SPI_I2S_SendData(SPI1, speedM1);
				ubRxIndex = 0;
				break;
			case M1_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					directionM1 = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M1_DIRECTION:
				SPI_I2S_SendData(SPI1, directionM1);
				ubRxIndex = 0;
				break;
			case M1_ENC:
				SPI_I2S_SendData(SPI1, speedM1);
				ubRxIndex = 0;
				break;
			case M1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					flagChangeM1 = true;
					if (aRxBuffer[1] > 0) {
						flagM1 = true;
					}
					else {
						flagM1 = false;
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			/*======================================================================*/

			/*=============================== MOTOR 2 ==============================*/
			case M2_SPEED2 | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					speedM2 = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M2_SPEED2:
				SPI_I2S_SendData(SPI1, speedM2);
				ubRxIndex = 0;
				break;
			case M2_ENC:
				SPI_I2S_SendData(SPI1, speedM2);
				ubRxIndex = 0;
				break;
			case M2_DIRECTION | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					directionM2 = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case M2_DIRECTION:
				SPI_I2S_SendData(SPI1, directionM2);
				ubRxIndex = 0;
				break;
			case M2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					flagChangeM2 = true;
					if (aRxBuffer[1] > 0) {
						flagM2 = true;
					}
					else {
						flagM2 = false;
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			/*======================================================================*/

			/*=============================== VALVE 1 ==============================*/
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
			case VALVE1:
				if (flagValve1) {
					SPI_I2S_SendData(SPI1, 0x11);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				ubRxIndex = 0;
				break;
			/*======================================================================*/

			/*=============================== VALVE 2 ==============================*/
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
			case VALVE2:
				if (flagValve2) {
					SPI_I2S_SendData(SPI1, 0x11);
				}
				else {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				ubRxIndex = 0;
				break;
			/*======================================================================*/

			/*================================ DOS 1 ===============================*/
			case DOS1_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (syringe1.cover.state == CLOSE && aRxBuffer[1] == 0xFF) {
						syringe1.speed = syringe1speed;
						syringe1.start();
					}
					else {
						syringe1.speed = 0;
						syringe1.stop();
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_EN:
				if (syringe1.en) {
					SPI_I2S_SendData(SPI1, 0x11);
				}
				else {
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
			case DOS1_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe1speed = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS1_SPEED:
				SPI_I2S_SendData(SPI1, syringe1.speed);
				ubRxIndex = 0;
				break;
			case DOS1_VOLUME:
				SPI_I2S_SendData(SPI1, syringe1.volume);
				ubRxIndex = 0;
				break;
 			/*======================================================================*/

 			/*================================ DOS 2 ===============================*/
			case DOS2_EN | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					if (syringe2.cover.state == CLOSE && aRxBuffer[1] == 0xFF) {
						syringe2.speed = syringe2speed;
						syringe2.start();
					}
					else {
						syringe2.speed = 0;
						syringe2.stop();
					}
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_EN:
				if (syringe2.en) { SPI_I2S_SendData(SPI1, 0x11); }
				else { SPI_I2S_SendData(SPI1, 0x00); }
				break;
			case DOS2_SINGLE_DOSE | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe2.setDose(aRxBuffer[1]);
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_SPEED | CMD_SET:
				if (ubRxIndex == 2) {
					ubRxIndex = 0;
					syringe2speed = aRxBuffer[1];
					while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, 0x00);
				}
				break;
			case DOS2_SPEED:
				SPI_I2S_SendData(SPI1, syringe2.speed);
				ubRxIndex = 0;
				break;
			case DOS2_VOLUME:
				SPI_I2S_SendData(SPI1, syringe2.volume);
				ubRxIndex = 0;
				break;
			/*======================================================================*/

			/*============================= DOS COMMON =============================*/
			case DOS_COVERS:
				if ((syringe1.cover.state == CLOSE) && (syringe2.cover.state == CLOSE)) {
					SPI_I2S_SendData(SPI1, 0x00);
				}
				else if ((syringe1.cover.state == OPEN) && (syringe2.cover.state == CLOSE)) {
					SPI_I2S_SendData(SPI1, 0x01);
				}
				else if ((syringe1.cover.state == CLOSE) && (syringe2.cover.state == OPEN)) {
					SPI_I2S_SendData(SPI1, 0x10);
				}
				else if ((syringe1.cover.state == OPEN) && (syringe2.cover.state == OPEN)){
					SPI_I2S_SendData(SPI1, 0x11);
				}
				ubRxIndex = 0;
				break;
			/*======================================================================*/

			default :
				ubRxIndex = 0;
				while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, 0x00);
				break;

		}
	}
}
