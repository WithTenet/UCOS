
/*
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� i2c_CheckDevice() ���I2C�豸�Ƿ��������ú���������GPIO

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
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	GY_I2C_SDA_0();
	GY_I2C_SCL_1();
	i2c_Delay();
	GY_I2C_SDA_1();
}


void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 GY_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}


uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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

	GY_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (GY_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
	GY_I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	GY_I2C_SCL_0();
	i2c_Delay();
	GY_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

void i2c_NAck(void)
{
	GY_I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	GY_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	GY_I2C_SCL_0();
	i2c_Delay();	
}

static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GY_I2C_GPIO_CLK, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = GY_I2C_SCL_PIN | GY_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	/* ��©��� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GY_I2C_GPIO_PORT, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}


uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* ����GPIO */

	
	i2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte((_Address<<1) | GY_I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}
