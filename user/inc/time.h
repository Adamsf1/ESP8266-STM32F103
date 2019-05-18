#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f10x.h"
#include "uart.h"
#include "StepperMotor.h"
#include "Http.h"
#include "USB.h"
#include "RGB.h"
#include "string.h"
#include "DMA.h"
#include "LED.h"
#include "USB.h"
#include "Motor.h"
#include "DHT11.h"

struct Device_Flag
{
	u8 StepMotor_flag;
	u8 RGB_flag;
	u8 Motor_flag;
	u8 USB_flag;
	
};
extern struct Device_Flag Device_Flag; 
extern struct Http_Data DeviceData;
extern struct Http_Data GetData;
extern u16 KEY_cnt;
void timer2_init(u16 psc,u16 arr);
void timer3_init(u16 psc,u16 arr);
void timer4_init(u16 psc,u16 arr);
#endif
