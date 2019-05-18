#include "LED.h"


void LED_init(void)
{
	RCC->APB2ENR |=0x1<<3;
	
	GPIOB->CRH &=~(0xF<<12);
	GPIOB->CRH |=(0x02<<12);
	
	
	LED1=0;
}
