#include "DHT.h"

void DHT11_Input_init(void)
{
	RCC->APB2ENR |=0x01<<3;
	GPIOB->CRL &=~(0x0F<<12);//PB3输入模式
	GPIOB->CRL |=0x04<<12;//PB3输入模式
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);
}


void DHT11_Output_init(void)
{
	RCC->APB2ENR |=0x01<<3;
	GPIOB->CRL &=~(0x0F<<12);//PB3输出模式
	GPIOB->CRL |=0x03<<12;//PB3输出模式
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);

	
}
static u16 DHT11_Read_Bit(void)
{
	while(!PBin(3)); //等待变高电平,每次发送数据前都会有一个 50us 的电平信号
	delay_us(40); //等待 40us
	if(PBin(3) == 1)
	{
		while(PBin(3)); 
		return 1;
	}
	else
		return 0;
}

static u16 DHT11_Read_Byte(void)
{
	u8 i;
	u8 Data = 0;
	for(i=0;i<8;i++)
	{
		Data <<= 1;
		Data |= DHT11_Read_Bit();
	}
	return Data;
}

 u8 DHT11_Read(u8 buf[5])
{
	u8 i;
	DHT11_Output_init(); //设置引脚为输出模式
	PAout(3) = 1;
	PAout(3) = 0;
	delay_ms(19); //拉低至少 18ms
	PAout(3) = 1; //总线拉高 20~40us,DHT11会返回一个低电平
	delay_us(30);
	DHT11_Input_init();
	
	if(PAin(3)==0)
	{
		while(!PAin(3));
		while(PAin(3));
		while(PAin(1));
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		while(!PBin(3)); //等待 40 位数据输出完后的 50us 低电平输出结束
		DHT11_Output_init();
		PAout(3) = 1;
		if ((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
		{
				double temp;  //dht11数据发送
				double hum;
				temp=buf[0]+buf[1]/10;
				hum=buf[2]+buf[3]/10;
				printf("temp: %f hum: %f\r\n",temp,hum);

		}
		else return 0;
	}
}





