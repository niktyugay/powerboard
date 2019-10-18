#ifndef DRIVER_H_
#define DRIVER_H_

#include "stm32f10x.h"

extern void delay_ms(uint32_t);

typedef enum {INTERNAL = 0, EXTERNAL = !DISABLE} ClockSourceDriver;
//
// Default values
typedef struct{
	uint32_t 	GCONF_sr, // 
					GSTAT_sr,
					SLAVECONF_sr,
					OTP_PROG_sr,
					OTP_READ_sr,
					FACTORY_CONF_sr,
					IHOLD_IRUN_sr,//0x00010000UL, // Reset default would be IRUN=31 IHOLD=16
					TPOWERDOWN_sr,
					TPWMTHRS_sr,
					VACTUAL_sr,
					CHOPCONF_sr,//0x10000053UL,
					PWMCONF_sr;
	uint16_t replyDelay;
	uint8_t TxCounter;
	uint32_t TSTEP_sr;
	USART_TypeDef* USARTx;
}Driver_str;
//


#define SEND_DRIVER(USARTy,x) while(!(USARTy->SR & USART_SR_TC)){};USART_SendData((USARTy), (x)) 
#define READ_DRIVER(USARTy,data)	getNextUartByte(&(data),(USARTy))
#define USART_IS_EMPTY(USARTy,b) isEmptyUart(&(b),(USARTy))
#define DELAY_MS(x)	Delay(x)


//REGISTER MAPPING

#define TMC2208_READ 				0x00
#define TMC2208_WRITE 			0x80
#define TMC2208_SYNC 				0x05
#define TMC2208_SLAVE_ADDR	0x00

// Register memory positions
#define REG_GCONF 				0x00
#define REG_GSTAT 				0x01
#define REG_IFCNT 				0x02
#define REG_SLAVECONF 		0x03
#define REG_OTP_PROG 			0x04
#define REG_OTP_READ 			0x05
#define REG_IOIN 					0x06
#define REG_FACTORY_CONF 	0x07
#define REG_IHOLD_IRUN 		0x10
#define REG_TPOWERDOWN 		0x11
#define REG_TSTEP 				0x12
#define REG_TPWMTHRS 			0x13
#define REG_VACTUAL 			0x22
#define REG_MSCNT 				0x6A
#define REG_MSCURACT 			0x6B
#define REG_CHOPCONF 			0x6C
#define REG_DRV_STATUS 		0x6F
#define REG_PWMCONF 			0x70
#define REG_PWM_SCALE 		0x71
#define REG_PWM_AUTO 			0x72

