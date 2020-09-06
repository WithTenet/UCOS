#include "apps.h"

extern int ticks;
void App_Task0(void *p_arg){
	OS_CPU_SysTickInit();	
	while(1)
	{
		OSTimeDly(200);
		printf("hello,Task2,Ticks:%d\n",ticks);
		OSTimeDly(200);
	}
}
void App_Task1(void *p_arg){
	OS_CPU_SysTickInit();	
	while(1)
	{
		OSTimeDly(400);
		printf("hello,Task1,Ticks:%d\n",ticks);
		OSTimeDly(400);
	}
}
void App_Task2(void *p_arg){
	int i=0;
	OS_CPU_SysTickInit();
	while(1){
		OSTimeDly(100);
		LED_ON;
		printf("LED ON %d times,Ticks:%d\n",i+1,ticks);
		OSTimeDly(100);
		LED_OFF;
		i++;
	}
}

void BSP_Init(void){
	HSE_SetSysClock(25, 336, 2, 7);
	LED_Config();
	Debug_USART_Config();
	GY_Config();
	MPU6050_Init();
	MPU6050ReadID();
}




