#include "stm32f10x.h"
#include "delay.h"

#ifndef  SYSTICK_INTERRUPT

////延时nus微秒
////参数 nus :延时多少微秒
////返回值：0 正常  1异常(提前结束了延时)
u8 delay_us(u32 nus)
{
	u32 sta;
	//选择时钟源---f=SYSCLK/8=21M
	SysTick->CTRL &=~(0x01<<2);
	
	SysTick->LOAD =nus * (SYSTICK/8);  //设定计数初值
	SysTick->VAL =0;       //重装载
	SysTick->CTRL |=0x01<<0;   //开systick定时器
	do{
		sta = SysTick->CTRL;
	}while(((sta&(0x01<<16))==0)&&(sta & (0x01<<0)));  //等待定时时间到(当定时时间到或者定时被关闭会直接往下执行)
	
	SysTick->CTRL &=~(0x01<<0);  //关systick定时器
	if(sta&(0x01<<0))
	{
		return 0;    //正常延时时间到
	}
	
	return 1;    //异常延时
}


////延时ms微秒
////参数 ms :延时多少微秒
////返回值：0 正常  1异常(提前结束了延时)
u8 delay_ms(u16 ms)
{
	u32 sta;
	//选择时钟源---f=SYSCLK/8=21M
	SysTick->CTRL &=~(0x01<<2);
	
	SysTick->LOAD =ms*1000*(SYSTICK/8);  //设定计数初值
	SysTick->VAL =0;       //重装载
	SysTick->CTRL |=0x01<<0;   //开systick定时器
	do{
		sta = SysTick->CTRL;
	}while(((sta & (0x01<<16))==0)&&(sta & (0x01<<0)));  //等待定时时间到(当定时时间到或者定时被关闭会直接往下执行)
	
	SysTick->CTRL &=~(0x01<<0);  //关systick定时器
	if(sta&(0x01<<0))
	{
		return 0;    //正常延时时间到
	}
	
	return 1;    //异常延时
}

#else
void systick_interrupt_init(u16 nms)
{
		//选择时钟源---f=SYSCLK/8=9M 
	SysTick->CTRL &=~(0x01<<2);	
	SysTick->LOAD =nms *1000 * (SYSTICK/8);  //设定计数初值
	SysTick->VAL =0;       //重装载
	
		//在NVIC中计算、设置EXTI中断的优先级
	 NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,3,3));
//	 //使能中断-----（核心中断没有此设置）*********************************
//	NVIC_EnableIRQ(SysTick_IRQn);
	
	SysTick->CTRL |=0x01<<1;   //开systick定时器中断
	SysTick->CTRL |=0x01<<0;   //开systick定时器
	
}

#endif

void SysTick_Handler(void)
{
	
	
}
