#ifndef _UART_H_
#define _UART_H_
#include "string.h"

#include "stm32f10x.h"

  
typedef struct
{
	u8 rev_write[1024];//接收缓冲区
	u8 rev_buf[1024];
	u8 rev_over;      //接收完成标志 0表示没有收到新的字符串  1表示收到新的字符串
	u16 rev_len;      //接收缓冲区偏移地址
	u8 timeout;       //超时计数值
	u8 rev_start;     //开始接收标志  1表示开始收到新数据
	
}TYPE_UART_REV;


extern TYPE_UART_REV Uart1_Rev,Uart2_Rev;

void uart1_init(u32 apb2clk,u32 bound);
void uart2_init(u32 apb2clk,u32 bound);
void uart1_send_string(u8 *p);
void uart2_send_string(u8 *p);
#endif
