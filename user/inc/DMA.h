#ifndef DMA_H
#define DMA_H
#include "stm32f10x.h"
#include "uart.h"

extern u8 DMA1_Transfer_OK;
void DMA1_chanel1_init(void);	
void DMA1_chanel1_Enble(u32 par_addr,u32 m_addr,u32 num);

#endif
