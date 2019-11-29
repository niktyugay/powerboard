#include "Usart.h"

volatile  char commandsFromUART[LENGTH_DATA_ARRAY];
volatile unsigned char  countCommandsFromUART=0;
volatile  int  currentPositionUARTinInterrupt=0;
volatile   int currentPositionUART = 0;

volatile  char commandsFromUART3[LENGTH_DATA_ARRAY];
volatile unsigned char  countCommandsFromUART3=0;
volatile  int  currentPositionUARTinInterrupt3=0;
volatile   int currentPositionUART3 = 0;
int lenght=0;
int i = 0;
volatile char singleCommand[50];

void initUart1() {
	GPIO_InitTypeDef gpio_struct;
	USART_InitTypeDef	usart_struct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_9;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin = GPIO_Pin_10;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	usart_struct.USART_BaudRate = 115200;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &usart_struct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
}

void initUart2() {
	GPIO_InitTypeDef gpio_struct;
	USART_InitTypeDef	usart_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_2;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin = GPIO_Pin_3;
	gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct);
	usart_struct.USART_BaudRate = 115200;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &usart_struct);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);
}

void initUart3() {
	GPIO_InitTypeDef gpio_struct;
	USART_InitTypeDef	usart_struct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin = GPIO_Pin_10;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_struct);
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_struct.GPIO_Pin = GPIO_Pin_11;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_struct);
	usart_struct.USART_BaudRate = 115200;
	usart_struct.USART_WordLength = USART_WordLength_8b;
	usart_struct.USART_StopBits = USART_StopBits_1;
	usart_struct.USART_Parity = USART_Parity_No;
	usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &usart_struct);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(USART3_IRQn);
	USART_Cmd(USART3, ENABLE);
}

void SendSt(char* str_p,USART_TypeDef * US)
{
		uint16_t i=0;
		
		while(str_p[i])
		{
		while(!(US->SR & USART_SR_TC)){};
			USART_SendData(US, str_p[i]);
			i++;
		}
}
void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
void checkCommands(void){
	if(countCommandsFromUART>0){
		countCommandsFromUART--;
			USART_SendData(USART2,commandsFromUART[currentPositionUART]);
			//CS1_ON();
			SPI_I2S_SendData(SPI1,commandsFromUART[currentPositionUART]);
			//TIM_SetCompare1(TIM4,commandsFromUART[currentPositionUART]);
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		
			//CS1_OFF();
			//invLED(LED_RED);
			SendNext();
			if(currentPositionUART+1>LENGTH_DATA_ARRAY-12){currentPositionUART = 0;}
			else{
				currentPositionUART++;
			}
	}
	
}
void sendCommandSPI1(int j){
		/*for(int i = 0; i < j;i++){
			SPI_I2S_SendData(SPI1,singleCommand[i]);
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == RESET);
		}
		if(singleCommand[0]<(char)128){
			SPI_I2S_SendData(SPI1,maxChar);
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == RESET);
			lenght = SPI_I2S_ReceiveData(SPI1);
			for(int i = 0; i < lenght; i++){
			SPI_I2S_SendData(SPI1,maxChar);
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == RESET);
			while(!(USART1->SR & USART_SR_TC)){};
			USART_SendData(USART1,SPI_I2S_ReceiveData(SPI1));
			}
		}
	*/
}

void USART1_IRQHandler(void) {
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		commandsFromUART[currentPositionUARTinInterrupt]=USART_ReceiveData(USART1);
		if ((currentPositionUARTinInterrupt+2>LENGTH_DATA_ARRAY)) {
			currentPositionUARTinInterrupt=0;}
		else {
			currentPositionUARTinInterrupt++;
		}
	}
}

void getNextUartByte(uint8_t *temp,USART_TypeDef* USARTx) {
	if(USARTx==USART1){
		if(currentPositionUART!=currentPositionUARTinInterrupt) {
			*temp = commandsFromUART[currentPositionUART];
			if (currentPositionUART+2>LENGTH_DATA_ARRAY) {
				currentPositionUART = 0;
			}
			else {
				currentPositionUART++;
			}
		}
	}
	else {
		if(currentPositionUART3!=currentPositionUARTinInterrupt3) {
			*temp = commandsFromUART3[currentPositionUART3];
			if (currentPositionUART3+2>LENGTH_DATA_ARRAY) {
				currentPositionUART3 = 0;
			}
			else {
				currentPositionUART3++;
			}
		}
	}
}

void isEmptyUart(uint8_t *b,USART_TypeDef* USARTx){
	if(USARTx==USART1) {
		if (currentPositionUART==currentPositionUARTinInterrupt) {
			*b=1;
		}
		else {
			*b=0;
		}
	}
	else {
		if(currentPositionUART3==currentPositionUARTinInterrupt3){
			*b=1;
		}
		else {
			*b=0;
		}
	}
}
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		commandsFromUART3[currentPositionUARTinInterrupt3]=USART_ReceiveData(USART3);
		if	((currentPositionUARTinInterrupt3+2>LENGTH_DATA_ARRAY))	{
			currentPositionUARTinInterrupt3=0;}
		else	{
			currentPositionUARTinInterrupt3++;
		}
	}
}
