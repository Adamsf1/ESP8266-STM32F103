#include "IR.H"

void IR_PIN_Init(void)
{
	RCC->APB2ENR |=0x01<<2;
	
	GPIOA->CRL &=~(0x0f<<4);
	GPIOA->CRL |=(0x04<<4);
	
	
	
	
}
void IR_Init(void)
{
	IR_PIN_Init();
	RCC->APB1ENR |=0x01<<0;
	TIM2->SMCR &=~(0x01<<14);//禁止外部时钟模式 2
	TIM2->SMCR &=~(0x07<<0);//禁止从模式––如果 CEN =“1”，预分频器时钟直接由内部时钟提供
	//配置基本定时器功能（让计数器能正常工作）
	TIM2->CR1=0;//配置CR1
  TIM2->CCMR1 &=~(0x03<<8);
	TIM2->CCMR1 |=(0x01<<8);
	TIM2->ARR=72-1;
	TIM2->PSC=10000-1;
	TIM2->EGR=1;
	TIM2->SR &=~(0x01<<0);//清除一次更新标志
	while(TIM2->SR&(0x01<<0));//等待标志清除完毕
	TIM2->CR1 &=~(0x03<<8);
	TIM2->CR1 |=(0x02<<8);//fDTS=fCK_INT/4=18Mhz
	TIM2->CCMR1 |=(0x0f<<12); //fSAMPLING=fDTS/32， N=8
	                         //输入捕获2能滤除的干扰波的频率最小值为:fSAMPLING=fDTS/32/8=18Mhz/32/8
	//通道2边沿检测--上升沿
	TIM2->CCMR1 &=~(0x03<<8);
	TIM2->CCMR1 |=(0x01<<8);//CC2通道被配置为输入， IC2映射在TI2上
	
	TIM2->CCMR1 &=~(0x03<<10);//通道2无预分频器，捕获输入上每检测到一个边沿便执行捕获
	
	
	TIM2->DIER |=0x01<<2;//使能CC2捕获中断
	TIM2->DIER |=0x01<<0;//使能更新中断
	//设置优先级分组
	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority (7-2, 2,2));
	//NVIC中断使能
	NVIC_EnableIRQ(TIM2_IRQn);
	//NVIC_EnableIRQ(TIM2_UP_TIM20_IRQn);
	
	TIM2->CCER |=0x01<<4;//使能CC2捕获
	TIM2->CR1 |=0x01<<0;//CEN =“1”,开启计数器

	
}



IR_para IR;
//void TIM2_IRQHandler(void)
//{

//	if(TIM2->SR&(0x01<<0))//更新事件
//	{
//		TIM2->SR &=~(0x01<<0);//清除标志
//		while(TIM2->SR&(0x01<<0));//等待标志清除完毕
//		if(IR.Headflag==1)
//		{
//			IR.Headflag=0;
//			IR.over=1;
//		}
//		
//	}
//	
//	if(TIM2->SR&(0x01<<2))
//	{
//		TIM2->SR &=~(0x01<<2);//清除标志
//		while(TIM2->SR&(0x01<<2));//等待标志清除完毕
//		
//		if(PA1_IN)
//		{
//			TIM2->CNT=0;//计数器清零
//			TIM2->CCER |=(0x01<<5);//切换成下降沿检测
//				
//		}
//		else 
//		{
//			IR.ccr2=TIM2->CCR2;
//			TIM2->CCER &=~(0x01<<5);//上升沿捕获
//			if(IR.Headflag)
//			{
//				if(IR.ccr2>=300&&IR.ccr2<=800)
//				{
//					IR.data>>=1;
//					
//				}
//				else if(IR.ccr2>=1400&&IR.ccr2<=1800)
//				{
//					IR.data>>=1;
//					IR.data |=0x01<<31;
//					
//				}
//				else if(IR.ccr2>=2300&&IR.ccr2<=2700)
//				{
//					IR.cnt++;
//				}
//			}
//			else 
//			{
//				if(IR.ccr2>=4200&&IR.ccr2<=4800)
//				IR.Headflag=1;
//				IR.cnt=0;
//			}		
//		}
//	}
//}

u8 RemotoScan(void)
{
	u8 Addrcode,NonAddrcode;
	u8 Controlcode,NonControlcode;
	
	if(IR.over)
	{
		IR.over=0;
		Addrcode=(IR.data&0x000000FF)>>0;
		NonAddrcode=(IR.data&0x0000FF00)>>8;
		
		Controlcode=(IR.data&0x00FF0000)>>16;
		NonControlcode=(IR.data&0xFF000000)>>24;
		if(Controlcode==~NonControlcode)
		{
			return Controlcode;
		}
	}
	return 0;
}
