/******************** (C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * ADC0809:OE -> P1.0; EOC -> P1.1; ST -> P1.2;CLK -> P1.3; A -> P1.4; B -> P1.5; C->P1.6;
 *         OUT1~OUT8 -> P3��
 * LCD1602:RS -> P0.0; RW -> P0.1; E -> P0.2;
 *         D0~D7 -> P2��
 * TDS������ģ��: VCC -> 5V; GND -> GND; AO -> IN3;
 *
 * ʵ��ƽ̨��
 *
 * �Ա�    ��https://ilovemcu.taobao.com
**********************************************************************************/
#include <reg52.h>
#include <adc0809.h>
#include <1602.h>
#include <intrins.h>
#include <math.h>
unsigned char disbuff[5]={0};
uint num;
float PH_V ,PH;
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float temp_data=25.0;
float compensationCoefficient=1.0;//�¶�У׼ϵ��
float compensationVolatge;
float kValue=1.0;
/**************��ʱ1s����***************/
void Delay_1s()
{
     unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
}

/**************TDSֵ�ɼ�����***************/
void TDS_Value_Conversion()
{
	Red_0809 () ;
	TDS_voltage=(num*5.0)/256;
	compensationCoefficient=1.0+0.02*((temp_data)-25.0); 
	compensationVolatge=TDS_voltage/compensationCoefficient;
	TDS_value=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 
	255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue;
	
	  if((TDS_value<=0)){TDS_value=0;}
		if((TDS_value>1400)){TDS_value=1400;}
		
		/*��ʾTDS*/
		disbuff[0]=(int)(TDS_value)/1000+'0';
		disbuff[1]=(int)(TDS_value)%1000/100+'0';	
		disbuff[2]=(int)(TDS_value)%100/10+'0';
		disbuff[3]=(int)(TDS_value)%10+'0';
		
		DisplaySingleChar(0x4,1,disbuff[0]);
		DisplaySingleChar(0x5,1,disbuff[1]);
		DisplaySingleChar(0x6,1,disbuff[2]);
		DisplaySingleChar(0x7,1,disbuff[3]);
}

/**************������***************/
void main()
{ 	
	TMOD = 0x02; //�жϳ�ʼ����adc0809����ʱ���ź�
	TH0  = 0x14;
	TL0  = 0x00;
	IE   = 0x82;
	TR0  = 1;
	P1   = 0x3f;
	LCDInit();	 //LCD��ʼ��
  DisplayString(0x0,0,"  Welcome  TDS  ");
	DisplayString(0x0,1,"Detection system");
	Delay_1s();
	WriteCmd(LCD_CLS);		 //����
	
	DisplaySingleChar(0x0,1,'T');
	DisplaySingleChar(0x1,1,'D');
	DisplaySingleChar(0x2,1,'S');
	DisplaySingleChar(0x3,1,'=');
	
	while(1)
	{
    TDS_Value_Conversion();
  }
}

void Timer0_INT() interrupt 1
{
 	CLK = !CLK;
}
