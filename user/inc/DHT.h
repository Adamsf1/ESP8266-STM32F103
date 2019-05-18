#ifndef __DHT_H
#define	__DHT_H
#include "stm32f10x.h"
#include "F103io_bit.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"


u8 DHT11_Read(u8 buf[5]);

void readDHT11(u8 Data_Buff[5]);
#endif /* __DHT11_H */

