#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include "stm32f10x.h"
#include "F103io_bit.h"
#include "stdio.h"
#include "delay.h"


#define LEFT_MODE    0
#define RIGHT_MODE   1
#define DHT_Flash (s16 *)(0x0801FC00)
#define A   PAout(7)
#define A_  PAout(6)
#define B   PAout(5)
#define B_  PAout(4)

#define BEAT1  {A=1;A_=0;B=0;B_=1;}
#define BEAT2  {A=0;A_=1;B=0;B_=1;}
#define BEAT3  {A=0;A_=1;B=1;B_=0;}
#define BEAT4  {A=1;A_=0;B=1;B_=0;}

void timer1_init(void);
void StepMotorInit(void);
void WindowControl(s16 range);
void SteperMotor_Contorl(s16 target);
void Save_Data(s16 data);
#endif
