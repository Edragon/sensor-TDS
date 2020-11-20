/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * OLED :CS -> P0.3; RST -> P0.1; DC -> P0.2;SCL -> P2.2;SDIN -> P0.0;
 *         D0~D7 -> P0口
 * TDS传感器模块: VCC -> 5V; GND -> GND; AO -> P1.3;
 * 温度传感器DS18B20：VCC-> 5V;GND -> GND ;T -> P1.5
 * 功能描述：测量TDS值、温度值液晶显示；
             串口接收测量所得的TDS值（波特率9600）；
 *           可连接上位机显示TDS值；

 * 淘宝    ：https://ilovemcu.taobao.com
**********************************************************************************/

#include <STC12C5A60S2.h>
#include <intrins.h>
#include "oled.h"
#include "DS18B20.h"
#define	uchar	unsigned char
#define uint	unsigned int
	


#define MAIN_Fosc		11059200L	//定义主时钟, 模拟串口和和延时会自动适应。5~35MHZ  22118400L 


#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0
#define ADC_FLAG	(1 << 4)	//软件清0
#define ADC_START	(1 << 3)	//自动清0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7

/**************变量定义***************************/
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float temp_data=0.0;
float compensationCoefficient=1.0;//温度校准系数
float compensationVolatge;
float kValue=1.0;

unsigned char disbuff[18]={0}; 
void DelayUs(unsigned int time);   //延时子函数
void Delay1(unsigned int time);		 //延时主程序
void  delay_ms_AD(unsigned char ms);

uint	adc10_start(uchar channel);	//channel = 0~7
uint i;

void delay_1s()	  //延时1s
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
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
void TDS_Value_Conversion()
{
	TDS_voltage=(TDS*5.0)/1024;
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
		OLED_ShowChar(36,2,disbuff[0]);//显示ASCII字符
		OLED_ShowChar(45,2,disbuff[1]);//显示ASCII字符
		OLED_ShowChar(54,2,disbuff[2]);//显示ASCII字符
		OLED_ShowChar(63,2,disbuff[3]);//显示ASCII字符	
}

void TEMP_Value_Conversion()
{
	  disbuff[4]=(int)(temp_data)/1000+'0';
	  disbuff[5]=(int)(temp_data)%1000/100+'0';	
	  disbuff[6]=(int)(temp_data)%100/10+'0';
	  disbuff[7]='.';
	  disbuff[8]=(int)(temp_data)%10+'0';


		OLED_ShowChar(24,4,disbuff[4]);//显示ASCII字符
		OLED_ShowChar(33,4,disbuff[5]);//显示ASCII字符
		OLED_ShowChar(42,4,disbuff[6]);//显示ASCII字符
		OLED_ShowChar(51,4,disbuff[7]);//显示ASCII字符	
	  OLED_ShowChar(60,4,disbuff[8]);//显示ASCII字符	

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
	StartUART();
	Init_Ds18b20();
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 

	P1ASF = 0x0F;			//12C5A60AD/S2系列模拟输入(AD)选择
	ADC_CONTR = ADC_360T | ADC_ON;
	OLED_ShowString(0,0,"T D S ");	
	OLED_ShowCHinese(54,0,3);
	OLED_ShowCHinese(72,0,4);
	OLED_ShowCHinese(90,0,5);
	OLED_ShowCHinese(108,0,6);
	OLED_ShowString(0,2,"TDS:");	
	OLED_ShowString(0,4,"T:");	
	while(1)
	{
		  temp_data=Get_temp();
			TDS = adc10_start(3);				
			TDS_Value_Conversion();
		  TEMP_Value_Conversion();
		
					for(i=0;i<9;i++)
				{
					R_S_Byte(disbuff[i]);
				}	
				R_S_Byte('\r');//发送回车换行符
				R_S_Byte('\n');
			  delay_1s();
	}
}
	

///********************* 做一次ADC转换 *******************/
uint	adc10_start(uchar channel)	//channel = 0~7
{
	uint	adc;
	uchar	i;

	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 

	i = 250;
	do{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
			adc = (uint)ADC_RES;
			adc = (adc << 2) | (ADC_RESL & 3);
			return	adc;
		}
	}while(--i);
	return	1024;
}

//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数.
// 返回: none.
// 版本: VER1.0
// 日期: 2010-12-15
// 备注: 
//========================================================================
void  delay_ms_AD(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}



