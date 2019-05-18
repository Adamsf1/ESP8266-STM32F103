#include "OLED.h"



//OLED_SCLK----SCK----PB13----普通功能推挽输出
//OLED_SDIN---MOSI----PB15---普通功能推挽输出
//OLED_CS----PB7----普通功能推挽输出
//OLED_RES---PB6----普通功能推挽输出
//OLED_D/C---PB12----普通功能推挽输出

void OLED_PIN_Init(void)
{
	RCC->APB2ENR |=0x01<<3;

	GPIOB->CRL &=~(0xFF<<24);
	GPIOB->CRL |=(0x33<<24);
	GPIOB->CRH &=~(0x0F<<16);
	GPIOB->CRH |=(0x03<<16);
	
	CS=1;
	RES=1;

}	
void OLED_Send_Command(u8 cmd)
{
	CS=0;
	DC=0;
	SPI2_Send_Byte(cmd);
	CS=1;
	
}

void OLED_Send_Data(u8 data)
{
	CS=0;
	DC=1;
	SPI2_Send_Byte(data);
	CS=1;
	
}

void OLED_Clear(void)
{
	u8 page,i;
	for(page=0;page<8;page++)
	{
		OLED_Send_Command(0xB0+page);
		OLED_Send_Command(0x10|0);
		OLED_Send_Command(0x00|0);
		for(i=0;i<128;i++)
		OLED_Send_Data(0);
	}
}

void OLED_Init(void)
{
	OLED_PIN_Init();
	SPI2_PIN_Init();
	
	RES=1;
	delay_ms(100);
	RES=0;
	delay_ms(100);
	RES=1;
	delay_ms(100);
	OLED_Send_Command(0xAE); //关闭显示
  OLED_Send_Command(0xD5); //设置时钟分频因子,震荡频率
  OLED_Send_Command(80);   //[3:0],分频因子;[7:4],震荡频率
  OLED_Send_Command(0xA8); //设置驱动路数
  OLED_Send_Command(0X3F); //默认0X3F(1/64)
  OLED_Send_Command(0xD3); //设置显示偏移
  OLED_Send_Command(0X00); //默认为0

  OLED_Send_Command(0x40); //设置显示开始行 [5:0],行数.

  OLED_Send_Command(0x8D); //电荷泵设置
  OLED_Send_Command(0x14); //bit2，开启/关闭
  OLED_Send_Command(0x20); //设置内存地址模式
  OLED_Send_Command(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLED_Send_Command(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
  OLED_Send_Command(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
  OLED_Send_Command(0xDA); //设置COM硬件引脚配置
  OLED_Send_Command(0x12); //[5:4]配置

  OLED_Send_Command(0x81); //对比度设置
  OLED_Send_Command(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
  OLED_Send_Command(0xD9); //设置预充电周期
  OLED_Send_Command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_Send_Command(0xDB); //设置VCOMH 电压倍率
  OLED_Send_Command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_Send_Command(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLED_Send_Command(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
  OLED_Send_Command(0xAF); //开启显示
	
	
	OLED_Clear();
	
}

void Set_position(u8 cloumn ,u8 page)
{
		OLED_Send_Command(0xB0+page);
		OLED_Send_Command(0x10|(cloumn>>4));
		OLED_Send_Command(0x00|(0x0F&cloumn));
}


void OLED_Show_ASCII(u8 cloumn,u8 page,u8 ch)
{
	u8 i,ascii;
	ascii=ch-' ';
	
	Set_position(cloumn ,page);
		for(i=0;i<8;i++)
		{
			OLED_Send_Data(Aciss_8X16[ascii*16+i]);
		}
	
	
	Set_position(cloumn ,page+1);
		for(i=0;i<8;i++)
		{
			OLED_Send_Data(Aciss_8X16[ascii*16+i+8]);
		}

}


void OLED_Show_Word(u8 cloumn,u8 page,u8* word)
{
	u8 i,n;
	
	for(i=page;i<page+2;i++)
	{
		Set_position(cloumn,i);
		for(n=0;n<16;n++)
		OLED_Send_Data(*word++);
		
	}
}
void OLED_String(u8 cloumn,u8 page,u8 *ch)
{
	u8 i,ascii,n;
	
	for(n=0;n<strlen((const char*)ch);n++)
	{
		ascii=*(ch+n)-' ';
		Set_position(cloumn ,page);
		for(i=0;i<8;i++)
		{
			OLED_Send_Data(Aciss_8X16[ascii*16+i]);
		}

		Set_position(cloumn ,page+1);
		for(i=0;i<8;i++)
		{
			OLED_Send_Data(Aciss_8X16[ascii*16+i+8]);
		}
		
		cloumn+=8;
	}
}

void OLED_Show_Humidity(u8 x,u8 page,u8 buf[5])
{
		OLED_Show_ASCII(x,page,buf[0]/10+48);
		OLED_Show_ASCII(x+8,page,buf[0]/1%10+48);
		OLED_Show_ASCII(x+16,page,'.');
		OLED_Show_ASCII(x+24,page,buf[1]+48);
}
void OLED_Show_Temperature(u8 x,u8 page,u8 buf[5])
{
		OLED_Show_ASCII(x,page,buf[2]/10+48);
		OLED_Show_ASCII(x+8,page,buf[2]/1%10+48);
		OLED_Show_ASCII(x+16,page,'.');
		OLED_Show_ASCII(x+24,page,buf[3]+48);
}


void OLED_Show_Information(void)
{
	OLED_Clear();
	OLED_String(0,0,(u8*)"Adams");
	OLED_Show_Word(0,2,wen);
	OLED_Show_Word(16,2,du);
	OLED_Show_ASCII(32,2,':');
	OLED_Show_Word(0,4,shi);
	OLED_Show_Word(16,4,du);
	OLED_Show_ASCII(32,4,':');
}
