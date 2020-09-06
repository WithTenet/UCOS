#include "bsp_bluetooth.h"
#include "stm32f4xx.h"
#include <stdio.h>
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ; 
	
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
	
	
	
	
	
	RCC_APB1PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
  USART_Init(DEBUG_USART, &USART_InitStructure);
	
  USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(DEBUG_USART, ENABLE);
	void NVIC_Configuration(void);
}





//发送一个字符设置
void USART_SendByte (USART_TypeDef * pUSARTx ,char ch)
{
   USART_SendData( pUSARTx, ch);
	 while(USART_GetFlagStatus( pUSARTx ,USART_FLAG_TXE)==RESET); 

}

//发送字符串
void USART_SendString(USART_TypeDef * pUSARTx, char *str)
{unsigned int k=0;
	
	do{
		 USART_SendByte(pUSARTx, *(str+k));
	   k++;
	  }while(*(str+k)!='\0');
		
	 while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC)==RESET)
   {}
   }


//使用Printf函数像串口发送信息
	 int fputc(int ch, FILE *f)
	 {
		USART_SendData(USART2, (uint8_t) ch );
		while(USART_GetFlagStatus( USART2 ,USART_FLAG_TXE)==RESET); 
		return ch;
	 }
//使用getchar ， scanf 函数接收串口信息；
	 int fgetc(FILE *f)
	 {
		 while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE )==RESET);
		 return (int)USART_ReceiveData(USART2);
		 
	 }
	 
