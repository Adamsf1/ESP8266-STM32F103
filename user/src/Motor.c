#include "Motor.h"

//PB4 PB5
void Motor_init(void)
{
	RCC->APB2ENR |=0x01<<0;	//AFIO时钟使能
	RCC->APB2ENR |=0x01<<3; //GPIOB时钟开启
	RCC->APB1ENR |=0x01<<1; //TIM3时钟使能
	
	GPIOB->CRL &=~(0xF<<16);
	GPIOB->CRL |=(0x09<<16);
	GPIOB->CRL &=~(0xF<<20);
	GPIOB->CRL |=(0x09<<20);
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);
	AFIO->MAPR &=~(0x03<<10);
	AFIO->MAPR |=(0x02<<10);
	//TIM3->CR1 =0;
	TIM3->CCMR1 |=(0x06<<4);  //CH1PWM模式1,
	TIM3->CCMR1 &=~(0x03<<0);	//CH1设置为输出模式
	TIM3->CCMR1 |=(0x06<<12); //CH2PWM模式1,
	TIM3->CCMR1 &=~(0x03<<8);	//CH2设置为输出模式
	TIM3->CCER |=(0x01<<1);	  //CH1低电平有效
	TIM3->CCER |=0x01<<0;			//CH1开启输出
	TIM3->CCER |=(0x01<<5);	  //CH2低电平有效
	TIM3->CCER |=0x01<<4;			//CH2开启输出
	//TIM3->CNT =0;  						//计数器清零
	TIM3->PSC =720-1;					//预分频
	TIM3->ARR =100-1;					//装载值
	//3.比较寄存器的值加载到它的影子寄存器中 4.计数器值清零
	TIM3->EGR |=0x01<<0;
	
//清除更新标志,等待清除完毕
	TIM3->SR &=~(0x01<<0);
	while(TIM3->EGR&0x01);
	TIM3->CCR1 =0;
	TIM3->CCR2 =0;
	
	TIM3->CR1 |=(0x01<<0);//使能计数器
	
}
void Motor_Control(s8 speed)
{
	if(speed==0)
	{
		TIM3->CCR2=0;
		TIM3->CCR1=0;
	}
	if(speed>0)
	{
		TIM3->CCR2=0;
		TIM3->CCR1=speed-1;
	}
	if(speed<0)
	{
		TIM3->CCR1=0;
		TIM3->CCR2=-speed-1;
	}
}
