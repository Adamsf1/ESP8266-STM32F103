#include "Adams.h"



#define SSID "ADAMSF 7582"		//WIFI账号
#define PWD  "83407805"				//WIFI密码
#define SEVER "192.168.137.1"	//服务器IP
#define PORT  "8080"					//端口号



int main(void)
{
	
	NVIC_SetPriorityGrouping(7-2);
	LED_init();
	RGB_PIN_Init();
	OLED_Init();
	Motor_init();
	uart1_init(72,115200);
	uart2_init(36,115200);
	timer4_init(720,20);
	timer2_init(7200,10000);
	DMA1_chanel1_init();
	StepMotorInit();
	ESP8266_Init();
	ESP8266_Connect_WIFI((u8*) SSID,(u8*) PWD);
	ESP8266_Connect_Sever((u8*) SEVER,(u8*) PORT);
	ESP8266_CIPSEND();
	USB_Init();
	OLED_Show_Information();
	TIM2->CR1 |=(0x01<<0);//开计数器

	while(1)
	{
		
		

		if(Device_Flag.StepMotor_flag==1)
		{
			Device_Flag.StepMotor_flag=0;
			DeviceData.StepMotor=GetData.StepMotor;
			SteperMotor_Contorl(GetData.StepMotor);
		}
		
		if(Device_Flag.RGB_flag==1)
		{
			Device_Flag.RGB_flag=0;
			DeviceData.RGB_Red=GetData.RGB_Red;
			DeviceData.RGB_Green=GetData.RGB_Green;
			DeviceData.RGB_Blue=GetData.RGB_Blue;
			RGB_Control(DeviceData.RGB_Red,DeviceData.RGB_Green,DeviceData.RGB_Blue);
		}
		if(Device_Flag.Motor_flag==1)
		{
			Device_Flag.Motor_flag=0;
			DeviceData.Motor=GetData.Motor;
			Motor_Control(DeviceData.Motor);
		}
			if(Device_Flag.USB_flag==1)
		{
			Device_Flag.USB_flag=0;
			DeviceData.USB=GetData.USB;
			USB_Contorl(DeviceData.USB);
		}
		
	
	}
}
