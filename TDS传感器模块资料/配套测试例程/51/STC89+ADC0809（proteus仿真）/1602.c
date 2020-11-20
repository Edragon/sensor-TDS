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

/*---------------------------------  LCD初始化函数--------------------------------------*/

void LCDInit(void)
{
    //三次显示模式设置
	LCD_Data=0;
	LCD_E=Disable;
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);

	WriteCmd(LCD_Init);	   						//初始化
	WriteCmd(LCD_CloseCtr);							//关显示
	WriteCmd(LCD_CLS);								//清屏幕
	WriteCmd(LCD_EnterSet);							//光标移动设置
	WriteCmd(LCD_DispCtr);							//显示开以及光标设置
	
}

/*---------------------------------  LCD模式设置函数--------------------------------------*\
	
	SetWriteCmd() 	 			设置LCD为写命令模式
	SetReadCmd()   		    		设置LCD为读命令模式
	SetWriteData()				设置LCD为写数据模式
	
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

/*---------------------------------  LCD功能执行函数--------------------------------------*\
	
	WriteCmd()					写命令
	WriteData()					写数据
	ExecuteCmd()					执行命令
	SetXY()						显示定位
	DisplaySingleChar()			显示单个字符
	DisplayString()				显示一串字符
	IsBusy()						忙标志检测
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