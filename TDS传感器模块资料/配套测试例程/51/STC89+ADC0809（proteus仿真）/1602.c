#include "1602.h"

void Delay(unsigned int time)
{
    unsigned int timeCounter = 0;
	for (timeCounter = time;timeCounter > 0 ;timeCounter --)
		DelayUs(255);
}

void DelayUs(unsigned int time)
{
	unsigned int timeCounter = 0;
	for (timeCounter = 0;timeCounter < time;timeCounter ++)
		_nop_();
}

/*---------------------------------  LCD��ʼ������--------------------------------------*/

void LCDInit(void)
{
    //������ʾģʽ����
	LCD_Data=0;
	LCD_E=Disable;
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);

	WriteCmd(LCD_Init);	   						//��ʼ��
	WriteCmd(LCD_CloseCtr);							//����ʾ
	WriteCmd(LCD_CLS);								//����Ļ
	WriteCmd(LCD_EnterSet);							//����ƶ�����
	WriteCmd(LCD_DispCtr);							//��ʾ���Լ��������
	
}

/*---------------------------------  LCDģʽ���ú���--------------------------------------*\
	
	SetWriteCmd() 	 			����LCDΪд����ģʽ
	SetReadCmd()   		    		����LCDΪ������ģʽ
	SetWriteData()				����LCDΪд����ģʽ
	
\*----------------------------------------------------------------------------------------*/
void SetWriteCmd(void)
{
	LCD_RW=Write;
	LCD_RS=Cmd;
}

void SetReadCmd(void)
{
	LCD_RW=Read;
	LCD_RS=Cmd;
}

void SetWriteData(void)
{
	LCD_RW=Write;
	LCD_RS=Data;
}

/*---------------------------------  LCD����ִ�к���--------------------------------------*\
	
	WriteCmd()					д����
	WriteData()					д����
	ExecuteCmd()					ִ������
	SetXY()						��ʾ��λ
	DisplaySingleChar()			��ʾ�����ַ�
	DisplayString()				��ʾһ���ַ�
	IsBusy()						æ��־���
\*----------------------------------------------------------------------------------------*/
void WriteCmd(char cmd)
{
	while(IsBusy());
	LCD_Data=cmd;	
	SetWriteCmd();
	ExecuteCmd();
}

void WriteData(char ddata)
{
	while(IsBusy());
	LCD_Data=ddata;
	SetWriteData();	
	ExecuteCmd();
}

void ExecuteCmd(void)
{
	LCD_E=Enable;
	LCD_E=Disable;
}

void SetXY(char x,char y)
{
	if (y)
		x|=0x40;
	x|=0x80;
	Delay(5);
	WriteCmd(x);

}

void DisplaySingleChar(char x,char y,char cchar)
{
	Delay(5);
	SetXY(x,y);
	WriteData(cchar);	
}

void DisplayString(char x,char y,char *str)
{
	while(*str)
	{
		Delay(5);
		DisplaySingleChar(x++,y,*str);
		str++;
	}
}

bit IsBusy(void)
{
	LCD_Data=0xFF;
	SetReadCmd();
	ExecuteCmd();
	return (bit)(LCD_Data & 0x80);	
}