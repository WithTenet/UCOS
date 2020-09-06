

#include "./i2c/bsp_i2c_ee.h"
#include "./i2c/bsp_i2c_gpio.h"
#include "bsp_debug_usart.h"

uint8_t Sensor_Read(uint8_t slave_addr,uint16_t _usAddress, uint16_t _usSize,uint8_t *_pReadBuf)
{
	uint16_t i;
	
	/* 采用串行GY随即读取指令序列，连续读取若干字节 */
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((slave_addr<<1) | GY_I2C_WR);	/* 此处是写指令 */
	 
	/* 第3步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* GY器件无应答 */
	}

	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	i2c_SendByte((uint8_t)_usAddress);
	
	/* 第5步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* GY器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。前面的代码的目的向GY传送地址，下面开始读取数据 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((slave_addr<<1) | GY_I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* GY器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: ee_WriteBytes
*	功能说明: 向串行GY指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t Sensor_Write(uint8_t slave_addr,uint16_t _usAddress, uint16_t _usSize,uint8_t *_pWriteBuf)
{
	uint16_t i;	
    i2c_Start(); 
	i2c_SendByte((slave_addr<<1)|GY_I2C_WR);
    if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* GY器件无应答 */
	}
	
    i2c_SendByte(_usAddress);
   	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* GY器件无应答 */
	}		
	for(i=0;i<_usSize;i++)
	{
		i2c_SendByte(_pWriteBuf[i]);	
		 if (i2c_WaitAck() != 0)
        {
            goto cmd_fail;	/* GY器件无应答 */
        }	
	}    
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

void Soft_Dely(uint32_t num){
    while(num--);
}
