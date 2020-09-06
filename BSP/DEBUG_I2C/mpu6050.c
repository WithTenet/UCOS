#include "mpu6050.h"
#include "bsp_bluetooth.h"
#include "./ bsp_i2c_ee.h"

void MPU6050_Init(void){
    int  i=0,j=0;
    uint8_t data;
    for(i=0;i<1000;i++)
    {
    for(j=0;j<1000;j++)
    {
      ;
    }
    }
    //唤醒MPU6050
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_PWR_MGMT_1,1,&data);
    
    //写入采样率
    data=0x07;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_SMPLRT_DIV,1,&data); 
    //MPU配置
    data=0x06;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_CONFIG,1,&data);
    //加速度计配置   16G
    data=0x08;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_ACCEL_CONFIG,1,&data);
    //陀螺仪配置   2000
    data=0x18;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_GYRO_CONFIG,1,&data);
    //I2C_MST关
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_USER_CTRL,1,&data);  
    //打开旁通模式
    data=0x02;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_INT_PIN_CFG,1,&data);
    
}

uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    Sensor_Read(MPU6050_ADDRESS,MPU6050_RA_WHO_AM_I,1,&Re);    //读器件地址
    
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\nCan't find MPU6050,checking the conector!");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
		return 1;
	}		
}

void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    Sensor_Read(MPU6050_ADDRESS,MPU6050_ACC_OUT,6, buf);//读取加速度六字节数据
    accData[0] = (buf[0] << 8) | buf[1];   //x高八位右移和第八位共同组成16位数据
    accData[1] = (buf[2] << 8) | buf[3];   //y
    accData[2] = (buf[4] << 8) | buf[5];   //z
}

void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    Sensor_Read(MPU6050_ADDRESS,MPU6050_GYRO_OUT,6,buf); //读取陀螺仪六字节数据
    gyroData[0] = (buf[0] << 8) | buf[1];           //x
    gyroData[1] = (buf[2] << 8) | buf[3];           //y 
    gyroData[2] = (buf[4] << 8) | buf[5];           //z
}


void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	Sensor_Read(MPU6050_ADDRESS,MPU6050_RA_TEMP_OUT_H,2,buf);     //读取温度值高8位与低8位共两字节
    temp3= (buf[0] << 8) | buf[1];	//高8位低8位组合成16位数据
	*Temperature=((double) temp3/340.0)+36.53;  //根据手册计算公式得温度

}

