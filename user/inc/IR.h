#ifndef IR_H
#define IR_H

#include "stm32f10x.h"
#include "F103io_bit.h"


typedef struct 
{
	u8 Headflag;
	u32 data;
	u16 ccr2;
	u8 cnt;
	u8 over;
}IR_para;

#define PA1_IN PAin(1)


void IR_PIN_Init(void);
void TIM2_Init(void);
u8 RemotoScan(void);
#endif
