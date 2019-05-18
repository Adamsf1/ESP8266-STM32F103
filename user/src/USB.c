#include "USB.h"

//PB1
void USB_Init(void)
{
	RCC->APB2ENR |=0x01<<3; //GPIOB时钟开启
	
	GPIOB->CRL &=~(0x0F<<4);//PB1输出模式
	GPIOB->CRL |=0x03<<4;//PB1输出模式
}
//KEY1 PA0
//KEY2 PA8
//KEY3 PB10
void key_Init(void)
{
	RCC->APB2ENR |=0x01<<2;
	
	GPIOA->CRL &=~(0xF<<0);
	GPIOA->CRL|=(0x4<<0);		//KEY1
	GPIOA->CRH &=~(0xF<<0);
	GPIOA->CRH |=(0x4<<0);	//KEY2
	
	RCC->APB2ENR |=0x01<<3;
	GPIOB->CRH &=~(0xF<<8);
	GPIOB->CRH |=(0x4<<8); 	//KEY3
	
	
}

void USB_Contorl(u8 value)
{
	if(value)
	{
		USB_switch=1;
	}
	else 
	{
		USB_switch=0;
	}
}



