#ifndef __DS18B20_H__
#define __DS18B20_H__


//#include <reg51.h>
#include <STC12C5A60S2.h>

//DS18B20 IO设置
sbit DS18B20_DQ = P1^5;


//函数或者变量声明
extern void DS18B20_Delay( unsigned int n );
extern void DS18B20_Write_Byte( unsigned char dat);
extern unsigned char DS18B20_Read_Byte( );
extern void Init_Ds18b20(void) ;
unsigned int Get_temp(void);



#endif
