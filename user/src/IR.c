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
	TIM2->SMCR &=~(0x01<<14);//��ֹ�ⲿʱ��ģʽ 2
	TIM2->SMCR &=~(0x07<<0);//��ֹ��ģʽ�C�C��� CEN =��1����Ԥ��Ƶ��ʱ��ֱ�����ڲ�ʱ���ṩ
	//���û�����ʱ�����ܣ��ü�����������������
	TIM2->CR1=0;//����CR1
  TIM2->CCMR1 &=~(0x03<<8);
	TIM2->CCMR1 |=(0x01<<8);
	TIM2->ARR=72-1;
	TIM2->PSC=10000-1;
	TIM2->EGR=1;
	TIM2->SR &=~(0x01<<0);//���һ�θ��±�־
	while(TIM2->SR&(0x01<<0));//�ȴ���־������
	TIM2->CR1 &=~(0x03<<8);
	TIM2->CR1 |=(0x02<<8);//fDTS=fCK_INT/4=18Mhz
	TIM2->CCMR1 |=(0x0f<<12); //fSAMPLING=fDTS/32�� N=8
	                         //���벶��2���˳��ĸ��Ų���Ƶ����СֵΪ:fSAMPLING=fDTS/32/8=18Mhz/32/8
	//ͨ��2���ؼ��--������
	TIM2->CCMR1 &=~(0x03<<8);
	TIM2->CCMR1 |=(0x01<<8);//CC2ͨ��������Ϊ���룬 IC2ӳ����TI2��
	
	TIM2->CCMR1 &=~(0x03<<10);//ͨ��2��Ԥ��Ƶ��������������ÿ��⵽һ�����ر�ִ�в���
	
	
	TIM2->DIER |=0x01<<2;//ʹ��CC2�����ж�
	TIM2->DIER |=0x01<<0;//ʹ�ܸ����ж�
	//�������ȼ�����
	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority (7-2, 2,2));
	//NVIC�ж�ʹ��
	NVIC_EnableIRQ(TIM2_IRQn);
	//NVIC_EnableIRQ(TIM2_UP_TIM20_IRQn);
	
	TIM2->CCER |=0x01<<4;//ʹ��CC2����
	TIM2->CR1 |=0x01<<0;//CEN =��1��,����������

	
}



IR_para IR;
//void TIM2_IRQHandler(void)
//{

//	if(TIM2->SR&(0x01<<0))//�����¼�
//	{
//		TIM2->SR &=~(0x01<<0);//�����־
//		while(TIM2->SR&(0x01<<0));//�ȴ���־������
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
//		TIM2->SR &=~(0x01<<2);//�����־
//		while(TIM2->SR&(0x01<<2));//�ȴ���־������
//		
//		if(PA1_IN)
//		{
//			TIM2->CNT=0;//����������
//			TIM2->CCER |=(0x01<<5);//�л����½��ؼ��
//				
//		}
//		else 
//		{
//			IR.ccr2=TIM2->CCR2;
//			TIM2->CCER &=~(0x01<<5);//�����ز���
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
