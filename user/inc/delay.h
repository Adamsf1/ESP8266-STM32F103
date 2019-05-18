#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"  

#define SYSTICK   72
//#define  SYSTICK_INTERRUPT

#ifndef  SYSTICK_INTERRUPT
u8 delay_us(u32 nus);
u8 delay_ms(u16 ms);
#else
void systick_interrupt_init(u16 );
#endif

#endif





























