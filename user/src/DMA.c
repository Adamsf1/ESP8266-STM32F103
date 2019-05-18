#include "DMA.h"

void DMA1_chanel1_init(void)	
{
	RCC->AHBENR |=0x01<<0;
	
	DMA1_Channel1->CCR &=~(0x01<<0);//关闭数据流
	while(DMA1_Channel1->CCR &(0x01<<0));//等待数据流关闭成功
	
	DMA1_Channel1->CCR =0;
	DMA1_Channel1->CCR |=0x01<<14;		//存储器到存储器模式
	DMA1_Channel1->CCR |=0x01<<7;  		//存储器地址增量模式
	DMA1_Channel1->CCR |=0x01<<6;  		//外设地址增量模式
	DMA1_Channel1->CCR |=0x01<<1;	//使能传输完成中断
	
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(7-2, 1, 1));//设置优先级
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);//使能中断
}	


void DMA1_chanel1_Enble(u32 par_addr,u32 m_addr,u32 num)
{
	DMA1_Channel1->CCR &=~(0x01<<0);
	while(DMA1_Channel1->CCR&(0x01<<0));
	
	DMA1_Channel1->CPAR =par_addr;
	DMA1_Channel1->CMAR =m_addr;
	DMA1_Channel1->CNDTR=num;
	
	DMA1_Channel1->CCR |=0x01<<0;
	
} 

u8 DMA1_Transfer_OK=0;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA1->ISR&(0x01<<1))//数据流传输完成中断标志
	{
		DMA1->IFCR |=0x01<<1;//清传输完成标志
		while(DMA1->ISR &(0X01<<1));//等待清除完毕
		DMA1_Transfer_OK=1;
		//uart1_send_string(Uart2_Rev.rev_write);
	}

}
