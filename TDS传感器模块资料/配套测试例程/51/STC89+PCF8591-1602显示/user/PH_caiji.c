/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * PCF8591:VCC -> 5V; GND -> GND; SCL -> P1.6;SDA -> P1.7; 
 *         OUT1~OUT8 -> P3��
 * LCD1602:RS -> P2.5; RW -> P2.6; E -> P2.7;
 *         D0~D7 -> P0��
 * TDS������ģ��: VCC -> 5V; GND -> GND; AO -> AIN0;
 *
 * ��������������TDSֵҺ����ʾ��
             ���ڽ��ղ������õ�TDSֵ��������9600����
 *           ��������λ����ʾTDSֵ��
 * �Ա�    ��https://ilovemcu.taobao.com
**********************************************************************************/

//#include <reg52.h>
#include <include.h>
#include "DS18B20.h"

typedef unsigned char uchar;
typedef unsigned char uint;

/**************�ⲿ��������************************/
int num=0; 
/**************��������***************************/
double PH=0.0;
unsigned char disbuff[9]={0};
double ADC_Voltage=0.0;
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float temp_data=25.0;
float compensationCoefficient=1.0;//�¶�У׼ϵ��
float compensationVolatge;
float kValue=1.0;

/*
 * ��������Get_Voltage_Value
 * ����  ����ȡADC0809��ת����ѹֵ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 *						AIN0(0X40):��������
 *						AIN1(0X41):����
 *					  AIN2(0X42):����������
 *					  AIN3(0X43):��������
 */
void Get_Voltage_Value()
{
			ISendByte(PCF8591,0x40);
      num=IRcvByte(PCF8591);  //ADC0 ģ��ת��1      PH�缫
		  ADC_Voltage =num * 5.0/ 255;  //ת��Ϊ��ѹֵ
}
/*
 * ��������Character_Conversion
 * ����  ����ʱ����
 *         
 * ����  ��unsigned char n����ʱ��
 * ���  ����
 * ˵��  ��
 */
/**************TDSֵ�ɼ�����***************/
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
		
		/*��ʾTDS*/
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
{  							//������9600
     SCON = 0x50;
     TMOD = 0x20;
     TH1 = 0xFD;
     TL1 = 0xFD; 
     PCON = 0x00;
     TR1 = 1;
}
//**********ͨ�����ڽ����յ����ݷ��͸�PC��**************
void R_S_Byte(uchar R_Byte)
{	
	 SBUF = R_Byte;  
     while( TI == 0 );				//��ѯ��
  	 TI = 0;    
}
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
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
				R_S_Byte('\r');//���ͻس����з�
				R_S_Byte('\n');
				delay_1s();
  }	
}
