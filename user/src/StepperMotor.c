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
  RCC->APB2ENR |= 0x01<<11;//①使能定时器时钟源  第6章
	
	TIM1->PSC = 10-1;  //  计数时钟 = 时钟源 / PSC + 1    设置分频值
	TIM1->ARR = 7200-1;  //设置重装载值
	
	TIM1->DIER |= 1;//使能中断--外设
	NVIC_SetPriority(TIM1_UP_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM1_UP_IRQn);//内核中断使能
	
	TIM1->CR1 &=~(0x01<<0);//先关闭 计数器
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
	FLASH->KEYR=0x45670123;//解锁
	FLASH->KEYR=0xCDEF89AB;
	FLASH->CR &=~(0x03<<8);
	while(FLASH->SR&(0x01<<0));//确认当前未执行任何 Flash 操作
	FLASH->CR |=0x01<<1;//激活页擦除
	FLASH->AR =(u16)0x0801FC00;
	FLASH->CR |=0x01<<6;//触发一次擦除操作
	while(FLASH->SR&(0x01<<0));//等待擦除完毕
	FLASH->CR &=~(0x01<<1);//取消页擦除，不加这句写入失败
	
	FLASH->CR |=0x01<<0;//激活 Flash 编程
	*(s16 *)0x0801FC00=data; 

	FLASH->CR &=~(0x01<<0);//取消FLASH编程
	FLASH->CR |=0x01<<7;//加锁	
//	printf("stepMotor now:%d\r\n",*(s16 *)(0x0801FC00));

}


static s16	Value;

/*
更新节拍函数
参数说明：LEFT_MODE/RIGHT_MODE
*/
void RunStepMotor(u8 run_mode)
{
	
	static u8 cnt=0;
	
	cnt++;
	if(RIGHT_MODE==run_mode)//从左到右  12341234....
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
	else if(LEFT_MODE==run_mode)//从右往左   43214321....
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
	TIM1->SR &=~(0X01<<0); //清除更新标志
	while(TIM1->SR&0x01);  //等待更新事件标志清除
	
	RunStepMotor(MOTOR_MODE);
	
	if(++cnt>=80*MOTOR_NUM)
	{
		cnt=0;
		StopStepMotor();
		Save_Data(Value);
		TIM1->CR1 &=~(0x01<<0);//先关闭 计数器
	}
	
}


/*
窗帘控制
参数说明：range -10~10  大于0往右跑range  小于0往左跑-range
*/


void WindowControl(s16 range)
{
	if(range>0)
	{
		MOTOR_MODE=RIGHT_MODE ;
    MOTOR_NUM=range;
		TIM1->CR1 |= (0x01<<0);//开启步进电机
	}
	else if(range<0)
	{
		MOTOR_MODE=LEFT_MODE ;
    MOTOR_NUM=-range;
		TIM1->CR1 |= (0x01<<0);//开启步进电机
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

