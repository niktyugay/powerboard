#include "driver.h"
// Default values

void Driver_StructInit(Driver_str* Driver_InitStruct, USART_TypeDef* USARTx){
	Driver_InitStruct->GCONF_sr = 0x000001e3UL;
	Driver_InitStruct->GSTAT_sr = 0x00000000UL;
	Driver_InitStruct->SLAVECONF_sr = 0x00000000UL;
	Driver_InitStruct->OTP_PROG_sr = 0x00000000UL;
	Driver_InitStruct->OTP_READ_sr = 0x00000000UL;
	Driver_InitStruct->FACTORY_CONF_sr = 0x0000000dUL;
	Driver_InitStruct->IHOLD_IRUN_sr = 0x00011f10UL;
	Driver_InitStruct->TPOWERDOWN_sr = 0x00000014UL;
	Driver_InitStruct->TPWMTHRS_sr = 0x00000000UL;
	Driver_InitStruct->VACTUAL_sr = 0x00000000UL;
	Driver_InitStruct->CHOPCONF_sr = 0x10000053UL;
	Driver_InitStruct->PWMCONF_sr = 0xC80f0e24UL;
	Driver_InitStruct->TxCounter = 0;
	Driver_InitStruct->replyDelay = 5;
	Driver_InitStruct->USARTx = USARTx;
}

uint8_t calcCRC(uint8_t datagram[], uint8_t len) {
	uint8_t crc = 0;
	for (uint8_t i = 0; i < len; i++) {
		uint8_t currentByte = datagram[i];
		for (uint8_t j = 0; j < 8; j++) {
			if ((crc >> 7) ^ (currentByte & 0x01)) {
				crc = (crc << 1) ^ 0x07;
			} else {
				crc = (crc << 1);
			}
			crc &= 0xff;
			currentByte = currentByte >> 1;
		} 
	}
	return crc;
}

void sendDatagram(uint8_t addr, uint32_t regVal, Driver_str* Driver_Struct) {
	uint8_t len = 7;
	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, (uint8_t)(regVal>>24), (uint8_t)(regVal>>16), (uint8_t)(regVal>>8), (uint8_t)(regVal>>0), 0x00};
	datagram[len] = calcCRC(datagram, len);
			for(int i=0; i<=len; i++){
				SEND_DRIVER(Driver_Struct->USARTx ,datagram[i]);
			}
}


uint8_t readDatagram(uint8_t addr, uint32_t *data, Driver_str* Driver_Struct) {
	uint8_t len = 3;
	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, 0x00};
	datagram[len] = calcCRC(datagram, len);
	uint64_t out = 0x00000000UL;
	
		out = _sendDatagram( datagram, len, Driver_Struct);

	uint8_t out_datagram[] = {(uint8_t)(out>>56), (uint8_t)(out>>48), (uint8_t)(out>>40), (uint8_t)(out>>32), (uint8_t)(out>>24), (uint8_t)(out>>16), (uint8_t)(out>>8), (uint8_t)(out>>0)};
	if (calcCRC(out_datagram, 7) == (uint8_t)(out&0xFF)) {
		*data = out>>8;
		return 0;
	} else {
		return 1;
	}
}

uint64_t _sendDatagram(uint8_t datagram[], uint8_t len, Driver_str* Driver_Struct) {
	uint64_t out = 0x00000000UL;
	uint8_t isEmpty;
	uint8_t tempData;
	USART_IS_EMPTY(Driver_Struct->USARTx,isEmpty);
	
	while (!isEmpty){
		READ_DRIVER(Driver_Struct->USARTx,tempData);
		USART_IS_EMPTY(Driver_Struct->USARTx,isEmpty);
	} // Flush

	for(int i=0; i<=len; i++) {
		SEND_DRIVER(Driver_Struct->USARTx,datagram[i]);
		//USART_SendData(USART2,datagram[i]);
	}
	for(int byte=0; byte<4; byte++) {
		READ_DRIVER(Driver_Struct->USARTx,tempData);
		//USART_SendData(USART2,tempData);
	} // Flush bytes written
	DELAY_MS(Driver_Struct->replyDelay);
	
	USART_IS_EMPTY(Driver_Struct->USARTx,isEmpty);
	while(!isEmpty) {
		uint8_t res;
		READ_DRIVER(Driver_Struct->USARTx,res);
		out <<= 8;
		out |= res&0xFF;
		USART_IS_EMPTY(Driver_Struct->USARTx,isEmpty);
	}
	
	return out;
}
uint8_t initDriver(Driver_str* Driver_Struct){
	Driver_Struct->TxCounter = readTxRegCounter(Driver_Struct);
	return writeAllReg(Driver_Struct);
			
}

uint8_t readTxRegCounter( Driver_str* Driver_Struct){
	uint32_t counter;
	readDatagram(TMC2208_READ|REG_IFCNT, &counter,Driver_Struct);
	return counter;
}

