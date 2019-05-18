#ifndef SPI_H
#define SPI_H
#include "stm32f10x.h"
#include "F103io_bit.h"


#define SCLK PBout(13)
#define SDIN PBout(15)
void SPI2_PIN_Init(void);
u8 SPI2_Send_Byte(u8 data);




#endif

