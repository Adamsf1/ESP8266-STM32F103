#ifndef _UART_H_
#define _UART_H_
#include "string.h"

#include "stm32f10x.h"

  
typedef struct
{
	u8 rev_write[1024];//���ջ�����
	u8 rev_buf[1024];
	u8 rev_over;      //������ɱ�־ 0��ʾû���յ��µ��ַ���  1��ʾ�յ��µ��ַ���
	u16 rev_len;      //���ջ�����ƫ�Ƶ�ַ
	u8 timeout;       //��ʱ����ֵ
	u8 rev_start;     //��ʼ���ձ�־  1��ʾ��ʼ�յ�������
	
}TYPE_UART_REV;


extern TYPE_UART_REV Uart1_Rev,Uart2_Rev;

void uart1_init(u32 apb2clk,u32 bound);
void uart2_init(u32 apb2clk,u32 bound);
void uart1_send_string(u8 *p);
void uart2_send_string(u8 *p);
#endif
