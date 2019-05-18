#ifndef HTTP_H
#define HTTP_H
#include "stm32f10x.h"
#include "uart.h"
#include "time.h"
#include "delay.h"
#include "stdio.h"
#include "LED.h"
#include "string.h"
#include "DMA.h"
#include "OLED.h"

struct Http_Data
{
	u8 	RGB_Red;
	u8  RGB_Green;
	u8  RGB_Blue;
	s16  StepMotor;
	u8  USB;
	s8  Motor;
	
};
struct Http_Data Http_Save_Data(void);
u8* Get_Adress(u8* key_word);
extern struct Http_Data Http_Data;
void Update_event(void);
u8 HTTP_GET_Coding(void);
void Http_Check_Receive(void);
u8 GET_Data(void);
#endif

