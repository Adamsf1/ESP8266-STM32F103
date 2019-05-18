#ifndef LED_H
#define LED_H
#include "stm32f10x.h"
#include "F103io_bit.h"

void LED_init(void);
#define LED1 PBout(11)


#endif
