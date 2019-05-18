#include "Motor.h"

//PB4 PB5
void Motor_init(void)
{
	RCC->APB2ENR |=0x01<<0;	//AFIOʱ��ʹ��
	RCC->APB2ENR |=0x01<<3; //GPIOBʱ�ӿ���
	RCC->APB1ENR |=0x01<<1; //TIM3ʱ��ʹ��
	
	GPIOB->CRL &=~(0xF<<16);
	GPIOB->CRL |=(0x09<<16);
	GPIOB->CRL &=~(0xF<<20);
	GPIOB->CRL |=(0x09<<20);
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);
	AFIO->MAPR &=~(0x03<<10);
	AFIO->MAPR |=(0x02<<10);
	//TIM3->CR1 =0;
	TIM3->CCMR1 |=(0x06<<4);  //CH1PWMģʽ1,
	TIM3->CCMR1 &=~(0x03<<0);	//CH1����Ϊ���ģʽ
	TIM3->CCMR1 |=(0x06<<12); //CH2PWMģʽ1,
	TIM3->CCMR1 &=~(0x03<<8);	//CH2����Ϊ���ģʽ
	TIM3->CCER |=(0x01<<1);	  //CH1�͵�ƽ��Ч
	TIM3->CCER |=0x01<<0;			//CH1�������
	TIM3->CCER |=(0x01<<5);	  //CH2�͵�ƽ��Ч
	TIM3->CCER |=0x01<<4;			//CH2�������
	//TIM3->CNT =0;  						//����������
	TIM3->PSC =720-1;					//Ԥ��Ƶ
	TIM3->ARR =100-1;					//װ��ֵ
	//3.�ȽϼĴ�����ֵ���ص�����Ӱ�ӼĴ����� 4.������ֵ����
	TIM3->EGR |=0x01<<0;
	
//������±�־,�ȴ�������
	TIM3->SR &=~(0x01<<0);
	while(TIM3->EGR&0x01);
	TIM3->CCR1 =0;
	TIM3->CCR2 =0;
	
	TIM3->CR1 |=(0x01<<0);//ʹ�ܼ�����
	
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