uint8_t checkTransmission(uint8_t countTransfer,Driver_str* Driver_Struct){
	Driver_Struct->TxCounter+=countTransfer;
	if(Driver_Struct->TxCounter==readTxRegCounter(Driver_Struct)){
		return 0;
	}
	Driver_Struct->TxCounter = readTxRegCounter(Driver_Struct);
		return 1;
}

uint8_t writeAllReg(Driver_str* Driver_Struct){
	REGISTER_W(GCONF,Driver_Struct); 
	REGISTER_W(SLAVECONF,Driver_Struct);
	REGISTER_W(FACTORY_CONF,Driver_Struct);
	REGISTER_W(IHOLD_IRUN,Driver_Struct);
	REGISTER_W(TPOWERDOWN,Driver_Struct);
	REGISTER_W(TPWMTHRS,Driver_Struct);
	REGISTER_W(VACTUAL,Driver_Struct);	
	REGISTER_W(CHOPCONF,Driver_Struct);
	REGISTER_W(PWMCONF,Driver_Struct);
	return checkTransmission(9,Driver_Struct);
}
void readAllReg(Driver_str* Driver_Struct){
	REGISTER_R(PWMCONF,Driver_Struct);
//	uint32_t testPWMCONF;
//	readDatagram(TMC2208_READ|REG_PWMCONF,&testPWMCONF);
//	PWMCONF_sr = testPWMCONF;
}
uint8_t spreadCycle(FunctionalState e,Driver_str* Driver_Struct){
	MOD_REG(GCONF,EN_SPREADCYCLE,e,Driver_Struct);
	return checkTransmission(1,Driver_Struct);
}

uint32_t readTstep(Driver_str* Driver_Struct){
	uint32_t counter;
	readDatagram(TMC2208_READ|REG_TSTEP, &counter,Driver_Struct);
	return counter;
}

uint16_t getPWM_Freq(ClockSourceDriver clock,Driver_str* Driver_Struct){
	if(clock==0){
		switch((Driver_Struct->PWMCONF_sr&PWM_FREQ_bm)>>PWM_FREQ_bp){
			case(0):{return (uint16_t)23400;}
			case(1):{return (uint16_t)35100;}
			case(2):{return (uint16_t)46900;}
			case(3):{return (uint16_t)58500;}
			default:{return (uint16_t)0;}
		}
	}
	return (uint16_t)0;
}
uint8_t setSpeed(uint8_t rotationsPerMinute,Driver_str* Driver_Struct){
	Driver_Struct->VACTUAL_sr = rotationsPerMinute * getMRESfromSR(Driver_Struct) * 200 /	42.9;
		REGISTER_W(VACTUAL,Driver_Struct);
	return checkTransmission(1,Driver_Struct);
}

uint16_t getMRESfromDriver(Driver_str* Driver_Struct){
	REGISTER_R(CHOPCONF,Driver_Struct);
	return getMRESfromSR(Driver_Struct);
}

uint16_t getMRESfromSR(Driver_str* Driver_Struct){
	switch((Driver_Struct->CHOPCONF_sr&MRES_bm)>>MRES_bp){
		case(0):{return (uint16_t)256;}
		case(1):{return (uint16_t)128;}
		case(2):{return (uint16_t)64;}
		case(3):{return (uint16_t)32;}
		case(4):{return (uint16_t)16;}
		case(5):{return (uint16_t)8;}
		case(6):{return (uint16_t)4;}
		case(7):{return (uint16_t)2;}
		case(8):{return (uint16_t)1;}
		}
	return 0;
}

uint8_t setMRES(uint16_t resolution,Driver_str* Driver_Struct){
	switch(resolution){
		case(256):{return sendMRES(0,Driver_Struct);}
		case(128):{return sendMRES(1,Driver_Struct);}
		case(64):{return 	sendMRES(2,Driver_Struct);}
		case(32):{return 	sendMRES(3,Driver_Struct);}
		case(16):{return 	sendMRES(4,Driver_Struct);}
		case(8):{return 	sendMRES(5,Driver_Struct);}
		case(4):{return 	sendMRES(6,Driver_Struct);}
		case(2):{return 	sendMRES(7,Driver_Struct);}
		case(1):{return 	sendMRES(8,Driver_Struct);}
	}
	return 1;
}

uint8_t sendMRES(uint16_t resolution,Driver_str* Driver_Struct){
	MOD_REG(CHOPCONF, MRES, resolution,Driver_Struct);
	return checkTransmission(1,Driver_Struct);
}
uint8_t setDirection(uint8_t direction,Driver_str* Driver_Struct){
	if(direction==1||direction==0){
		MOD_REG(GCONF, SHAFT, direction,Driver_Struct);
		return checkTransmission(1,Driver_Struct);
	}else{
		return 1;
	}
}
