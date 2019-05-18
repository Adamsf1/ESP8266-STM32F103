#include "OLED.h"



//OLED_SCLK----SCK----PB13----��ͨ�����������
//OLED_SDIN---MOSI----PB15---��ͨ�����������
//OLED_CS----PB7----��ͨ�����������
//OLED_RES---PB6----��ͨ�����������
//OLED_D/C---PB12----��ͨ�����������

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
	OLED_Send_Command(0xAE); //�ر���ʾ
  OLED_Send_Command(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
  OLED_Send_Command(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
  OLED_Send_Command(0xA8); //��������·��
  OLED_Send_Command(0X3F); //Ĭ��0X3F(1/64)
  OLED_Send_Command(0xD3); //������ʾƫ��
  OLED_Send_Command(0X00); //Ĭ��Ϊ0

  OLED_Send_Command(0x40); //������ʾ��ʼ�� [5:0],����.

  OLED_Send_Command(0x8D); //��ɱ�����
  OLED_Send_Command(0x14); //bit2������/�ر�
  OLED_Send_Command(0x20); //�����ڴ��ַģʽ
  OLED_Send_Command(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
  OLED_Send_Command(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
  OLED_Send_Command(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
  OLED_Send_Command(0xDA); //����COMӲ����������
  OLED_Send_Command(0x12); //[5:4]����

  OLED_Send_Command(0x81); //�Աȶ�����
  OLED_Send_Command(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
  OLED_Send_Command(0xD9); //����Ԥ�������
  OLED_Send_Command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_Send_Command(0xDB); //����VCOMH ��ѹ����
  OLED_Send_Command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_Send_Command(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
  OLED_Send_Command(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
  OLED_Send_Command(0xAF); //������ʾ
	
	
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
