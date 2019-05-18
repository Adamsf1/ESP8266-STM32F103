#ifndef USB_H
#define USB_H

#include "stm32f10x.h"
#include "F103io_bit.h"
#include "delay.h"
#include "time.h"


#define KEY1 PAin(0)
#define KEY2 PAin(8)
#define KEY3 PBin(10)
#define	USB_switch PBout(1)
void USB_Init(void);
void USB_Contorl(u8 value);
void key_Init(void);
void USB_Key_Control(void);
#endif
