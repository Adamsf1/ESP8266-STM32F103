#include "DHT11.h"
//PB3(DHT11_DATA)

void DHT11_PIN_InputMode(void)
{
	RCC->APB2ENR |=0x01<<3;
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);
	GPIOB->CRL &=~(0x0F<<12);//PB3输入模式
	GPIOB->CRL |=0x04<<12;//PB3输入模式
	
	
}


void DHT11_PIN_OutputMode(void)
{
	RCC->APB2ENR |=0x01<<3;
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);
	GPIOB->CRL &=~(0x0F<<12);//PB3输出模式
	GPIOB->CRL |=0x03<<12;//PB3输出模式
	
	
}



void DHT11_Send_Request(void)
{
	DHT11_PIN_OutputMode();
	DHT11_OUT=1;
	DHT11_OUT=0;
	delay_ms(20);
	DHT11_OUT=1;
	delay_us(30);
	DHT11_PIN_InputMode();
}

u8 DHT11_Scan(void)
{
	while(DHT11_IN==0);
	delay_us(40);
	if(DHT11_IN==1)
	{
		while(DHT11_IN==1);
		return 1;
	}
	else return 0;
	
}




u16 DHT11_Read_byte(void)
{
	u16 data=0;
	u8 i;
	for(i=0;i<8;i++)
	{
		data <<=1;
		data |=DHT11_Scan();
	}
	return data;
	
}

u16 DHT11_Rec_Data(u8 DHT11_DATA[5])
{
	u8 i;
	
	DHT11_Send_Request();
	if(DHT11_IN==0)
	{
		while(DHT11_IN==0);
		while(DHT11_IN==1);
	
		for(i=0;i<5;i++)
		{
			DHT11_DATA[i]=DHT11_Read_byte();
		}
		
		while(DHT11_IN==0);
		DHT11_PIN_OutputMode();
		DHT11_OUT=1;
		
		u8 Check=DHT11_DATA[0]+DHT11_DATA[1]+DHT11_DATA[2]+DHT11_DATA[3];
		if(Check!=DHT11_DATA[4])
		{
			printf("DHT11 Check error\r\n");
			return 1;
		}
		else return 0;
	}
	else return 2;
}



	
