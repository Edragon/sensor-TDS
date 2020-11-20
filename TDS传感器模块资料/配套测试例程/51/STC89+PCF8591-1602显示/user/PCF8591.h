#ifndef _PCF8591_H_
#define _PCF8591_H_

#define  PCF8591 0x90    //PCF8591 µÿ÷∑

bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);
bit ISendByte(unsigned char sla,unsigned char c);
unsigned char IRcvByte(unsigned char sla);

#endif