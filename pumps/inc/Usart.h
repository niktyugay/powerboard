#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include "stdio.h"
//#include "stm32f4xx.h"
//#include "system_stm32f4xx.c"
//#include "stm32f4xx_conf.h"

#define LENGTH_DATA_ARRAY	20

#define maxChar 0xFF
void initUart1(void);
void initUart2(void);
void SendSt(char* str_p,USART_TypeDef * US);
void SendNum(float s);
void SendNext(void);
//void USART2_IRQHandler(void);
void sendCommandSPI1(int j);
void checkCommands(void);
void SendNumInt(int s);
//void USART1_IRQHandler(void);
//void USART3_IRQHandler(void);
void getNextUartByte(uint8_t *temp,USART_TypeDef* USARTx);
void isEmptyUart(uint8_t *b,USART_TypeDef* USARTx);


#endif
