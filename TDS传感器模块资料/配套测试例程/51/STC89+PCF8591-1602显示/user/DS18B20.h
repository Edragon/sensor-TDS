#ifndef __DS18B20_H__
#define __DS18B20_H__


#include <reg52.h>
//#include <STC12C5A60S2.h>
//#include <include.h>
//DS18B20 IO����
sbit DS18B20_DQ = P1^1;


//�������߱�������
extern void DS18B20_Delay( unsigned int n );
extern void DS18B20_Write_Byte( unsigned char dat);
extern unsigned char DS18B20_Read_Byte( );
extern void Init_Ds18b20(void) ;
unsigned int Get_Temp()   ;                //��ȡ�¶�get the wenduerature



#endif
