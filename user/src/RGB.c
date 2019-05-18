#include "RGB.h"

//PB9(RGB_DATA),PB8(RGB_CLK)
void RGB_PIN_Init(void)
{
	RCC->APB2ENR |=0x01<<3;
	
	GPIOB->CRH &=~(0xFF<<0);
	GPIOB->CRH |=(0x11<<0);
	RGB_Control(0,0,0);
}

u32 Color_DATA(u8 r,u8 g,u8 b)
{
	u32 RGB_COLOR=0;
	RGB_COLOR |=0x03<<30;
	RGB_COLOR |=((~(b)&0xc0)>>6)<<28;
	RGB_COLOR	|=((~(g)&0xc0)>>6)<<26;
	RGB_COLOR	|=((~(r)&0xc0)>>6)<<24;
	RGB_COLOR	|=b<<16;
	RGB_COLOR	|=g<<8;
	RGB_COLOR	|=r<<0;
	
	return RGB_COLOR;
	
}

void RGB_DATA_Send(u32 RGB_data)
{
	u8 i;
	for(i=0;i<32;i++)
	{	
		if(RGB_data&0x80000000)
		{
			RGB_DATA=1;
		}
		else 
		{
			RGB_DATA=0;
		}
		RGB_data<<=1;
		RGB_CLK=0;
		delay_us(200);
		RGB_CLK=1;
		delay_us(200);
	}
}

void RGB_Control(u8 r,u8 g,u8 b)
{
	u32 RGB=0;
	RGB=Color_DATA(r,g,b);
	
	RGB_DATA_Send(0);
	RGB_DATA_Send(RGB);
	RGB_DATA_Send(RGB);

}