// GCONF
#define I_SCALE_ANALOG_bp		0
#define INTERNAL_RSENSE_bp	1
#define EN_SPREADCYCLE_bp		2
#define SHAFT_bp						3
#define INDEX_OTPW_bp				4
#define INDEX_STEP_bp				5
#define PDN_DISABLE_bp			6
#define MSTEP_REG_SELECT_bp	7
#define MULTISTEP_FILT_bp		8
#define I_SCALE_ANALOG_bm		1ul//0b1ul
#define INTERNAL_RSENSE_bm	2ul//0b10ul
#define EN_SPREADCYCLE_bm		4ul//0b100ul
#define SHAFT_bm						8ul
#define INDEX_OTPW_bm				16ul//0b10000ul
#define INDEX_STEP_bm				32ul//0b100000ul
#define PDN_DISABLE_bm			64ul//0b1000000ul
#define MSTEP_REG_SELECT_bm	128ul//0b10000000ul
#define MULTISTEP_FILT_bm		256ul//0b100000000ul
// GSTAT
#define RESET_bp 			0
#define DRV_ERR_bp		1
#define UV_CP_bp			2
#define RESET_bm 			1ul//0b1ul
#define DRV_ERR_bm		2ul//0b10ul
#define UV_CP_bm			4ul//0b100ul
// IFCNT
#define IFCNT_bp			0
#define IFCNT_bm			255ul//0xFFul
// SLAVECONF
#define SLAVECONF_bp		0
#define SLAVECONF_bm		3840ul//0xF00ul
#define SENDDELAY_bp		8
#define SENDDELAY_bm		3840ul//0xF00ul
// OTP_PROG
#define OTPBIT_bp				0
#define OTPBYTE_bp			4
#define OTPMAGIC_bp			8
#define OTPBIT_bm				7ul//0b111ul
#define OTPBYTE_bm			48ul//0b110000ul
#define OTPMAGIC_bm			65280ul//0b1111111100000000ul
// OTP_READ
#define OTP0_bp				0
#define OTP1_bp				8
#define OTP2_bp			  16
#define OTP0_bm				255ul//0xFFul
#define OTP1_bm				65280ul//0xFF00ul
#define OTP2_bm			  16711680ul//0xFF0000ul
// IOIN
#define ENN_bp				0
#define MS1_bp				2
#define MS2_bp				3
#define DIAG_bp				4
#define PDN_UART_bp		6
#define STEP_bp 			7
#define SEL_A_bp 			8
#define DIR_bp				9
#define VERSION_bp		24
#define ENN_bm				1ul//0b1ul
#define MS1_bm				4ul//0b100ul
#define MS2_bm				8ul//0b1000ul
#define DIAG_bm				16ul//0b10000ul
#define PDN_UART_bm		64ul//0b1000000ul
#define STEP_bm 			128ul//0b10000000ul
#define SEL_A_bm 			256ul//0b100000000ul
#define DIR_bm				512ul//0b1000000000ul
#define VERSION_bm		4278190080ul//0xFF000000ul
// FACTORY_CONF
#define FCLKTRIM_bp		0
#define OTTRIM_bp			8
#define FCLKTRIM_bm		31ul//0x1Ful
#define OTTRIM_bm			768ul//0x300ul
// IHOLD_IRUN
#define IHOLD_bp 				0
#define IRUN_bp					8
#define IHOLDDELAY_bp	  16
#define IHOLD_bm 				31ul//0x1Ful
#define IRUN_bm					7936ul//0x1F00ul
#define IHOLDDELAY_bm		983040ul//0xF0000ul
// TPOWERDOWN
#define TPOWERDOWN_bp		0
#define TPOWERDOWN_bm		255ul//0xFFul
// MSCURACT
#define CUR_A_bp			0
#define CUR_B_bp		  16
#define CUR_A_bm			511ul//0x1FFul
#define CUR_B_bm			33488896ul//0x1FF0000ul
// PWM_SCALE
#define PWM_SCALE_SUM_bp	0
#define PWM_SCALE_AUTO_bp 16
#define PWM_SCALE_SUM_bm	255ul//0xFFul
#define PWM_SCALE_AUTO_bm 33488896ul//0x1FF0000ul
// PWM_AUTO
#define PWM_OFS_AUTO_bp		0
#define PWM_GRAD_AUTO_bp  16
#define PWM_OFS_AUTO_bm		255ul//0xFFul
#define PWM_GRAD_AUTO_bm 	16711680ul//0xFF0000ul
// OTP_READ
#define OTP_FCLKTRIM_bp					0
#define OTP_OTTRIM_bp						5
#define OTP_INTERNALRSENSE_bp 	6
#define OTP_TBL_bp 							7
#define OTP_PWM_GRAD_bp					8
#define OTP_PWM_AUTOGRAD_bp 		12
#define OTP_CHOPCONF_bp	   			12
#define OTP_TPWMTHRS_bp	   			13
#define OTP_PWM_OFS_bp	   			16
#define OTP_PWM_REG_bp	   			17
#define OTP_OTP_PWM_FREQ_bp 		18
#define OTP_IHOLDDELAY_bp  			19
#define OTP_IHOLD_bp 	   				21
#define OTP_OTP_EN_SPREADCYCLE	23
// CHOPCONF
#define TOFF_bp				0
#define HSTRT_bp			4
#define HEND_bp				7
#define TBL_bp 			  15
#define VSENSE_bp		  17
#define MRES_bp 		  24
#define INTPOL_bp		  28
#define DEDGE_bp		  29
#define DISS2G_bp		  30
#define DISS2VS_bp		31
#define TOFF_bm			 	15ul//0xFul
#define HSTRT_bm		 	112ul//0x70ul
#define HEND_bm				1920ul//0x780ul
#define TBL_bm		  	98304ul//0x18000ul
#define VSENSE_bm	  	131072ul//0x20000ul
#define MRES_bm				251658240ul//0xF000000ul
#define INTPOL_bm  		268435456ul//0x10000000ul
#define DEDGE_bm   		536870912ul//0x20000000ul
#define DISS2G_bm  		1073741824ul//0x40000000ul
#define DISS2VS_bm 		2147483648ul//0x80000000ul
// PWMCONF
#define PWM_OFS_bp 					0
#define PWM_GRAD_bp 				8
#define PWM_FREQ_bp 	   		16
#define PWM_AUTOSCALE_bp   	18
#define PWM_AUTOGRAD_bp    	19
#define FREEWHEEL_bp 	   		20
#define PWM_REG_bp 		   		24
#define PWM_LIM_bp		   		28
#define PWM_OFS_bm 					255ul//0xFFul
#define PWM_GRAD_bm 				65280ul//0xFF00ul
#define PWM_FREQ_bm 	    	196608ul//0x30000ul
#define PWM_AUTOSCALE_bm    262144ul//0x40000ul
#define PWM_AUTOGRAD_bm     524288ul//0x80000ul
#define FREEWHEEL_bm 	    	3145728ul//0x300000ul
#define PWM_REG_bm 		    	251658240ul//0xF000000ul
#define PWM_LIM_bm		    	4026531840ul//0xF0000000ul
// DRV_STATUS
#define OTPW_bp 			0
#define OT_bp 				1
#define S2GA_bp				2
#define S2GB_bp				3
#define S2VSA_bp			4
#define S2VSB_bp			5
#define OLA_bp 				6
#define OLB_bp 				7
#define T120_bp				8
#define T143_bp				9
#define T150_bp			  10
#define T157_bp			  11
#define CS_ACTUAL_bp	16
#define STEALTH_bp		30
#define STST_bp			  31
#define OTPW_bm 			1ul//0b1ul
#define OT_bm 				2ul//0b10ul
#define S2GA_bm				4ul//0b100ul
#define S2GB_bm				8ul//0b1000ul
#define S2VSA_bm			16ul//0b10000ul
#define S2VSB_bm			32ul//0b100000ul
#define OLA_bm 				64ul//0b1000000ul
#define OLB_bm 				128ul//0b10000000ul
#define T120_bm				256ul//0b100000000ul
#define T143_bm				512ul//0b1000000000ul
#define T150_bm			  1024ul//0b10000000000ul
#define T157_bm			  2048ul//0b100000000000ul
#define CS_ACTUAL_bm	2031616ul//0x1F0000ul
#define STEALTH_bm		1073741824ul//0x40000000ul
#define STST_bm			  2147483648ul//0x80000000ul

