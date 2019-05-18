#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "LED.h"
#include "delay.h"

//TX	PA9
//RX	PA10

/*配置步骤：																
	(1)打开GPIO和USART的时钟												
	(2)设置USART两个管脚GPIO模式											  
	(3)配置USART数据格式、波特率等参数										 
	(4)最后使能USART功能													  
*/

void uart1_init(u32 apb2clk,u32 bound)
{
	//1.使能AFIO,GPIO和USART部件的时钟
	RCC->APB2ENR |=0x01<<0;
	RCC->APB2ENR |=0x01<<2;
	RCC->APB2ENR |=0x01<<14;
	
	//2.将USART Tx的GPIO配置为推挽复用模式,最大速度50MHz
	GPIOA->CRH &=~(0xF<<4);
	GPIOA->CRH |=(0xB<<4);
	
	//3.将USART Rx的GPIO配置为浮空输入模式
	GPIOA->CRH &=~(0xF<<8);
	GPIOA->CRH |=(0x01<<10);
	
	//4.配置USART参数
	USART1->BRR =apb2clk*1000000/bound;//16*div=fck/bound
	USART1->CR1 &=~(0x01<<12);         	//数据长度为8位
	USART1->CR2 &=~(0x03<<12);    			//设置停止位为1位
	
	//nvic中设置uart1的优先级、使能uart1中断
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));
	NVIC_EnableIRQ(USART1_IRQn);
	//接收缓冲区非空中断使能
	USART1->CR1 |=0X01<<5; 
	
	//5.	使能接受、使能发送、开模块。
	USART1->CR1 |=0X01<<3;    //开串口的发送功能
	USART1->CR1 |=0X01<<2;    //开串口的接受功能
	USART1->CR1 |=0X01<<13;   //开串口1
	

}



TYPE_UART_REV Uart1_Rev={0},Uart2_Rev={0};

void USART1_IRQHandler(void)
{
	u8 data;
	
	if(USART1->SR & (0X01<<5))  //接收中断
	{
		data=USART1->DR;
		while((USART2->SR & (0X01<<7))==0);  //等待发送缓冲区为空
		USART2->DR=data;
		
	}
	
}



//PA2--TX
//PA3--RX
void uart2_init(u32 apb2clk,u32 bound)
{
	//1.使能AFIO,GPIO和USART部件的时钟
	RCC->APB2ENR |=0x01<<0;
	RCC->APB2ENR |=0x01<<2;
	RCC->APB1ENR |=0x01<<17;
	
	//2.将USART Tx的GPIO配置为推挽复用模式,最大速度50MHz
	GPIOA->CRL &=~(0xF<<8);
	GPIOA->CRL |=(0xB<<8);
	
	//3.将USART Rx的GPIO配置为浮空输入模式
	GPIOA->CRL &=~(0xF<<12);
	GPIOA->CRL |=(0x01<<14);
	
	//4.配置USART参数
	USART2->BRR =apb2clk*1000000/bound;//16*div=fck/bound
	USART2->CR1 &=~(0x01<<12);         	//数据长度为8位
	USART2->CR2 &=~(0x03<<12);    			//设置停止位为1位
	
	//nvic中设置uart1的优先级、使能uart1中断
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,3,4));
	NVIC_EnableIRQ(USART2_IRQn);
	//接收缓冲区非空中断使能
	USART2->CR1 |=0X01<<5; 
	
	//5.	使能接受、使能发送、开模块。
	USART2->CR1 |=0X01<<3;    //开串口的发送功能
	USART2->CR1 |=0X01<<2;    //开串口的接受功能
	USART2->CR1 |=0X01<<13;   //开串口1
	

}



void USART2_IRQHandler(void)
{
		Uart2_Rev.rev_buf[Uart2_Rev.rev_len++]=USART2->DR;//读取数据
		Uart2_Rev.rev_start=1;
		Uart2_Rev.timeout=0;
		TIM4->CR1 |=0x01<<0;//开计数器2
		
}

//串口1发送字符串
void uart1_send_string(u8 *p)
{
	while(*p!='\0')
	{
		while((USART1->SR & (0X01<<7))==0);  //等待发送缓冲区为空
		USART1->DR=*p++;
	}
}


//串口2发送字符串
void uart2_send_string(u8 *p)
{
	while(*p!='\0')
	{
		while((USART2->SR & (0X01<<7))==0);  //等待发送缓冲区为空
		USART2->DR=*p++;
	}
}





#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);  //等待之前的数据发送完毕
		USART1->DR=ch;
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
