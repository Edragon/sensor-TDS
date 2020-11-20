
#ifndef ADC0809_H
#define ADC0809_H
#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char
	
extern uint num;

sbit OE  = P1^0;
sbit EOC = P1^1;
sbit ST  = P1^2;
sbit CLK = P1^3;

void Red_0809 ();

#endif


