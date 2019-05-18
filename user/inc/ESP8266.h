#ifndef ESP8266_H
#define ESP8266_H
#include "stm32f10x.h"
#include "uart.h"
#include "DMA.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"
#include "font.h"
#include "OLED.h"


u8 AT_SEND(u8* AT,u8 *reply);
u8 ESP8266_Connect_WIFI(u8* ssid,u8* pwd);
u8 ESP8266_Connect_Sever(u8* sever,u8* port);
u8 ESP8266_Restore(void);
void ESP8266_Exit_PsaaThrough_Mode(void);
u8 ESP8266_Init(void);
void ESP8266_CIPSEND(void);


#endif


