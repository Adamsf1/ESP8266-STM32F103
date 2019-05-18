#include "StepperMotor.h"

/*IN1 PA7
	IN2 PA6
	IN3	PA5
	IN4 PA4
*/

void TIM1_Init(void)
{
	RCC->APB2ENR |=0x01<<11;
	
//	TIM1->CR1 |=0x01<<7;
//	TIM1->CR1 |=0x01<<3;
	TIM1->PSC=10-1;
	TIM1->ARR=7200-1;
	TIM1->DIER |=0x01<<0;//��������ж�
	NVIC_SetPriority(TIM1_UP_IRQn,NVIC_EncodePriority(7-2,2,2));
	NVIC_EnableIRQ(TIM1_UP_IRQn);//�ں��ж�ʹ��
	TIM1->CR1 &=~(0X01<<0);//�رռ�����
}

void StepperMotor_Init(void)
{
	RCC->APB2ENR |=0x01<<2;
	
	GPIOA->CRL &=~(0x0f<<16);
	GPIOA->CRL |=0x03<<16;		//PA4
	GPIOA->CRL &=~(0x0f<<20);
	GPIOA->CRL |=0x03<<20;		//PA5
	GPIOA->CRL &=~(0x0f<<24);
	GPIOA->CRL |=0x03<<24;		//PA6
	GPIOA->CRL &=~(0x0f<<28);
	GPIOA->CRL |=0x03<<28;		//PA7
	TIM1_Init();
}



void StopStepMotor(void)
{
	IN_1=0;IN_2=0;IN_3=0;IN_4=0;
}

static s16 cnt;
static u16 direction;
static u16 beat;
static u16 count;
static u8	Value;

void TIM1_UP_IRQHandler(void)
{
	
	TIM1->SR &=~(0x01<<0);
	while(TIM1->SR &(0x01<<0));
	
	if(direction==0)
	{
		switch(beat)
		{
			case 1:Beat1;	beat=2;break;
			case 2:Beat2;	beat=3;break;
			case 3:Beat3;	beat=4;break;
			case 4:Beat4;	beat=1;break;
			default:break;
		}
		
	}
	else if(direction==1)
	{
		switch(beat)
		{
			case 1:Beat4;	beat=2;break;
			case 2:Beat3;	beat=3;break;
			case 3:Beat2;	beat=4;break;
			case 4:Beat1;	beat=1;break;
			default:break;
		}
		
	}
	
	if(++count>=cnt)
	{
		count=0;
		StopStepMotor();
		Save_Data(Value);
		
		TIM1->CR1 &=~(0X01<<0);//�رռ�����
		
	}
	
}


void StepperMotor(s16 n)
{
	if(n<0)
	{
		cnt=-(n*80);
		direction=1;
	}
	else 
	{
		cnt=n*80;
		direction=0;
	}
		beat=1;
		TIM1->CR1 |=0X01<<0;//����������	
}

void Save_Data(s16 data)
{
	printf("stepMotor value:%d\r\n",*(s16 *)(0x0801FC00));
	FLASH->KEYR=0x45670123;//����
	FLASH->KEYR=0xCDEF89AB;
	while(FLASH->SR&(0x01<<0));//ȷ�ϵ�ǰδִ���κ� Flash ����
	FLASH->CR |=0x01<<1;//����ҳ����
	FLASH->AR =(u32)0x0801FC00;
	FLASH->CR |=0x01<<6;//����һ�β�������
	while(FLASH->SR&(0x01<<0));//�ȴ��������
	FLASH->CR &=~(0x01<<1);//ȡ��ҳ�������������д��ʧ��
	
	FLASH->CR |=0x01<<0;//���� Flash ���
	*(s16 *)0x0801FC00=data; 

	FLASH->CR &=~(0x01<<0);//ȡ��FLASH���
	FLASH->CR |=0x01<<7;//����	
	printf("stepMotor now:%d\r\n",*(s16 *)(0x0801FC00));

}


void SteperMotor_Contorl(u16 n)
{
	if(n>0)
	{
		Value=n;
		s16 Data;	
		Data=*((s16 *)(0x0801FC00));
		StepperMotor(n-Data);
	}

}


