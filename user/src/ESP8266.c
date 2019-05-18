#include "ESP8266.h"

u8 AT_SEND(u8* AT_cmd,u8*reply)
{
	u16 count=0;
	uart2_send_string(AT_cmd);
	
	while(1)
	{
		while(!DMA1_Transfer_OK)
		{
			
			if(++count>=500)
			{
				return 1;//超时或者没有期待的响应
			}
			delay_ms(1);
		}
		DMA1_Transfer_OK=0;
		
		if(strstr((const char *)Uart2_Rev.rev_write,(const char *)reply)!=NULL)
		{
			return 0;
		}
	}
}


u8 ESP8266_Connect_WIFI(u8* ssid,u8* pwd)
{
	u8 WIFI_Buf[100]={"AT+CWJAP_DEF="};
	AT_SEND((u8 *)"AT+CWQAP\r\n",(u8 *)"OK");//断开AP连接
	
	strcat((char *)WIFI_Buf,"\"");
	strcat((char *)WIFI_Buf,(const char *)ssid);
	strcat((char *)WIFI_Buf,"\",\"");
	strcat((char *)WIFI_Buf,(const char *)pwd);
	strcat((char *)WIFI_Buf,"\"\r\n");

  uart2_send_string((u8*)WIFI_Buf);	
	
	while(1)
	{
		while(!DMA1_Transfer_OK);//等待传输完成标志
		DMA1_Transfer_OK=0;//清标志
		if(NULL!=strstr((const char *)Uart2_Rev.rev_write,(const char *)"OK"))
		{
			
			OLED_String(0,2,(u8*)"WIFI Connected!");
			printf("WIFI Connected\r\n");
			return 0;
		}
		else 
		{
			OLED_String(0,2,(u8*)"WIFI Connecting");

		}
	}
}

u8 ESP8266_Connect_Sever(u8* sever,u8* port)
{
	u8 TCP_Buf[100]={"AT+CIPSTART="};
	strcat((char *)TCP_Buf,"\"TCP\",\"");
	strcat((char *)TCP_Buf,(const char *)sever);
	strcat((char *)TCP_Buf,"\",");
	strcat((char *)TCP_Buf,(const char *)port);
	strcat((char *)TCP_Buf,"\r\n");
	
  uart2_send_string((u8*)TCP_Buf);
	
	while(1)
	{	
		
		while(!DMA1_Transfer_OK);
		DMA1_Transfer_OK=0;
		if(NULL!=strstr((const char *)Uart2_Rev.rev_write,(const char *)"OK"))
		{
			printf("TCP Connected\r\n");
			OLED_String(0,4,(u8*)"TCP Connected!");
			return 0;
		}
		else 
		{	
			OLED_String(0,4,(u8*)"TCP Connecting");

		}
	}
}

void ESP8266_Exit_PsaaThrough_Mode(void)
{
	uart2_send_string((u8*)"+++");
}





u8 ESP8266_Restore(void)
{
	uart2_send_string((u8*)"+++");
	uart2_send_string((u8*)"AT+RESTORE\r\n");	
	while(1)
	{
		while(!DMA1_Transfer_OK);
		DMA1_Transfer_OK=0;
	
		if(strstr((const char*)Uart2_Rev.rev_write,(const char*)"ready")!=NULL)
		{
			OLED_Clear();
			printf("ESP8266 RESTORE OK\r\n");
			OLED_String(0,0,(u8*)"Restore OK");
			return 0;
		}
		else OLED_String(0,0,(u8*)"Restore Error");
	}
}
u8 ESP8266_Init(void)
{
	u8 step;
	ESP8266_Exit_PsaaThrough_Mode();//退出穿透模式
	delay_ms(100);
	ESP8266_Restore();
	delay_ms(500);
	
	step= AT_SEND((u8 *)"AT+GMR\r\n",(u8 *)"OK");//查询版本信息
	if(step)
	{
		printf("AT+GMR error\r\n");
		return 2;
	}
	
	
	step=AT_SEND((u8*)"AT+CWMODE_DEF=1\r\n",(u8*)"OK");//设置WIFI模组的工作模式
	if(step)
	{
		printf("AT+CIPMODE=1 ERROR");
		return 3;
	}

	
	step=AT_SEND((u8*)"AT+CIPMODE=1\r\n",(u8*)"OK");//设置透传模式
	if(step)
	{
		printf("AT+CIPMODE=1 error\r\n");
		return 4;
	}
	
	
	printf("HTTP Config Successed\r\n");

	return 0;
	
}

void ESP8266_CIPSEND(void)
{
	u8 step;
	step=AT_SEND((u8 *)"AT+CIPSEND\r\n",(u8 *)"OK");//启动穿透模式
	if(!step )
	{
		printf("\r\nYou can GET or POST with server now\r\n\r\n\r\n");
		OLED_String(0,6,(u8*)"CIPSEND OK");
	}
	else OLED_String(0,6,(u8*)"CIPSEND Error");
}

