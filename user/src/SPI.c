#include "SPI.h"

//OLED_SCLK----SCK----PB13----普通功能推挽输出
//OLED_SDIN---MOSI----PB15---普通功能推挽输出

void SPI2_PIN_Init(void)
{
	RCC->APB2ENR |=0x01<<3;
	
	GPIOB->CRH &=~(0x0F<<20);	
	GPIOB->CRH |=0x0B<<20;		//PB13输出模式
	GPIOB->CRH &=~(0x0F<<28);	
	GPIOB->CRH |=0x0B<<28;		//PB15输出模式
	
	RCC->APB1ENR |=0x01<<14;
	//SPI寄存器配置
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
