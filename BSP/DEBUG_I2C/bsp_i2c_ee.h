#ifndef __I2C_EE_H
#define	__I2C_EE_H

#include "stm32f4xx.h"
#define I2C_INFO(fmt,arg...)           printf("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          printf("<<-I2C-ERROR->> "fmt"\n",##arg)


uint8_t Sensor_Read(uint8_t slave_addr,uint16_t _usAddress, uint16_t _usSize,uint8_t *_pReadBuf );
uint8_t Sensor_Write(uint8_t slave_addr,uint16_t _usAddress, uint16_t _usSize,uint8_t *_pWriteBuf );
void Soft_Dely(uint32_t num);

#endif /* __I2C_EE_H */
