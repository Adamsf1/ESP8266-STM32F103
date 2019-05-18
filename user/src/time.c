#include "time.h"
struct Http_Data GetData={0};
struct Http_Data DeviceData={0};
struct Device_Flag Device_Flag={0};
void timer2_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1;//①使能定时器时钟源  第6章
	
	TIM2->PSC = psc-1;  //  计数时钟 = 时钟源 / PSC + 1    设置分频值
	TIM2->ARR = arr-1;  //设置重装载值
	
	TIM2->DIER |= 1;//使能中断--外设
  NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM2_IRQn);//内核中断使能
	
	TIM2->CR1 &=~(0x01<<0);//关闭计数器
}



u8 dht[5];
void TIM2_IRQHandler(void)
{
	TIM2->SR &=~(0X01<<0); //清除更新标志
	while(TIM2->SR&0x01);  //等待更新事件标志清除
	
	DHT11_Rec_Data(dht);
	OLED_Show_Temperature(40,2,dht);
	OLED_Show_Humidity(40,4,dht);
	
	Http_Check_Receive();	
	if(GET_Data()==1)
	{
		GetData=Http_Save_Data();
		//printf("Device: Red:%d,Green:%d,blue:%d,StepMotor:%d,USB:%d\r\n",DeviceData.RGB_Red,DeviceData.RGB_Green,DeviceData.RGB_Blue,DeviceData.StepMotor,DeviceData.USB);
		if((DeviceData.RGB_Red!=GetData.RGB_Red||DeviceData.RGB_Green!=GetData.RGB_Green||DeviceData.RGB_Blue!=GetData.RGB_Blue)&&GetData.RGB_Blue<=255&&GetData.RGB_Red<=255&&GetData.RGB_Green<=255)
		{
			Device_Flag.RGB_flag=1;
		}
		
		if(DeviceData.USB!=GetData.USB&&(GetData.USB==1||GetData.USB==0))
		{
				Device_Flag.USB_flag=1;
		}
		
		if(DeviceData.StepMotor!=GetData.StepMotor&&GetData.StepMotor<=10&&GetData.StepMotor>=0)
		{
			
			Device_Flag.StepMotor_flag=1;
		}
		
		if(DeviceData.Motor!=GetData.Motor&&GetData.Motor<=100)
		{
			Device_Flag.Motor_flag=1;
		}
		
	}


}

//计数时间(溢出时间)= 计数周期X计数次数 = 1/(72M/PSC) X ARR=(ARR*PSC)/72M

void timer3_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1<<1;//①使能定时器时钟源  第6章
	
	TIM3->PSC = psc-1;  //  计数时钟 = 时钟源 / PSC + 1    设置分频值
	TIM3->ARR = arr-1;  //设置重装载值
	
	TIM3->DIER |= 1;//使能中断--外设
	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM3_IRQn);//内核中断使能
	
	TIM3->CR1 &=~(0x01<<0);//关计数器
}


void TIM3_IRQHandler(void)
{
	TIM3->SR &=~(0X01<<0); //清除更新标志
	while(TIM3->SR&0x01);  //等待更新事件标志清除
	

}



void timer4_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1<<2;//①使能定时器时钟源  
	
	TIM4->PSC = psc-1;  //  计数时钟 = 时钟源 / PSC + 1    设置分频值
	TIM4->ARR = arr-1;  //设置重装载值
	
	TIM4->DIER |= 1;//使能中断--外设
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM4_IRQn);//内核中断使能
	
	TIM4->CR1 &=~(0x01<<0);//关计数器
}



u16 KEY_cnt=0;
void TIM4_IRQHandler(void)
{
	TIM4->SR &=~(0X01<<0); //清除更新标志
	while(TIM4->SR&0x01);  //等待更新事件标志清除
	
	if(Uart2_Rev.rev_over==1)
	{
		Uart2_Rev.rev_over=0;
	//	uart1_send_string((u8 *)Uart2_Rev.rev_buf);	
		memset(Uart2_Rev.rev_write,0,sizeof(Uart2_Rev.rev_write));//清零
		DMA1_chanel1_Enble((u32)Uart2_Rev.rev_buf,(u32) Uart2_Rev.rev_write ,strlen((const char *)Uart2_Rev.rev_buf));		
		
		TIM4->CR1 &=~(0x01<<0);//关闭计数器
	}
	
	if(Uart2_Rev.rev_start)//开始接收，超时计时
	{
		if(++Uart2_Rev.timeout>100)//接收完成，做接收完成处理
		{
			Uart2_Rev.rev_buf[Uart2_Rev.rev_len]='\0';
			Uart2_Rev.rev_start=0;
			Uart2_Rev.rev_len=0;
		  Uart2_Rev.timeout=0; 
			Uart2_Rev.rev_over=1;
			
		}
	}
	
}