#define VACTUAL_bm 0
#define VACTUAL_bp 0



//MACROS
#define REGISTER_W(R,DRIVE_STR) sendDatagram(TMC2208_WRITE|REG_##R, (DRIVE_STR)->R##_sr,(DRIVE_STR));
#define REGISTER_R(R,DRIVE_STR) uint32_t dataTemp; uint8_t b = readDatagram(TMC2208_READ|REG_##R, &dataTemp,(DRIVE_STR)); (DRIVE_STR)->R##_sr = dataTemp; 

#define MOD_REG(REG, SETTING, VALUE,DRIVE_STR) (DRIVE_STR)->REG##_sr &= ~SETTING##_bm; (DRIVE_STR)->REG##_sr |= ((uint32_t)VALUE<<SETTING##_bp)&SETTING##_bm; REGISTER_W(REG,(DRIVE_STR));

#define GET_BYTE(REG, SETTING) REG(&REG##_sr); 	return (REG##_sr&SETTING##_bm)	>>SETTING##_bp;


//
uint8_t calcCRC(uint8_t datagram[], uint8_t len);
void sendDatagram(uint8_t addr, uint32_t regVal, Driver_str* Driver_Struct);
uint8_t readDatagram(uint8_t addr, uint32_t *data, Driver_str* Driver_Struct); 
uint64_t _sendDatagram(uint8_t datagram[], uint8_t len, Driver_str* Driver_Struct);
void Driver_StructInit(Driver_str* Driver_InitStruct, USART_TypeDef* USARTx);
uint8_t initDriver(Driver_str* Driver_Struct);
uint8_t readTxRegCounter( Driver_str* Driver_Struct);
uint8_t checkTransmission(uint8_t countTransfer,Driver_str* Driver_Struct);//Good - 0, Bad - 1
uint8_t writeAllReg(Driver_str* Driver_Struct);
uint8_t spreadCycle(FunctionalState e,Driver_str* Driver_Struct);
uint32_t readTstep(Driver_str* Driver_Struct);
uint16_t getPWM_Freq(ClockSourceDriver clock,Driver_str* Driver_Struct);
void readAllReg(Driver_str* Driver_Struct);
uint8_t setSpeed(uint8_t rotationsPerMinute,Driver_str* Driver_Struct);
uint16_t getMRESfromSR(Driver_str* Driver_Struct);
uint16_t getMRESfromDriver(Driver_str* Driver_Struct);
uint8_t sendMRES(uint16_t resolution,Driver_str* Driver_Struct);//
uint8_t setMRES(uint16_t resolution,Driver_str* Driver_Struct);
uint8_t setDirection(uint8_t direction,Driver_str* Driver_Struct);

//


#endif
