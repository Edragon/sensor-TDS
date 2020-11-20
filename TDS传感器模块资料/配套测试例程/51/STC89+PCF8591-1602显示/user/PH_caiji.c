/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * PCF8591:VCC -> 5V; GND -> GND; SCL -> P1.6;SDA -> P1.7; 
 *         OUT1~OUT8 -> P3口
 * LCD1602:RS -> P2.5; RW -> P2.6; E -> P2.7;
 *         D0~D7 -> P0口
 * TDS传感器模块: VCC -> 5V; GND -> GND; AO -> AIN0;
 *
 * 功能描述：测量TDS值液晶显示；
             串口接收测量所得的TDS值（波特率9600）；
 *           可连接上位机显示TDS值；
 * 淘宝    ：https://ilovemcu.taobao.com
**********************************************************************************/

//#include <reg52.h>
#include <include.h>
#include "DS18B20.h"

typedef unsigned char uchar;
typedef unsigned char uint;

/**************外部变量定义************************/
int num=0; 
/**************变量定义***************************/
double PH=0.0;
unsigned char disbuff[9]={0};
double ADC_Voltage=0.0;
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float temp_data=25.0;
float compensationCoefficient=1.0;//温度校准系数
float compensationVolatge;
float kValue=1.0;

/*
 * 函数名：Get_Voltage_Value
 * 描述  ：获取ADC0809的转换电压值
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 *						AIN0(0X40):热敏电阻
 *						AIN1(0X41):悬空
 *					  AIN2(0X42):滑动变阻器
 *					  AIN3(0X43):光敏电阻
 */
void Get_Voltage_Value()
{
			ISendByte(PCF8591,0x40);
      num=IRcvByte(PCF8591);  //ADC0 模数转换1      PH电极
		  ADC_Voltage =num * 5.0/ 255;  //转换为电压值
}
/*
 * 函数名：Character_Conversion
 * 描述  ：延时函数
 *         
 * 输入  ：unsigned char n：延时量
 * 输出  ：无
 * 说明  ：
 */
/**************TDS值采集函数***************/
void TDS_Value_Conversion()
{
	Get_Voltage_Value();
	TDS_voltage=(num*5.0)/256;
	compensationCoefficient=1.0+0.02*((temp_data/10)-25.0); 
	compensationVolatge=TDS_voltage/compensationCoefficient;
	TDS_value=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 
	255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue;
	
	  if((TDS_value<=0)){TDS_value=0;}
		if((TDS_value>1400)){TDS_value=1400;}
		
		/*显示TDS*/
		disbuff[0]=(int)(TDS_value)/1000+'0';
		disbuff[1]=(int)(TDS_value)%1000/100+'0';	
		disbuff[2]=(int)(TDS_value)%100/10+'0';
		disbuff[3]=(int)(TDS_value)%10+'0';
		
		DisplaySingleChar(0x4,0,disbuff[0]);
		DisplaySingleChar(0x5,0,disbuff[1]);
		DisplaySingleChar(0x6,0,disbuff[2]);
		DisplaySingleChar(0x7,0,disbuff[3]);
	
}
void StartUART( void )
{  							//波特率9600
     SCON = 0x50;
     TMOD = 0x20;
     TH1 = 0xFD;
     TL1 = 0xFD; 
     PCON = 0x00;
     TR1 = 1;
}
//**********通过串口将接收到数据发送给PC端**************
void R_S_Byte(uchar R_Byte)
{	
	 SBUF = R_Byte;  
     while( TI == 0 );				//查询法
  	 TI = 0;    
}
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
void main(void)
{
	int i;
	LCDInit();
	StartUART();
	
  DisplayString(0x0,0," Welcome   TDS  ");
	DisplayString(0x0,1,"Detection system");
	delay_1s();
	WriteCmd(LCD_CLS);
	
	DisplayString(0x0,0,"TDS:");
	while(1)
	{		
				Get_Voltage_Value();
        TDS_Value_Conversion(); 
			for(i=0;i<5;i++)
				{
					R_S_Byte(disbuff[i]);
				}	
				R_S_Byte('\r');//发送回车换行符
				R_S_Byte('\n');
				delay_1s();
  }	
}
