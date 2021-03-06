
/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO

*/
#include "./i2c/bsp_i2c_gpio.h"



static void i2c_CfgGpio(void);


static void i2c_Delay(void)
{
	uint8_t i;

	
	for (i = 0; i < 70; i++);
}


void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	GY_I2C_SDA_1();
	GY_I2C_SCL_1();
	i2c_Delay();
	GY_I2C_SDA_0();
	i2c_Delay();
	GY_I2C_SCL_0();
	i2c_Delay();
}


void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	GY_I2C_SDA_0();
	GY_I2C_SCL_1();
	i2c_Delay();
	GY_I2C_SDA_1();
}


void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			GY_I2C_SDA_1();
		}
		else
		{
			GY_I2C_SDA_0();
		}
		i2c_Delay();
		GY_I2C_SCL_1();
		i2c_Delay();	
		GY_I2C_SCL_0();
		if (i == 7)
		{
			 GY_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}


uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		GY_I2C_SCL_1();
		i2c_Delay();
		if (GY_I2C_SDA_READ())
		{
			value++;
		}
		GY_I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}


uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	GY_I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (GY_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	GY_I2C_SCL_0();
	i2c_Delay();
	return re;
}

void i2c_Ack(void)
{
	GY_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	GY_I2C_SCL_0();
	i2c_Delay();
	GY_I2C_SDA_1();	/* CPU释放SDA总线 */
}

void i2c_NAck(void)
{
	GY_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	GY_I2C_SCL_0();
	i2c_Delay();	
}

static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GY_I2C_GPIO_CLK, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = GY_I2C_SCL_PIN | GY_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	/* 开漏输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GY_I2C_GPIO_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}


uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* 配置GPIO */

	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte((_Address<<1) | GY_I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

	i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}
