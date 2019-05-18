#include "DMA.h"

void DMA1_chanel1_init(void)	
{
	RCC->AHBENR |=0x01<<0;
	
	DMA1_Channel1->CCR &=~(0x01<<0);//�ر�������
	while(DMA1_Channel1->CCR &(0x01<<0));//�ȴ��������رճɹ�
	
	DMA1_Channel1->CCR =0;
	DMA1_Channel1->CCR |=0x01<<14;		//�洢�����洢��ģʽ
	DMA1_Channel1->CCR |=0x01<<7;  		//�洢����ַ����ģʽ
	DMA1_Channel1->CCR |=0x01<<6;  		//�����ַ����ģʽ
	DMA1_Channel1->CCR |=0x01<<1;	//ʹ�ܴ�������ж�
	
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(7-2, 1, 1));//�������ȼ�
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);//ʹ���ж�
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
	if(DMA1->ISR&(0x01<<1))//��������������жϱ�־
	{
		DMA1->IFCR |=0x01<<1;//�崫����ɱ�־
		while(DMA1->ISR &(0X01<<1));//�ȴ�������
		DMA1_Transfer_OK=1;
		//uart1_send_string(Uart2_Rev.rev_write);
	}

}
