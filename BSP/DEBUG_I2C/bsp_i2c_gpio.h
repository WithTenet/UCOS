#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>


#define GY_I2C_WR	0		/* д����bit */
#define GY_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GY_I2C_GPIO_PORT				GPIOB			/* GPIO�˿� */
#define GY_I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define GY_I2C_SCL_PIN					GPIO_Pin_10		/* ���ӵ�SCLʱ���ߵ�GPIO */
#define GY_I2C_SDA_PIN					GPIO_Pin_3			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */

#define GY_I2C_SCL_1()  GY_I2C_GPIO_PORT->BSRRL = GY_I2C_SCL_PIN				/* SCL = 1 */
#define GY_I2C_SCL_0()  GY_I2C_GPIO_PORT->BSRRH = GY_I2C_SCL_PIN				/* SCL = 0 */
	
#define GY_I2C_SDA_1()  GY_I2C_GPIO_PORT->BSRRL = GY_I2C_SDA_PIN				/* SDA = 1 */
#define GY_I2C_SDA_0()  GY_I2C_GPIO_PORT->BSRRH = GY_I2C_SDA_PIN				/* SDA = 0 */
	
#define GY_I2C_SDA_READ()  ((GY_I2C_GPIO_PORT->IDR & GY_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif

