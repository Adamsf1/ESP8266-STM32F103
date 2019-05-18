#include "Http.h"
//0X432321353934424d066cff31  芯片序列号

//#define UniqueID_ADDR_Base 0x1FFFF7E8 //基地址
struct Http_Data Http_Data={0};


u8* Get_Adress(u8* key_word)
{
	u8* Address;
	Address=(u8 *)strstr((const char *)Uart2_Rev.rev_write,(const char *)key_word);
	return Address;
}

//获取芯片ID
//void Get_CoreID(u32 Device_ID[3])
//{
//	
//	Device_ID[0]=*(u32 *)UniqueID_ADDR_Base;  //低字节  
//	Device_ID[1]=*(u32 *)(UniqueID_ADDR_Base+4);
//	Device_ID[2]=*(u32 *)(UniqueID_ADDR_Base+8);//高字节
//	
//}


u32 find_Data(u8 *address)
{
	int t=0;
    while(*address!=';')
			{
        t=t*10+*address-'0';
        address++;
			}
			return t;
}
struct Http_Data Http_Save_Data(void)
{
	u8* address;
	u32 t;
	
	address=Get_Adress((u8*)"RGB_Red:");
	address+=8;
	Http_Data.RGB_Red=find_Data(address);
	

	address=Get_Adress((u8*)"RGB_Green:");
	address+=10;
	Http_Data.RGB_Green=find_Data(address);
	
	address=Get_Adress((u8*)"RGB_Blue:");
	address+=9;
	Http_Data.RGB_Blue=find_Data(address);

	
	address=Get_Adress((u8*)"StepMotor:");
	address+=10;
	Http_Data.StepMotor=find_Data(address);

		//printf("Http_Data.StepMotor:%d\r\n",Http_Data.StepMotor);
	
	address=Get_Adress((u8*)"USB:");
	address+=4;
	Http_Data.USB=find_Data(address);
	
	address=Get_Adress((u8*)"DCMotor:");
	address+=8;
	if((*address)=='+')
	{
		address+=1;
		Http_Data.Motor=find_Data(address);
	}
	else if((*address)=='-')
	{
		address+=1;
		Http_Data.Motor=-find_Data(address);
	}


	return Http_Data;
}



u8 GET_Data(void)
{
	u8* address;
	u8  check,i;
	u8  buf[24];
	uart2_send_string((u8*)"GET /HTML/Adams.html HTTP/1.1\r\nHost: 192.168.137.1\r\nConnection: Keep-Alive\r\nUser-Agent: Adams\r\n\r\n");\
	address=Get_Adress((u8*)"Coding:0X");
	address+=9;
	for(i=0;i<24;i++)
	{
		*(buf+i)=*(address+i);
	}
	check=strcmp((const char*)buf,(const char*)"432321353934424d066cff31");
	
	if(check==0)
	{
	
		//	Http_Data=Http_Save_Data();
		//	printf("Red:%d,Green:%d,blue:%d,StepMotor:%d,USB:%d\r\n",Http_Data.RGB_Red,Http_Data.RGB_Green,Http_Data.RGB_Blue,Http_Data.StepMotor,Http_Data.USB);
			return 1;
	}
	else return 0;
}



void Http_Check_Receive(void)
{
		static u8 n=0;
		if(DMA1_Transfer_OK)//如果DMA传输完成说明服务器有数据发送过来
		{
			DMA1_Transfer_OK=0;
			LED1=1;
			n++;
			OLED_String(0,6,(u8*)"Geting data");
			OLED_Show_ASCII(87+n*7,6,'.');
			if(n==4)
			{
				n=0;
				OLED_String(88,6,(u8*)"    ");
			}
			delay_ms(10);
			LED1=0;		
		}
}



