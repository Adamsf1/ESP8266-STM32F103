#include "stm32f10x.h"
#include "delay.h"

#ifndef  SYSTICK_INTERRUPT

////��ʱnus΢��
////���� nus :��ʱ����΢��
////����ֵ��0 ����  1�쳣(��ǰ��������ʱ)
u8 delay_us(u32 nus)
{
	u32 sta;
	//ѡ��ʱ��Դ---f=SYSCLK/8=21M
	SysTick->CTRL &=~(0x01<<2);
	
	SysTick->LOAD =nus * (SYSTICK/8);  //�趨������ֵ
	SysTick->VAL =0;       //��װ��
	SysTick->CTRL |=0x01<<0;   //��systick��ʱ��
	do{
		sta = SysTick->CTRL;
	}while(((sta&(0x01<<16))==0)&&(sta & (0x01<<0)));  //�ȴ���ʱʱ�䵽(����ʱʱ�䵽���߶�ʱ���رջ�ֱ������ִ��)
	
	SysTick->CTRL &=~(0x01<<0);  //��systick��ʱ��
	if(sta&(0x01<<0))
	{
		return 0;    //������ʱʱ�䵽
	}
	
	return 1;    //�쳣��ʱ
}


////��ʱms΢��
////���� ms :��ʱ����΢��
////����ֵ��0 ����  1�쳣(��ǰ��������ʱ)
u8 delay_ms(u16 ms)
{
	u32 sta;
	//ѡ��ʱ��Դ---f=SYSCLK/8=21M
	SysTick->CTRL &=~(0x01<<2);
	
	SysTick->LOAD =ms*1000*(SYSTICK/8);  //�趨������ֵ
	SysTick->VAL =0;       //��װ��
	SysTick->CTRL |=0x01<<0;   //��systick��ʱ��
	do{
		sta = SysTick->CTRL;
	}while(((sta & (0x01<<16))==0)&&(sta & (0x01<<0)));  //�ȴ���ʱʱ�䵽(����ʱʱ�䵽���߶�ʱ���رջ�ֱ������ִ��)
	
	SysTick->CTRL &=~(0x01<<0);  //��systick��ʱ��
	if(sta&(0x01<<0))
	{
		return 0;    //������ʱʱ�䵽
	}
	
	return 1;    //�쳣��ʱ
}

#else
void systick_interrupt_init(u16 nms)
{
		//ѡ��ʱ��Դ---f=SYSCLK/8=9M 
	SysTick->CTRL &=~(0x01<<2);	
	SysTick->LOAD =nms *1000 * (SYSTICK/8);  //�趨������ֵ
	SysTick->VAL =0;       //��װ��
	
		//��NVIC�м��㡢����EXTI�жϵ����ȼ�
	 NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,3,3));
//	 //ʹ���ж�-----�������ж�û�д����ã�*********************************
//	NVIC_EnableIRQ(SysTick_IRQn);
	
	SysTick->CTRL |=0x01<<1;   //��systick��ʱ���ж�
	SysTick->CTRL |=0x01<<0;   //��systick��ʱ��
	
}

#endif

void SysTick_Handler(void)
{
	
	
}
