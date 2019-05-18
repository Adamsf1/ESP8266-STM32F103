#ifndef DHT11_H
#define DHT11_H
#include "stm32f10x.h"
#include "F103io_bit.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "stm32f10x_gpio.h"

#define DHT11_IN  PBin(3)
#define DHT11_OUT PBout(3)

void DHT11_PIN_InputMode(void);
void DHT11_PIN_OutputMode(void);
void DHT11_PIN(void);
u16 DHT11_Rec_Data(u8 DHT11_DATA[5]);
double DHT11_Hum(void);
double DHT11_Temp(void);
void Dht11_Data_transform(u8* dht);
#endif
