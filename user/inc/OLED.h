#ifndef OLED_H
#define OLED_H
#include "stm32f10x.h"
#include "F103io_bit.h"
#include "SPI.h"
#include "delay.h"
#include "font.h"
#include "string.h"
#include "stdio.h"
//OLED_SCLK----SCK----PB13----普通功能推挽输出
//OLED_SDIN---MOSI----PB15---普通功能推挽输出
//OLED_CS----PB7----普通功能推挽输出
//OLED_RES---PB6----普通功能推挽输出
//OLED_D/C---PB12----普通功能推挽输出

#define CS PBout(7)
#define RES PBout(6)
#define DC PBout(12)

void OLED_PIN_Init(void);
void OLED_Init(void);
void OLED_Show_ASCII(u8 cloumn,u8 page,u8 ch);
void OLED_Show_Word(u8 cloumn,u8 page,u8* word);
void OLED_String(u8 cloumn,u8 page,u8 *ch);
void OLED_Show_Temperature(u8 x,u8 page,u8 buf[5]);
void OLED_Show_Humidity(u8 x,u8 page,u8 buf[5]);
void OLED_Show_Information(void);
void OLED_Clear(void);
#endif
