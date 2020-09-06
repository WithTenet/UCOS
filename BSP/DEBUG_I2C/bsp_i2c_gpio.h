#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>


#define GY_I2C_WR	0		/* 写控制bit */
#define GY_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GY_I2C_GPIO_PORT				GPIOB			/* GPIO端口 */
#define GY_I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOB		/* GPIO端口时钟 */
#define GY_I2C_SCL_PIN					GPIO_Pin_10		/* 连接到SCL时钟线的GPIO */
#define GY_I2C_SDA_PIN					GPIO_Pin_3			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */

#define GY_I2C_SCL_1()  GY_I2C_GPIO_PORT->BSRRL = GY_I2C_SCL_PIN				/* SCL = 1 */
#define GY_I2C_SCL_0()  GY_I2C_GPIO_PORT->BSRRH = GY_I2C_SCL_PIN				/* SCL = 0 */
	
#define GY_I2C_SDA_1()  GY_I2C_GPIO_PORT->BSRRL = GY_I2C_SDA_PIN				/* SDA = 1 */
#define GY_I2C_SDA_0()  GY_I2C_GPIO_PORT->BSRRH = GY_I2C_SDA_PIN				/* SDA = 0 */
	
#define GY_I2C_SDA_READ()  ((GY_I2C_GPIO_PORT->IDR & GY_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif

