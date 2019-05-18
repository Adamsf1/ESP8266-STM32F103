#include "time.h"
struct Http_Data GetData={0};
struct Http_Data DeviceData={0};
struct Device_Flag Device_Flag={0};
void timer2_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1;//��ʹ�ܶ�ʱ��ʱ��Դ  ��6��
	
	TIM2->PSC = psc-1;  //  ����ʱ�� = ʱ��Դ / PSC + 1    ���÷�Ƶֵ
	TIM2->ARR = arr-1;  //������װ��ֵ
	
	TIM2->DIER |= 1;//ʹ���ж�--����
  NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM2_IRQn);//�ں��ж�ʹ��
	
	TIM2->CR1 &=~(0x01<<0);//�رռ�����
}



u8 dht[5];
void TIM2_IRQHandler(void)
{
	TIM2->SR &=~(0X01<<0); //������±�־
	while(TIM2->SR&0x01);  //�ȴ������¼���־���
	
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

//����ʱ��(���ʱ��)= ��������X�������� = 1/(72M/PSC) X ARR=(ARR*PSC)/72M

void timer3_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1<<1;//��ʹ�ܶ�ʱ��ʱ��Դ  ��6��
	
	TIM3->PSC = psc-1;  //  ����ʱ�� = ʱ��Դ / PSC + 1    ���÷�Ƶֵ
	TIM3->ARR = arr-1;  //������װ��ֵ
	
	TIM3->DIER |= 1;//ʹ���ж�--����
	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM3_IRQn);//�ں��ж�ʹ��
	
	TIM3->CR1 &=~(0x01<<0);//�ؼ�����
}


void TIM3_IRQHandler(void)
{
	TIM3->SR &=~(0X01<<0); //������±�־
	while(TIM3->SR&0x01);  //�ȴ������¼���־���
	

}



void timer4_init(u16 psc,u16 arr)
{
  RCC->APB1ENR |= 1<<2;//��ʹ�ܶ�ʱ��ʱ��Դ  
	
	TIM4->PSC = psc-1;  //  ����ʱ�� = ʱ��Դ / PSC + 1    ���÷�Ƶֵ
	TIM4->ARR = arr-1;  //������װ��ֵ
	
	TIM4->DIER |= 1;//ʹ���ж�--����
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(7-2,1,3));
	NVIC_EnableIRQ(TIM4_IRQn);//�ں��ж�ʹ��
	
	TIM4->CR1 &=~(0x01<<0);//�ؼ�����
}



u16 KEY_cnt=0;
void TIM4_IRQHandler(void)
{
	TIM4->SR &=~(0X01<<0); //������±�־
	while(TIM4->SR&0x01);  //�ȴ������¼���־���
	
	if(Uart2_Rev.rev_over==1)
	{
		Uart2_Rev.rev_over=0;
	//	uart1_send_string((u8 *)Uart2_Rev.rev_buf);	
		memset(Uart2_Rev.rev_write,0,sizeof(Uart2_Rev.rev_write));//����
		DMA1_chanel1_Enble((u32)Uart2_Rev.rev_buf,(u32) Uart2_Rev.rev_write ,strlen((const char *)Uart2_Rev.rev_buf));		
		
		TIM4->CR1 &=~(0x01<<0);//�رռ�����
	}
	
	if(Uart2_Rev.rev_start)//��ʼ���գ���ʱ��ʱ
	{
		if(++Uart2_Rev.timeout>100)//������ɣ���������ɴ���
		{
			Uart2_Rev.rev_buf[Uart2_Rev.rev_len]='\0';
			Uart2_Rev.rev_start=0;
			Uart2_Rev.rev_len=0;
		  Uart2_Rev.timeout=0; 
			Uart2_Rev.rev_over=1;
			
		}
	}
	
}



