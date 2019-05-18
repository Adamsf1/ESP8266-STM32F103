#include "SPI.h"

//OLED_SCLK----SCK----PB13----��ͨ�����������
//OLED_SDIN---MOSI----PB15---��ͨ�����������

void SPI2_PIN_Init(void)
{
	RCC->APB2ENR |=0x01<<3;
	
	GPIOB->CRH &=~(0x0F<<20);	
	GPIOB->CRH |=0x0B<<20;		//PB13���ģʽ
	GPIOB->CRH &=~(0x0F<<28);	
	GPIOB->CRH |=0x0B<<28;		//PB15���ģʽ
	
	RCC->APB1ENR |=0x01<<14;
	//SPI�Ĵ�������
	SPI2->CR1=0;
	SPI2->CR1 |=0x03<<8;
	SPI2->CR1 |=0x01<<2;
	
	SPI2->CR1 |=0x01<<6;
}


u8 SPI2_Send_Byte(u8 data)
{
	
	while(!(SPI2->SR&(0x01<<1)));
	SPI2->DR=data;
	while(!(SPI2->SR&(0x01<<0)));
	return 	SPI2->DR;
}
