#include "StepperMotor.h"

/*IN1 PA7
	IN2 PA6
	IN3	PA5
	IN4 PA4
*/

u8 MOTOR_MODE;
u8 MOTOR_NUM;


void timer1_init(void)
{
  RCC->APB2ENR |= 0x01<<11;//��ʹ�ܶ�ʱ��ʱ��Դ  ��6��
	
	TIM1->PSC = 10-1;  //  ����ʱ�� = ʱ��Դ / PSC + 1    ���÷�Ƶֵ
	TIM1->ARR = 7200-1;  //������װ��ֵ
	
	TIM1->DIER |= 1;//ʹ���ж�--����
	NVIC_SetPriority(TIM1_UP_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM1_UP_IRQn);//�ں��ж�ʹ��
	
	TIM1->CR1 &=~(0x01<<0);//�ȹر� ������
}






void StepMotorInit(void)
{
	RCC->APB2ENR |=0x02;
	
	GPIOA->CRL &=~(0xFFFF<<16);
	GPIOA->CRL |=(0x3333<<16);
	timer1_init( );
}

void Save_Data(s16 data)
{
	//printf("stepMotor value:%d\r\n",*(s16 *)(0x0801FC00));
	FLASH->KEYR=0x45670123;//����
	FLASH->KEYR=0xCDEF89AB;
	FLASH->CR &=~(0x03<<8);
	while(FLASH->SR&(0x01<<0));//ȷ�ϵ�ǰδִ���κ� Flash ����
	FLASH->CR |=0x01<<1;//����ҳ����
	FLASH->AR =(u16)0x0801FC00;
	FLASH->CR |=0x01<<6;//����һ�β�������
	while(FLASH->SR&(0x01<<0));//�ȴ��������
	FLASH->CR &=~(0x01<<1);//ȡ��ҳ�������������д��ʧ��
	
	FLASH->CR |=0x01<<0;//���� Flash ���
	*(s16 *)0x0801FC00=data; 

	FLASH->CR &=~(0x01<<0);//ȡ��FLASH���
	FLASH->CR |=0x01<<7;//����	
//	printf("stepMotor now:%d\r\n",*(s16 *)(0x0801FC00));

}


static s16	Value;

/*
���½��ĺ���
����˵����LEFT_MODE/RIGHT_MODE
*/
void RunStepMotor(u8 run_mode)
{
	
	static u8 cnt=0;
	
	cnt++;
	if(RIGHT_MODE==run_mode)//������  12341234....
	{
		switch(cnt)
		{
			case 1:BEAT1;break;
			case 2:BEAT2; break;
			case 3:BEAT3;; break;
			case 4:BEAT4; break;
			default :break;
		}
	}
	else if(LEFT_MODE==run_mode)//��������   43214321....
	{
		switch(cnt)
		{
			case 1:BEAT4; break;
			case 2:BEAT3; break;
			case 3:BEAT2; break;
			case 4:BEAT1; break;
			default :break;
		}
		
	}

	cnt=cnt%4;
		
}

void StopStepMotor(void)
{
	A=0;A_=0;B=0;B_=0;
}



void TIM1_UP_IRQHandler(void)
{
	static u16 cnt=0;
	TIM1->SR &=~(0X01<<0); //������±�־
	while(TIM1->SR&0x01);  //�ȴ������¼���־���
	
	RunStepMotor(MOTOR_MODE);
	
	if(++cnt>=80*MOTOR_NUM)
	{
		cnt=0;
		StopStepMotor();
		Save_Data(Value);
		TIM1->CR1 &=~(0x01<<0);//�ȹر� ������
	}
	
}


/*
��������
����˵����range -10~10  ����0������range  С��0������-range
*/


void WindowControl(s16 range)
{
	if(range>0)
	{
		MOTOR_MODE=RIGHT_MODE ;
    MOTOR_NUM=range;
		TIM1->CR1 |= (0x01<<0);//�����������
	}
	else if(range<0)
	{
		MOTOR_MODE=LEFT_MODE ;
    MOTOR_NUM=-range;
		TIM1->CR1 |= (0x01<<0);//�����������
	}
	
}

void SteperMotor_Contorl(s16 target)
{
	if(target!=*DHT_Flash)
	{
		Value=target;
		WindowControl(target-*DHT_Flash);
	
	}

}

