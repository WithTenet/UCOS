
#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"


//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART2



#define DEBUG_USART_CLK                         RCC_APB1Periph_USART2
#define DEBUG_USART_BAUDRATE                    9600//串口波特率

#define DEBUG_USART_RX_GPIO_PORT                GPIOD
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define DEBUG_USART_RX_PIN                      GPIO_Pin_6
#define DEBUG_USART_RX_AF                       GPIO_AF_USART2
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource6

#define DEBUG_USART_TX_GPIO_PORT                GPIOD
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define DEBUG_USART_TX_PIN                      GPIO_Pin_5
#define DEBUG_USART_TX_AF                       GPIO_AF_USART2
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource5

#define DEBUG_USART_IRQHandler                  USART2_IRQHandler
#define DEBUG_USART_IRQ                 				USART2_IRQn

  
void Debug_USART_Config(void);
void USART_SendByte (USART_TypeDef * pUSARTx ,char ch );
void USART_SendString(USART_TypeDef * pUSARTx, char *str);

void DEBUG_USART_IRQHandler(void) ;

#endif /* __BSP_USART_H */

