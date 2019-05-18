#ifndef RGB_H
#define RGB_H
#include "stm32f10x.h"
#include "F103io_bit.h"
#include "delay.h"

void RGB_PIN_Init(void);
void RGB_DATA_Send(u32 RGB_data);
void RGB_Control(u8 b,u8 g,u8 r);
#define RGB_CLK  PBout(8)
#define RGB_DATA PBout(9)
#endif
